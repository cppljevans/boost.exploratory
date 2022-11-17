//OriginalSource:
//  copy&pasted on 2021-12-29.0553CST from 1st code listing here:
/*
https://github.com/boostorg/spirit/issues/707
 */
//===================
#include <iostream>
#include <string>
#include <vector>
#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#include <boost/utility/trace_scope.hpp>
#endif

using f3_ast = std::vector<int>;
struct S1 {
    double   bStar;
    uint64_t f1;
    uint64_t f2;
    f3_ast   f3;
};

struct S2 {
    uint64_t f1;
    uint64_t f2;
    f3_ast   f3;
};

#ifdef USE_UNFOLD_LEFT
  #include <boost/spirit/home/x3/support/ast/std_variant.hpp>
  using f6_ast=boost::spirit::x3::std_variant<S1, S2>;
#else
  #include <boost/variant.hpp>
  using f6_ast=boost::variant<S1, S2>;
#endif//USE_UNFOLD_LEFT

struct LogData {
    int      f3;
    int      f4;
    uint64_t f5;
    f6_ast   f6;
};

auto dataSet = std::vector<LogData>{};

#include <boost/fusion/adapted/struct/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(S1, bStar, f1, f2, f3);
BOOST_FUSION_ADAPT_STRUCT(S2, f1, f2, f3);
BOOST_FUSION_ADAPT_STRUCT(LogData, f3, f4, f5, f6);

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>
namespace x3 = boost::spirit::x3;

auto f3_content
  = x3::int_ % ','
  ;
auto f3_parser 
  = '{' 
  >> f3_content
  >> '}'
  ;
#ifdef USE_UNFOLD_LEFT
  #include <boost/spirit/home/x3/core/make_transform_parser.hpp>
  #define USE_TUPLE_TIE
  #ifdef USE_TUPLE_TIE
    #include <boost/fusion/tuple/tuple_tie.hpp>
  #endif//USE_TUPLE_TIE
namespace boost{namespace spirit{namespace x3{namespace traits{
  using f3_aof=x3::attribute_of_default<decltype(f3_parser)>;

  template
  <
  >
  struct
transform_attribute
  < f3_ast
  , f3_aof
  , transform_parser_id
  >
  {
      using to_attribute_type=f3_ast;
    #ifdef USE_TUPLE_TIE
      static auto pre(to_attribute_type& to_attr) 
      { return boost::fusion::tie(unused,to_attr,unused);
      }
      
      template<typename From, typename To>
      static void post(From&, To&)
      {  //do nothing since the pre function copied this to_attr ref 
         //to the value passed to the transform_parser_attribute::from_parser.
      }
    #else  
      using from_attribute_type=f3_aof;
      static auto pre(to_attribute_type& to_attr) 
      { return from_attribute_type(); 
      }
  
      static void post(to_attribute_type& to_attr, from_attribute_type& from_attr)
      {
          //traits::move_to(from_attr, to_attr);
          to_attr=at_c<1>(from_attr);
      }
    #endif//USE_TYPLE_TIE
  }; 
}}}}
auto f3_xform
    = x3::make_transform_parser<f3_ast>(f3_parser)
    ;
#else    
auto f3_xform
    = f3_parser
    ;
#endif//USE_UNFOLD_LEFT

int main() {
  #ifdef USE_TRACE_SCOPE
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif
    auto s = std::string
    {
R"(  39 45 13833 1.186186 2796264 221200 {13,  9,  4, 10,  2,  3}
     34 44
      45264 1.227986 2796264 884752 {14,  4,  0,  9, 11,  2}
     44 55 66666          7777777 888888 {10, 20,  0, 30, 40, 50}
  )"
     };
    if (x3::phrase_parse(
        s.begin(), s.end(),
        (  
            x3::int_ >> x3::int_ >> x3::uint64
         >> (
                (
                    x3::double_ >> x3::uint64 >> x3::uint64
                 >> f3_xform
                )
              | (
                    x3::uint64 >> x3::uint64
                 >> f3_xform
                )
            )
        ) % x3::eol,
        x3::blank,
        dataSet
    ))
    {
        std::cout<<"Parsing success\n";
        for(unsigned i=0; i<dataSet.size(); ++i)
        {
          std::cout<<"dataSet["<<i<<"]=\n";
          x3::traits::print_attribute(std::cout,dataSet[i]);
          std::cout<<";\n";
        }
    }
    else
    {
        std::cout<<"Parsing failed\n";
    }
}
