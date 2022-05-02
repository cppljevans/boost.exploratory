//OriginalSource:
//  https://stackoverflow.com/questions/67628514/boostspiritx3-problem-with-any-ast-class-representing-a-stdstring
//Modification:
//  Made each rule's attribute == attribute_of(rule's rhs).
//===============
//Purpose: 
//  Demonstrate problem in above https stackoverflow.com reference. 
//===============
//
#include <iostream>
#define USE_INDENT_OSTREAM
#ifdef USE_INDENT_OSTREAM
  #include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
  boost::iostreams::indent_scoped_ostreambuf ind_ostream(std::cout);
#endif//USE_INDENT_OSTREAM
#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/utility/trace_scope.hpp>
#include <boost/utility/stringify.hpp>
#endif
#include <boost/utility/demangle_type.hpp>

//#define BOOST_SPIRIT_X3_DEBUG // Provide some meaningful reassuring output.
#ifdef BOOST_SPIRIT_X3_DEBUG
  #define BOOST_SPIRIT_X3_DEBUG_OUT std::cout
#endif
#include <boost/utility/demangle_type.hpp>
#if 0
  #include <variant>
  #define VARIANT_NS std
#elif 0
  #define VARIANT_NS boost
#else
#include <boost/spirit/home/x3/support/ast/variant.hpp>
  #define VARIANT_NS x3
#endif
enum rule_enum
{ white_id
, braced_id
, gap_id
, start_id
};

template<rule_enum RuleEnum>
struct rule_id;

#include <boost/spirit/home/x3.hpp>
//#define USE_TRANSFORM_ATTRIBUTE_SPECIALIZED
#ifdef USE_TRANSFORM_ATTRIBUTE_SPECIALIZED
#include <iostream>
#include <vector>  
namespace boost { namespace spirit { namespace x3 { 
      template
      < typename Exposed
      , typename Transformed
      >
    struct default_transform_attribute
      < std::vector<Exposed>
      , std::vector<Transformed>
      >
    {
            typedef 
          std::vector
          < Transformed
          > //Transformed
        type;
            typedef
          std::vector
          < Exposed
          >
        exposed;

        static type pre(exposed&) { return type(); }

        static void post(exposed& val, type&& attribute)
        {
          std::cout<<__FILE__<<':'<<__LINE__<<':'<<__func__<<";\n";
        #define USE_POST_MOVE_TO
        #ifdef USE_POST_MOVE_TO
          traits::move_to(std::forward<type>(attribute), val);
          //^same as in spirit, but for this specialization.
          //This also causes the same comile-time error as in spirit version.
        #else
          std::cout<<"demangle<type>=="<<demangle_type<type>()<<";\n";
          std::cout<<"demangle<exposed>=="<<demangle_type<exposed>()<<";\n";
          using exposed_value = typename exposed::value_type;
          std::cout<<"demangle<exposed::value_type>=="<<demangle_type<exposed_value>()<<";\n";
          val.push_back(std::forward<type>(attribute));
          //This compiles without error.
        #endif
        }
    };
      template 
      < typename Exposed
      >
    struct default_transform_attribute
      < std::vector<Exposed>
      , std::vector<Exposed>
      >
    {
            typedef 
          std::vector
          < Exposed
          > //Transformed
        type;
            typedef
          std::vector
          < Exposed
          >
        exposed;

        static type pre(exposed& val) { return val; }

        static void post(exposed& val, type&& attribute)
        {
          std::cout<<__FILE__<<':'<<__LINE__<<':'<<__func__<<";\n";
        }
    };
}}}
#endif//USE_TRANSFORM_ATTRIBUTE_SPECIALIZED

namespace boost { namespace spirit { namespace x3 { 
#define ATTRIBUTE_OF_RAW
#ifdef ATTRIBUTE_OF_RAW
namespace traits
{
    template <typename Subject, typename Context, typename Enable>
    struct attribute_of<raw_directive<Subject>, Context, Enable>
    {
        using type=std::string;//workaround.  Otherwise, compile error.
    };
}
}}}
#endif// ATTRIBUTE_OF_RAW
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <string>
namespace x3        = boost::spirit::x3;

  using x3::char_;
  using x3::space;
  using x3::raw;
  template<typename Parser>
  using attribute_of=typename x3::traits::attribute_of<Parser,x3::unused_type>::type;
  
  auto braced_def = char_('{') >> *~char_('}') >> char_('}'); // { ... }
  using braced_rhs=decltype(braced_def);
  using braced_aof=attribute_of<braced_rhs>;
  struct braced_usr
    { char open;
      std::string text;
      char close;
    };
BOOST_FUSION_ADAPT_STRUCT(braced_usr, open, text, close)
  using braced_attr=
  #define BRACED_AOF
  #ifdef BRACED_AOF
    braced_aof
  #else
    braced_usr
  #endif
    ;
  x3::rule< rule_id<braced_id>, braced_attr > const braced  = "braced";
   
//#define WITHOUT_WHITE
#ifndef WITHOUT_WHITE
  auto white_def = raw[+space];
  using white_rhs=decltype(white_def);
  using white_aof=attribute_of<white_rhs>;
  using white_usr=std::string;
  using white_attr=
  #define WHITE_AOF
  #ifdef WHITE_AOF
    white_aof
  #else
    white_usr
  #endif
    ;
  x3::rule< rule_id<white_id>, white_attr> const white = "white";
  BOOST_SPIRIT_DEFINE(white);
#endif //WITHOUT_WHITE
  
  #ifndef WITHOUT_WHITE
    auto gap_def = +(white | braced); // spaces etc
  #else // WITHOUT_WHITE
    auto gap_def = +(space | braced); // spaces etc
  #endif // WITHOUT_WHITE
  using gap_rhs=decltype(gap_def);
  using gap_aof=attribute_of<gap_rhs>;
  #ifndef WITHOUT_WHITE
    using gap_item=VARIANT_NS::variant<white_attr,braced_attr>;
  #else
    using gap_item=VARIANT_NS::variant<char,braced_attr>;
  #endif
  using gap_usr=std::vector<gap_item>;
  
  using gap_attr=
  //#define GAP_AOF
  #ifdef GAP_AOF
    gap_aof
  #else
    gap_usr
  #endif
    ;
  x3::rule< rule_id<gap_id>, gap_attr> const gap = "gap";
//#define USE_START_ACTION
#ifdef USE_START_ACTION
  auto start_action = 
    [](auto& ctx)
    { std::cout<<"START_ACTION:"<<__FILE__<<':'<<__LINE__<<"\n";
    ; std::cout<<":demangle(_val)="<<demangle_type(x3::_val(ctx))<<"\n";
    ; std::cout<<":demangle(_attr)="<<demangle_type(x3::_attr(ctx))<<"\n";
    ; x3::_pass(ctx) = true;
    ;};
  #define START_ACTION [start_action]
#else
  #define START_ACTION
#endif//USE_START_ACTION
  auto start_vec = 
    + ( gap //gap attribute
      | (char_ -'{' -space) //std::string attribute
      )
    ;
  auto start_def = start_vec START_ACTION;
  using start_rhs=decltype(start_def);
  using start_aof=attribute_of<start_rhs>;
  using start_item = VARIANT_NS::variant<gap_attr,std::string>;
  using start_usr = std::vector<start_item>;
  using start_attr=
  //#define START_AOF
  #ifdef START_AOF
    start_aof
  #else
    start_usr
  #endif
    ;
  x3::rule< rule_id<start_id>, start_attr> const start="start";
  
  BOOST_SPIRIT_DEFINE(gap, braced, start);
  
#ifdef USE_TRANSFORM_ATTRIBUTE_SPECIALIZED
  #pragma message("yes USE_TRANSFORM_ATTRIBUTE_SPECIALIZED")
#else  
  #pragma message("not USE_TRANSFORM_ATTRIBUTE_SPECIALIZED")
#endif//USE_TRANSFORM_ATTRIBUTE_SPECIALIZED
#ifdef WITHOUT_WHITE
  #pragma message("yes WITHOUT_WHITE")
#else  
  #pragma message("not WITHOUT_WHITE")
#endif//WITHOUT_WHITE
#ifdef START_AOF
  #pragma message("yes START_AOF")
#else  
  #pragma message("not START_AOF")
#endif//START_AOF
#ifdef USE_ATTRIBUTE_OF_NO_COLLAPSE
  #pragma message("yes USE_ATTRIBUTE_OF_NO_COLLAPSE")
#else  
  #pragma message("not USE_ATTRIBUTE_OF_NO_COLLAPSE")
#endif//USE_ATTRIBUTE_OF_NO_COLLAPSE
int
main()
{
#ifdef USE_INDENT_OSTREAM
  boost::iostreams::indent_scoped_ostreambuf ind_ostream(std::cout);
#endif  
#if 0 && !defined(WITHOUT_WHITE)
  {
    std::cout<<"***"<<__LINE__<<";\n";
    char const* iter = "? {;};", * const end = iter + std::strlen(iter);
    white_attr ast;
    auto result=parse(iter, end, white_def, ast) || iter!=end;
    std::cout<<"result="<<result<<";\n";
  }
#endif
#if 0
  {
    std::cout<<"***"<<__LINE__<<";\n";
    char const* iter = "? {;};", * const end = iter + std::strlen(iter);
    braced_attr ast;
    auto result=parse(iter, end, braced_def, ast) || iter!=end;
    std::cout<<"result="<<result<<";\n";
  }
#endif
#if 0
  {
    std::cout<<"***"<<__LINE__<<";\n";
    std::cout<<"gap_attr="<<demangle_type<gap_attr>()<<";\n";
  }
#endif
#if 0
  //This compiles
  {
    std::cout<<"***"<<__LINE__<<";\n";
    char const* iter = "? {;};", * const end = iter + std::strlen(iter);
    gap_attr ast;
    auto result=parse(iter, end, gap_def, ast) || iter!=end;
    std::cout<<"result="<<result<<";\n";
  }
#endif
#if 1
  //this does NOT compile when:
  // !defined(WITHOUT_WHITE)
  // !defined(USE_TRANSFORM_ATTRIBUTE_SPECIALIZED)
  //  defined(START_AOF)
  //yet attribute_of<start_def>::type == start_attr; so, it should compile.
  {
    std::cout<<"***"<<__LINE__<<";\n";
    char const* iter = "? {;};", * const end = iter + std::strlen(iter);
    start_attr ast;
    std::cout<<":demangle(start_attr)="<<demangle_type(ast)<<";\n";
    auto result=parse(iter, end, start_def, ast) || iter!=end;
    std::cout<<"result="<<result<<";\n";
  }
#endif
#if 0
  {
    std::cout<<"***"<<__LINE__<<";\n";
    char const* iter = "? {;};", * const end = iter + std::strlen(iter);
    std::cout<<"start_attr="<<demangle_type<start_attr>()<<";\n";
    start_attr ast;
    auto result=parse(iter, end, start, ast) || iter!=end;
    std::cout<<"result="<<result<<";\n";
  }
#endif  
  return 0;
}

