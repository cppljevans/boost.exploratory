//OriginalSource:
//  https://stackoverflow.com/questions/67628514/boostspiritx3-problem-with-any-ast-class-representing-a-stdstring
//Modification:
//  Adde macros governing whether start's attribute is as originally specified or
//  as a result of attribute_of.
//Result:
//  If attribute_of used, it compiles; otherwise, it doesn't.
//===================
//
#include <boost/core/demangle.hpp>
template<typename Type>
std::string demangle_type()
{
  std::string mangled=typeid(Type).name();
  std::string dem=boost::core::demangle(mangled.c_str());
  return dem;
}

#define BOOST_SPIRIT_X3_DEBUG // Provide some meaningful reassuring output.
#define BOOST_SPIRIT_X3_DEBUG_OUT std::cout

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3        = boost::spirit::x3;

  using x3::char_;
  using x3::space;
  using x3::raw;
  template<typename Parser>
  using attribute_of=typename x3::traits::attribute_of<Parser,x3::unused_type>::type;
  
#define WITH_WHITE
#ifdef WITH_WHITE
  auto white_def = raw[+space];
  using white_attr=std::string;
  x3::rule< struct white, white_attr> const white = "white";
#endif//WITH_WHITE  
  auto braced_def = char_('{') >> *~char_('}') >> char_('}'); // { ... }
  struct braced_attr
    { char open;
      std::string text;
      char close;
    };
BOOST_FUSION_ADAPT_STRUCT(braced_attr, open, text, close)
  x3::rule< struct braced, braced_attr > const braced  = "braced";
   
  #ifdef WITH_WHITE
    auto gap_def = +(white | braced); // spaces etc
    using gap_item=boost::variant<white_attr,braced_attr>;
  #else // WITH_WHITE
    auto gap_def = +(space | braced); // spaces etc
    using gap_item=boost::variant<char,braced_attr>;
  #endif // WITH_WHITE
  using gap_attr=std::vector<gap_item>;
  x3::rule< struct gap, gap_attr> const gap = "gap";
  
  auto start_def = 
    +( gap //gap attribute
     | (char_ -'{' -space) //std::string attribute
     );
  using start_rhs=decltype(start_def);
  using start_aof=attribute_of<start_rhs>;
  using start_item = boost::variant<gap_attr,std::string>;
  using start_dcl = std::vector<start_item>;
  using start_attr=
  #define START_AOF
  #ifdef START_AOF
    //compiles OK and runs to completion
    start_aof
  #else
    //fails to compile.
    start_dcl
    //using this causes compile-time errors.
  #endif
    ;
  x3::rule< struct start, start_attr> const start = "start";

#ifdef WITH_WHITE
  BOOST_SPIRIT_DEFINE(white);
#endif//WITH_WHITE    
  BOOST_SPIRIT_DEFINE(gap, braced, start);
  
int
main()
{
#ifdef WITH_WHITE
  std::cout<<"yes WITH_WHITE;\n";
#else  
  std::cout<<"not WITH_WHITE;\n";
#endif//WITH_WHITE
  {
    std::cout<<"***"<<__LINE__<<";\n";
    char const* iter = "? {;};", * const end = iter + std::strlen(iter);
    std::cout<<"start_dcl="<<demangle_type<start_dcl>()<<";\n";
    std::cout<<"start_aof="<<demangle_type<start_aof>()<<";\n";
    std::cout<<"***parse;\n";
    start_aof ast;
    auto result=parse(iter, end, start, ast) || iter!=end;
    std::cout<<"result="<<result<<";\n";
  }
  return 0;
}

