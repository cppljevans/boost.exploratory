//OriginalSource:
//  https://stackoverflow.com/questions/74231690/functionality-of-x3raw
//===============
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#include <boost/utility/trace_scope.hpp>
#include <boost/spirit/home/x3.hpp>
namespace x3 = boost::spirit::x3;

const auto rule= +x3::char_('x');
const auto rule_raw= x3::raw[ rule ];

std::string in = "xxxx   "; // trailing spaces
std::string result;

#include <iostream>
int main()
{
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
    bool r=false;
  #if 0
    r= x3::phrase_parse( in.begin(), in.end(), rule, x3::ascii::space, result );
    std::cout << "rule: " << r << "  result:'" << result << "'\n";
  #endif
    result= "";
    r= x3::phrase_parse( in.begin(), in.end(), rule_raw, x3::ascii::space, result );
    std::cout << "rule_raw: " << r << "  result:'" << result << "'\n";
    return 0;
}  
