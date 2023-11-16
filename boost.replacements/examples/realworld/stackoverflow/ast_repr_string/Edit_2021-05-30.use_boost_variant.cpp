//OriginalSource:
//  https://stackoverflow.com/questions/67628514/boostspiritx3-problem-with-any-ast-class-representing-a-stdstring
//===============
// Purpose: Demonstrate problem with lower level detail (gap_item) where wrapped rule (gap) should do.
//#define BOOST_SPIRIT_X3_DEBUG       // Provide some meaningful reassuring output.
//#define WITHOUT_WHITE         // All ok until white added.
//#define WITH_GAP_ITEM_IN_VARIANT    // Unnacceptable workaround
#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
#include <boost/utility/trace_scope.hpp>
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>

#include <string>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3        = boost::spirit::x3;

namespace minimal { namespace ast
{
  struct white      : std::string           {using std::string::string;};
  struct braced                     {char open; std::string text; char close;};
  #ifdef WITHOUT_WHITE
  using gap_item = boost::variant<char,braced>;
  #else // WITHOUT_WHITE
  using gap_item = boost::variant<white,braced>;
  #endif // WITHOUT_WHITE
  struct gap        : std::vector<gap_item>     {};

  #ifndef WITH_GAP_ITEM_IN_VARIANT
  using start_item = boost::variant<gap,std::string>;
  #else // WITH_GAP_ITEM_IN_VARIANT
  using start_item = boost::variant<gap,std::string,gap_item,char>;
  #endif // WITH_GAP_ITEM_IN_VARIANT
  struct start  : std::vector<start_item>       {using std::vector<start_item>::vector;};
}}
BOOST_FUSION_ADAPT_STRUCT(minimal::ast::braced, open, text, close)

namespace minimal { namespace parser
{
  using x3::char_;
  using x3::space;
  using x3::raw;
  
  x3::rule<struct white,    ast::white> const   white       = "white";
  x3::rule<struct braced,   ast::braced>    const   braced      = "braced";
  x3::rule<struct gap,      ast::gap>   const   gap     = "gap";
  x3::rule<struct start,    ast::start> const   start       = "start";
  
  auto const white_def      = raw[+space];
  auto const braced_def     = char_('{') >> *(char_ -'}') >> char_('}');    // { ... }
  #ifdef WITHOUT_WHITE
  auto const gap_def        = +(space | braced);                // spaces etc
  #else // WITHOUT_WHITE
  auto const gap_def        = +(white | braced);                // spaces etc
  #endif // WITHOUT_WHITE
  auto const start_def      = +(gap | +(char_ -'{' -space));        // gap=container or string
  
  BOOST_SPIRIT_DEFINE(white, gap, braced, start);
}}

int
main()
{
    boost::iostreams::indent_scoped_ostreambuf<char>
  indent_outbuf(std::cout,2);
  std::cout<<std::boolalpha;
  char const*           iter    = "? {;};", * const end = iter + std::strlen(iter);
  minimal::ast::start   ast;
  auto result=parse(iter, end, minimal::parser::start, ast) || iter!=end;
  std::cout<<"result="<<result<<";\n";
  return 0;
}
//}}}
