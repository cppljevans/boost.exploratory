#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
#include <boost/utility/trace_scope.hpp>
#include <boost/utility/demangle.hpp>
#define BOOST_SPIRIT_X3_DEBUG_OUT std::cout
static boost::iostreams::indent_scoped_ostreambuf ind_ostream(BOOST_SPIRIT_X3_DEBUG_OUT);
#define BOOST_SPIRIT_X3_DEBUG // Provide some meaningful reassuring output.

#include <boost/spirit/home/x3.hpp>

namespace x3 = boost::spirit::x3;
using x3::unused_type;
using x3::unused;
using x3::attr;
using x3::space;

int main()
{
  #if 1
    { 
        auto constexpr parser_unused=attr(unused);
        boost::trace_scope a_trace_scope("parser_unused");
        char const * iter= "123 456", * const end= iter + std::strlen(iter);
        bool result=x3::phrase_parse(iter, end, parser_unused, space, unused);
        std::cout<<":parse_result="<<result<<":iter="<<iter<<";\n";
    }
  #endif
    return 0;
}
