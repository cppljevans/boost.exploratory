//Purpose:
//  Explore if code here:
//    https://stackoverflow.com/questions/73673899/boost-spirit-x3-unable-to-create-an-ast-that-has-a-optional-list
//  is caused by same problem as that mentioned in issue707.
//  It may be because 
//===========================
#include <vector>
#include <optional>

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace x3=boost::spirit::x3;

namespace ast
{
  struct doubleOrString : x3::variant<double,std::string>{
     using base_type::base_type;
     using base_type::operator=;
  };
 
  struct pass {
     std::vector<doubleOrString> dOrs;
  };
}//ast namespace

BOOST_FUSION_ADAPT_STRUCT(ast::pass,dOrs)

x3::rule<class doubleOrString_class, ast::doubleOrString> const doubleOrString = "doubleOrString";
x3::rule<class pass_class, ast::pass> const pass = "pass";
x3::rule<class es_class, std::optional<ast::pass>> const es = "es";

auto const doubleOrString_def =x3::double_ | x3::string("AString");
auto const pass_def = doubleOrString % x3::lit(",");
auto const es_def = (pass > x3::lit(";")) | x3::lit(";");

BOOST_SPIRIT_DEFINE(doubleOrString,pass,es);
