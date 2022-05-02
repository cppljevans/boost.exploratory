#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/utility/trace_scope.hpp>
#endif
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>
#define USE_DEQUE_TIE
#ifdef USE_DEQUE_TIE
#pragma message("using deque_tie")
#include <boost/fusion/container/generation/deque_tie.hpp>
#endif//USE_DEQUE_TIE
namespace x3 = boost::spirit::x3;
using x3::unused_type;
using x3::unused;
using x3::lit;
using x3::space;
using x3::int_;
using x3::rule;
using x3::traits::attribute_of;
using x3::traits::print_attribute;

auto seq_rule_def
  = lit('a')//unused_type attribute
  >>int_    //int attribute   
  >>lit('b')//unused_type attribute
  >>int_    //int attribute
  ;
using rhs_gram_t=decltype(seq_rule_def);
using rhs_attr_t=attribute_of<rhs_gram_t,unused_type>::type;
using seq_attr_t=
#define RULE_ATTR_IS_RHS_ATTR
#ifdef RULE_ATTR_IS_RHS_ATTR
  rhs_attr_t
#else
  boost::fusion::deque<int,int>
#endif
  ;
rule<struct seq_rule,seq_attr_t> seq_rule="seq_rule";
BOOST_SPIRIT_DEFINE(seq_rule);
auto seq_assign=seq_rule=seq_rule_def;
//#define USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION
#ifdef USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION
namespace boost { namespace spirit { namespace x3 { namespace traits
{
  #ifndef RULE_ATTR_IS_RHS_ATTR
  #ifdef USE_DEQUE_TIE
    unused_type unused_mutable;
  #endif
    template <>
    struct transform_attribute
      < seq_attr_t //Exposed
      , rhs_attr_t //Transformed
      , x3::parser_id
      >
    {
        using Exposed=seq_attr_t;
      #ifdef USE_DEQUE_TIE
        using Transformed=fusion::deque<unused_type&,int&,unused_type&,int&>;
      #else
        using Transformed=rhs_attr_t;
      #endif//USE_DEQUE_TIE
        typedef Transformed type;

        static Transformed pre(Exposed&exposed) 
        { 
        #ifdef USE_DEQUE_TIE
          int& x0=fusion::at_c<0>(exposed);
          int& x1=fusion::at_c<1>(exposed);
          return fusion::deque_tie(unused_mutable,x0,unused_mutable,x1);
        #else
          return Transformed();
        #endif//USE_DEQUE_TIE
        }

        static void post(Exposed& val, Transformed&& attribute)
        {
            trace_scope ts(__func__);
            std::cout<<"Exposed="<<demangle_type<Exposed>()<<";\n";
            std::cout<<"Transformed="<<demangle_type<Transformed>()<<";\n";
        #ifdef USE_DEQUE_TIE
        #else
            Transformed transformed=std::forward<Transformed>(attribute);
            fusion::at_c<0>(val)=fusion::at_c<1>(transformed);
            fusion::at_c<1>(val)=fusion::at_c<3>(transformed);
        #endif//USE_DEQUE_TIE
        }
    };
  #endif//RULE_ATTR_IS_RHS_ATTR
}}}}
#endif//USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION
char const * input="a 123 b 456";
char const * end= input + std::strlen(input);
template<typename Ast, typename Gram>
void test
( std::string label
, Gram gram
)
{
  std::cout<<label<<";\n";
  char const * iter=input;
  Ast ast;
  std::cout<<":demangle_type(ast)="<<demangle_type(ast)<<";\n";
  bool result=x3::phrase_parse(iter, end, gram, space, ast);
  std::cout<<":result="<<result<<":at_end="<<(iter==end)<<";\n";
  std::cout<<":ast=";print_attribute(std::cout,ast);std::cout<<";\n";
}
int main()
{
    test<seq_attr_t>("***seq_rule***",seq_rule);
    //test<rhs_attr_t>("***seq_rule_def***",seq_rule_def);
    //test<seq_attr_t>("***seq_assign:seq_attr_t***",seq_assign);
    //test<rhs_attr_t>("***seq_assign:rhs_attr_t***",seq_assign);
    return 0;
}
