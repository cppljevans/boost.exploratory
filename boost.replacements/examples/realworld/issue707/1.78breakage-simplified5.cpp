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
#include "is_same_aof_uda.hpp"

#include <boost/variant.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>

namespace x3 = boost::spirit::x3;
struct S1 {
    double   f0;
    uint32_t f1;
    int64_t  f2;
};
BOOST_FUSION_ADAPT_STRUCT(S1, f0, f1, f2);

auto S1_parser_expr
  =  x3::double_ //S1.f0
  >> x3::uint32  //S1.f1
  >> x3::int64   //S1.f2
  ;
using S1_parser_type=decltype(S1_parser_expr);
using S1_aof=typename x3::traits::attribute_of<S1_parser_type,x3::unused_type>::type;

struct S2 {
    uint32_t f0;
    int64_t  f1;
};
BOOST_FUSION_ADAPT_STRUCT(S2, f0, f1);

auto S2_parser_expr
  =  x3::uint32  //S2.f0
  >> x3::int64   //S2.f1
  ;
using S2_parser_type=decltype(S2_parser_expr);
using S2_aof=typename x3::traits::attribute_of<S2_parser_type,x3::unused_type>::type;

using var_S1_S2=boost::variant<S1, S2>;

//#define SHOW_ISSUE707_ERROR
#ifdef SHOW_ISSUE707_ERROR
struct LogData {
    int16_t   f0;
    var_S1_S2 f1;
};
BOOST_FUSION_ADAPT_STRUCT(LogData, f0, f1);
#else
using LogData =
    var_S1_S2
;
#endif//SHOW_ISSUE707_ERROR
  std::ostream&
operator<<
  ( std::ostream& sout
  , LogData const& x
  )
  { sout<<"LogData=\n"
  ; x3::traits::print_attribute(sout,x);
  ; return sout
  ;}

int main() 
{
  #ifdef USE_TRACE_SCOPE
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif
    std::vector<std::string> lines =
#ifdef SHOW_ISSUE707_ERROR
    { std::string("10 1.01 100 1000")
    , std::string("20      200 2000")
    };
#else
    { std::string("   1.01 100 1000")
    , std::string("        200 2000")
    };
#endif//SHOW_ISSUE707_ERROR
    auto LogData_parser_expr=
#ifdef SHOW_ISSUE707_ERROR
       x3::int16  //LogData.f0
    >> (//LogData.f1:variant<S1,S2>
#endif//SHOW_ISSUE707_ERROR
         S1_parser_expr
       | S2_parser_expr
#ifdef SHOW_ISSUE707_ERROR
       )
#endif//SHOW_ISSUE707_ERROR
    ;
    using LogData_parser_type=decltype(LogData_parser_expr);
    using LogData_aof=typename x3::traits::attribute_of<LogData_parser_type,x3::unused_type>::type;
    std::cout<<":LogData_aof="<<demangle_fmt_type<LogData_aof>()<<";\n";
    using same_aof_LogData=is_same_aof_uda<LogData_aof,LogData>;
    std::cout<<":same_aof_LogData="<<same_aof_LogData::value<<";\n";
    auto dataSet = LogData{};
    for( std::string line:lines)
    {
      boost::trace_scope ts(line);
      if (x3::phrase_parse(
          line.begin(), line.end(),
          LogData_parser_expr,
          x3::blank,
          dataSet
      ))
      {
          std::cout<<"Parsing success\n";
          std::cout<<"dataSet=\n";
          x3::traits::print_attribute(std::cout,dataSet);
          std::cout<<";\n";
      }
      else
      {
          std::cout<<"Parsing failed\n";
      }
    }
    return 0;    
}
