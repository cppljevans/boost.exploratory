//Following code copy&pasted from:
//  https://github.com/boostorg/spirit/issues/707#issuecomment-1103231416
//===============================
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

using namespace boost::spirit;

// Define AST. The root is `ast::expr`, which is the first left-hand side
// operand and a list of all operations on the right-hand side. Each operand is
// a recursive `variant` that has `ast::expr` inside.
namespace ast
{
    struct nil {};
    struct signed_;
    struct expr;

    struct operand : x3::variant
      < nil
      , double
      , x3::forward_ast<signed_>
      , x3::forward_ast<expr>
      >
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct signed_
    {
        char    sign;
        operand operand_;
    };

    struct operation
    {
        char    operator_;
        operand operand_;
    };

    struct expr
    {
        operand                first;
        std::vector<operation> rest;
    };
} // namespace ast

// Give the grammar access to the fields of AST.
BOOST_FUSION_ADAPT_STRUCT(ast::signed_, sign, operand_)
BOOST_FUSION_ADAPT_STRUCT(ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(ast::expr, first, rest)

// Arithmetic expression grammar definition.
namespace ArithExpr
{
    x3::rule<class expression, ast::expr   > const expression("expression");
    x3::rule<class term,       ast::expr   > const term("term");
    x3::rule<class factor,     ast::operand> const factor("factor");

    auto const expression_def =
        term
     >> *(
             (x3::char_('+') >> term)
           | (x3::char_('-') >> term)
         );
    auto const term_def =
        factor
     >> *(
             (x3::char_('*') >> factor)
           | (x3::char_('/') >> factor)
         );
    auto const factor_def =
        x3::double_
     | '(' >> expression >> ')'
     | (x3::char_('-') >> factor)
     | (x3::char_('+') >> factor);

    BOOST_SPIRIT_DEFINE(expression, term, factor);

    auto calc = expression;
    
    struct visitor_expression
    : boost::static_visitor<double>
    {
        double
      operator()
        ( ast::nil
        )
        {
          BOOST_ASSERT(0);
          return 0;
        }
        double
      operator()
        ( double x
        )
        {
          return x;
        }
        double
      operator()
        ( ast::signed_ const &x
        )
        {
          double rhs = boost::apply_visitor(*this, x.operand_);
          switch (x.sign) {
              case '-': return -rhs;
              case '+': return +rhs;
          }
          BOOST_ASSERT(0);
          return 0;
        }
        double
      operator()
        ( ast::expr const &x
        )
        {
          return std::accumulate
          (
              x.rest.begin(), x.rest.end(),
              // evaluate recursively left-hand side
              boost::apply_visitor(*this, x.first),
              [&](double lhs, const ast::operation &op) -> double 
              {
                  // evaluate recursively right-hand side
                  double rhs = boost::apply_visitor(*this, op.operand_);
                  switch (op.operator_) {
                      case '+': return lhs + rhs;
                      case '-': return lhs - rhs;
                      case '*': return lhs * rhs;
                      case '/': return lhs / rhs;
                  }
                  BOOST_ASSERT(0);
                  return 0;
              }
          );
        }
    };
} // namespace ArithExpr

template <typename Iterator>
double CalcArithExpr(Iterator const &first, Iterator last) 
{
    ast::expr expr;
    // Build AST.
    if (!x3::phrase_parse(first, last, ArithExpr::calc, x3::ascii::space, expr)) 
    {
        throw std::runtime_error("Cannot parse arithmetic expression");
    }

    // Parse the AST and calculate the result.
    ArithExpr::visitor_expression astEval;
    return astEval(expr);
}

int main(int argc, char *argv[]) {
    #define gen_str_val(expr) std::string str=#expr; double val=expr;
    gen_str_val(-(4.5 + 5e-1) * 2.22 - 9.1 / 3.45)
    std::cout<<":str="<<str<<";\n";
    std::cout<<":val="<<val<<";\n";
    double parse=CalcArithExpr(str.begin(), str.end());
    std::cout <<":parse="<<parse<<";\n";
    return 0;
}
