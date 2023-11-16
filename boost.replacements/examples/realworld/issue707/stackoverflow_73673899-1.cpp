#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <iostream>
#define USE_STD_OPTIONAL
#ifdef USE_STD_OPTIONAL
  #include <optional>
  #define OPTIONAL_NS std
namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename T>
    struct is_optional<std::optional<T>>
      : mpl::true_
    {};
}}}}
#else  
  #include <boost/optional.hpp>
  #define OPTIONAL_NS boost
#endif//USE_STD_OPTIONAL
#include <string>
#include <vector>

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
x3::rule<class es_class, OPTIONAL_NS::optional<ast::pass>> const es = "es";

auto const doubleOrString_def =x3::double_ | x3::string("AString");
auto const pass_def = doubleOrString % x3::lit(",");
auto const es_def = (pass > x3::lit(";")) | x3::lit(";");

BOOST_SPIRIT_DEFINE(doubleOrString,pass,es);

int main()
{
    using boost::spirit::x3::ascii::space;
    typedef std::string::const_iterator iterator_type;

    std::string str;
    while (getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        OPTIONAL_NS::optional<ast::pass> esdOrs;
        iterator_type iter = str.begin();
        iterator_type const end = str.end();
        bool r = phrase_parse(iter, end, es, space, esdOrs);
        

        if (r && iter == end)
        {
            std::cout << "\nGood!\n";
        }
        else
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}
