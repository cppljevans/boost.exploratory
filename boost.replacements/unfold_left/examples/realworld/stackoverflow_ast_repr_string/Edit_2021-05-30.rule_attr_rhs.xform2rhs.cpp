//OriginalSource:
//  https://stackoverflow.com/questions/67628514/boostspiritx3-problem-with-any-ast-class-representing-a-stdstring
//===============
//Purpose: 
//  Demonstrate problem in above https stackoverflow.com reference. 
//===============
//
#define BOOST_SPIRIT_X3_DEBUG // Provide some meaningful reassuring output.
#ifdef BOOST_SPIRIT_X3_DEBUG
  #define BOOST_SPIRIT_X3_DEBUG_OUT std::cout
#endif
#define USE_INDENT_OSTREAM
#ifdef USE_INDENT_OSTREAM
  #define USE_TRACE_SCOPE
  #include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
  #include <boost/utility/trace_scope.hpp>
#endif//USE_INDENT_OSTREAM
#ifdef USE_UNFOLD_LEFT
#include <boost/spirit/home/x3/support/ast/std_variant.hpp>
#define USED_VARIANT boost::spirit::x3::std_variant
#else
#define USED_VARIANT boost::variant
#endif//USE_UNFOLD_LEFT
//#define USE_MOVE_TO_CHAR_STRING
#ifdef USE_MOVE_TO_CHAR_STRING
namespace boost { namespace spirit { namespace x3 { 
namespace traits
{
    inline void move_to(char src, std::string& dest)
    {
        dest+=src;
    }
}
}}}
#endif//USE_MOVE_TO_CHAR_STRING
//#define USE_DEMANGLE_FMT
#ifdef USE_DEMANGLE_FMT
  #include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#else
  #include <boost/utility/demangle_type.hpp>
#endif//USE_DEMANGLE_FMT
#include <iostream>
#include <vector>
  template<typename T>
  std::ostream&
operator<<
  ( std::ostream&sout
  , std::vector<T>const& x
  )
  {
     sout<<"std::vector{ ";
     for(unsigned i=0; i<x.size(); ++i)
     { if(i>0) sout<<", "
     ;  sout<<x[i]
     ;}
     return sout<<'}';
  }
enum rule_enum
{ white_id
, braced_id
, gap_id
, start_id
};

template<rule_enum RuleEnum>
struct rule_id;

//#define TRANSFORM_ATTRIBUTE_RULE2RHS
#include <boost/spirit/home/x3.hpp>
#define ATTRIBUTE_OF_RAW
#ifdef ATTRIBUTE_OF_RAW
namespace boost { namespace spirit { namespace x3 { 
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
namespace x3        = boost::spirit::x3;

  using x3::char_;
  using x3::lit;
  using x3::space;
  using x3::raw;
  template<typename Parser>
  using attribute_of=typename x3::traits::attribute_of<Parser,x3::unused_type>::type;
  
  auto white_def = raw[+space];
//#define WITHOUT_WHITE
#ifndef WITHOUT_WHITE
  using white_rhs=decltype(white_def);
  using white_aof=attribute_of<white_rhs>;
  using white_usr=std::string;
  using white_attr=
  //#define WHITE_AOF
  #ifdef WHITE_AOF
    white_aof
  #else
    white_usr
  #endif
    ;
  x3::rule< rule_id<white_id>, white_attr> const white = "white";
  BOOST_SPIRIT_DEFINE(white);
#endif //WITHOUT_WHITE

  auto braced_def = 
       char_('{') //attribute: char
    >> *~char_('}') //attribute: std::string
    >> char_('}') //attribute: char
    ;
  using braced_rhs=decltype(braced_def);
  using braced_aof=attribute_of<braced_rhs>;
  struct braced_usr
    { char open;
      std::string text;
      char close;
    }
    ;
BOOST_FUSION_ADAPT_STRUCT(braced_usr, open, text, close)
  using braced_attr=
  //#define BRACED_AOF
  #ifdef BRACED_AOF
    braced_aof
  #else
    braced_usr
  #endif
    ;
  x3::rule< rule_id<braced_id>, braced_attr > const braced  = "braced";
   
  auto gap_item =
  #ifndef WITHOUT_WHITE
      white 
  #else
      white_def
  #endif // WITHOUT_WHITE
    | braced
    ;
  auto gap_def = +gap_item;
  using gap_rhs=decltype(gap_def);
  using gap_aof=attribute_of<gap_rhs>;
  using gap_attr=
    gap_aof
    ;
  x3::rule< rule_id<gap_id>, gap_attr> const gap = "gap";
  auto start_def = 
    + ( gap //gap attribute
      | +(char_ -'{' -space) //std::string attribute
      )
    ;
  using start_rhs=decltype(start_def);
  using start_aof=attribute_of<start_rhs>;
  using start_item = USED_VARIANT<gap_attr,std::string>;
  using start_usr = std::vector<start_item>;
  //static_assert(std::is_same_v<start_aof,start_usr>);
  using start_attr=
  //#define START_AOF
  #ifdef START_AOF
    start_aof
  #else
    start_usr
  #endif
    ;
  x3::rule< rule_id<start_id>, start_attr> const start="start";
  
//#define USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION
#ifdef USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION
namespace boost { namespace spirit { namespace x3 { namespace traits
{
#ifndef WITHOUT_WHITE
    template <>
    struct transform_attribute
      < white_attr //Exposed
      , white_aof //Transformed
      , x3::parser_id
      >
    {
        using Exposed=white_attr;
        using Transformed=white_aof;
        typedef Transformed type;

        static Transformed pre(Exposed&) 
        { 
            return Transformed(); 
        }

        static void post(Exposed& val, Transformed&& attribute)
        {
          #ifdef TRANSFORM_TRACE
            std::cout<<__FILE__<<':'<<__LINE__<<":white::"<<__func__<<":val="<<val<<":attribute="<<attribute<<";\n";
          #endif
        }
    };
#endif//WITHOUT_WHITE
    template <>
    struct transform_attribute
      < braced_attr //Exposed
      , braced_aof //Transformed
      , x3::parser_id
      >
    {
        using Exposed=braced_attr;
        using Transformed=braced_aof;
        typedef Transformed type;

        static Transformed pre(Exposed&) 
        { 
            return Transformed(); 
        }

        static void post(Exposed& val, Transformed&& attribute)
        {
          #ifdef TRANSFORM_TRACE
            std::cout<<__FILE__<<':'<<__LINE__<<":braced::"<<__func__<<":val=";
            print_attribute(std::cout,val);
            std::cout<<":attribute=";
            print_attribute(std::cout,attribute);
            std::cout<<":demangle_type(val)="<<demangle_type(val)<<":demangle_type(attribute)="<<demangle_type(attribute)<<";\n";
          #endif
          traits::move_to(std::forward<Transformed>(attribute), val);
        }
    };
    template <>
    struct transform_attribute
      < gap_attr //Exposed
      , gap_aof //Transformed
      , x3::parser_id
      >
    {
        using Exposed=gap_attr;
        using Transformed=gap_aof;
        typedef Transformed type;

        static Transformed pre(Exposed&) 
        { 
            return Transformed(); 
        }

        static void post(Exposed& val, Transformed&& attribute)
        {
          #ifdef TRANSFORM_TRACE
            std::cout<<__FILE__<<':'<<__LINE__<<":gap::"<<__func__<<";\n";
          #endif
        }
    };
  #ifdef START_TRANSFORM_ATTRIBUTE  
    template <>
    struct transform_attribute
      < start_usr //Exposed
      , start_aof //Transformed
      , x3::parser_id
      >
    {
      #ifdef START_AOF
        using Exposed=start_aof;
      #else
        using Exposed=start_usr;
      #endif
        using Transformed=start_aof;
        typedef Transformed type;

        static Transformed pre(Exposed&) 
        { 
            return Transformed(); 
        }

        static void post(Exposed& val, Transformed&& attribute)
        {
          #ifdef TRANSFORM_TRACE
            std::cout<<__FILE__<<':'<<__LINE__<<":start::"<<__func__<<";\n";
            std::cout<<"Exposed=\n";
            std::cout<<demangle_fmt_type<Exposed>();
            std::cout<<";\n";
            std::cout<<"Transformed=\n";
            std::cout<<demangle_fmt_type<Transformed>();
            std::cout<<";\n";
            bool exposed_transformed_same=std::is_same_v<Exposed,Transformed>;
            std::cout<<"exposed_transformed_same="<<exposed_transformed_same<<";\n";
          #endif
            traits::move_to(std::forward<Transformed>(attribute), val);
        }
    };
  #endif//START_TRANSFORM_ATTRIBUTE  
}}}}
#endif//USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION
  BOOST_SPIRIT_DEFINE(gap, braced, start);
#define STR(X) #X
#define DEFER(M,...) M(__VA_ARGS__)
#define CUSTOM_ERROR(X) _Pragma(STR(message(STR(X))))
//CUSTOM_ERROR(USED_VARIANT)
#ifdef USE_UNFOLD_LEFT
  #pragma message("yes USE_UNFOLD_LEFT")
#else  
  #pragma message("not USE_UNFOLD_LEFT")
#endif//USE_UNFOLD_LEFT
/*
#ifdef USE_ATTRIBUTE_OF_NO_COLLAPSE
  #pragma message("yes USE_ATTRIBUTE_OF_NO_COLLAPSE")
#else  
  #pragma message("not USE_ATTRIBUTE_OF_NO_COLLAPSE")
#endif//USE_ATTRIBUTE_OF_NO_COLLAPSE
*/
#ifdef TRANSFORM_ATTRIBUTE_RULE2RHS
  #pragma message("yes TRANSFORM_ATTRIBUTE_RULE2RHS")
#else  
  #pragma message("not TRANSFORM_ATTRIBUTE_RULE2RHS")
#endif//TRANSFORM_ATTRIBUTE_RULE2RHS
#ifdef TRANSFORM_TRACE
  #pragma message("yes TRANSFORM_TRACE")
#else  
  #pragma message("not TRANSFORM_TRACE")
#endif//TRANSFORM_TRACE
#ifdef WITHOUT_WHITE
  #pragma message("yes WITHOUT_WHITE")
#else  
  #pragma message("not WITHOUT_WHITE")
  #ifdef WHITE_AOF
    #pragma message("yes WHITE_AOF")
  #else  
    #pragma message("not WHITE_AOF")
  #endif//WHITE_AOF
#endif//WITHOUT_WHITE
#ifdef BRACED_AOF
  #pragma message("yes BRACED_AOF")
#else  
  #pragma message("not BRACED_AOF")
#endif//BRACED_AOF
#ifdef START_AOF
  #pragma message("yes START_AOF")
#else  
  #pragma message("not START_AOF")
#endif//START_AOF
#ifdef USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION
  #pragma message("yes USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION")
#else  
  #pragma message("not USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION")
#endif//USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION
int
main()
{
  std::cout<<std::boolalpha;
#ifdef USE_INDENT_OSTREAM
  boost::iostreams::indent_scoped_ostreambuf ind_ostream(std::cout);
#endif  
#if 0 && !defined(WITHOUT_WHITE)
  {
    std::cout<<"***ON LINE:"<<__LINE__<<";\n";
    char const* iter = "   ", * const end = iter + std::strlen(iter);
    white_attr ast;
    auto result=parse(iter, end, white_def, ast) || iter!=end;
    std::cout<<"result="<<result<<";\n";
  }
#endif
#if 0 && !defined(WITHOUT_WHITE)
  {
    std::cout<<"***ON LINE:"<<__LINE__<<";\n";
    char const* iter = "   ", * const end = iter + std::strlen(iter);
    white_attr ast;
    auto result=parse(iter, end, white, ast) || iter!=end;
    std::cout<<"result="<<result<<";\n";
  }
#endif
#if 0
  {
    std::cout<<"***ON LINE:"<<__LINE__<<";\n";
    char const* iter = "? {;};", * const end = iter + std::strlen(iter);
    braced_attr ast;
    auto result=parse(iter, end, braced_def, ast) || iter!=end;
    std::cout<<"result="<<result<<";\n";
  }
#endif
#if 0
  {
    std::cout<<"***ON LINE:"<<__LINE__<<";\n";
    std::cout<<"gap_attr="<<demangle_type<gap_attr>()<<";\n";
  }
#endif
#if 0
  //This compiles
  {
    std::cout<<"***ON LINE:"<<__LINE__<<";\n";
    char const* iter = "{;}", * const end = iter + std::strlen(iter);
    gap_attr ast;
    auto result=parse(iter, end, gap_def, ast) || iter!=end;
    std::cout<<"result="<<result<<";\n";
  }
#endif
#if 0
  {
    std::cout<<"***ON LINE:"<<__LINE__<<";\n";
    char const* iter = "   {abc,def hij}  ", * const end = iter + std::strlen(iter);
    gap_attr ast;
    auto result=parse(iter, end, gap, ast) || iter!=end;
    std::cout<<"result="<<result<<";\n";
  }
#endif
#if 0
  //this does NOT compile when:
  // !defined(WITHOUT_WHITE)
  // !defined(USE_TRANSFORM_ATTRIBUTE_SPECIALIZED)
  //  defined(START_AOF)
  //yet attribute_of<start_def>::type == start_attr; so, it should compile.
  {
    std::cout<<"***ON LINE:"<<__LINE__<<";\n";
    char const* iter = "?   {;};", * const end = iter + std::strlen(iter);
    start_attr ast;
    auto result=parse(iter, end, start_def, ast) || iter!=end;
    std::cout<<__func__<<':'<<__LINE__<<":result="<<result<<";\n";
    std::cout<<"print_attribute(ast)=\n";
    x3::traits::print_attribute(std::cout,ast);
  #ifdef USE_DEMANGLE_FMT
    std::cout<<":demangle_fmt(start_attr)="<<demangle_fmt_type(ast)<<";\n";
  #else
    std::cout<<":demangle(start_attr)="<<demangle_type(ast)<<";\n";
  #endif//USE_DEMANGLE_FMT
  }
#endif
#if 1
  {
    std::cout<<"***ON LINE:"<<__LINE__<<";\n";
    char const* iter = "?   {;};", * const end = iter + std::strlen(iter);
    start_attr ast;
    auto result=parse(iter, end, start, ast) || iter!=end;
    std::cout<<__func__<<':'<<__LINE__<<":result="<<result<<";\n";
    std::cout<<"print_attribute(ast)=\n";
    x3::traits::print_attribute(std::cout,ast);std::cout<<";\n";
  #ifdef USE_DEMANGLE_FMT
    std::cout<<":demangle_fmt(ast)=\n";
    std::cout<<demangle_fmt_type(ast);
    std::cout<<";\n";
  #else
    std::cout<<":demangle(ast)="<<demangle_type(ast)<<";\n";
  #endif//USE_DEMANGLE_FMT
  }
#endif  
  return 0;
}

