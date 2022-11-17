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
//#include <boost/spirit/home/x3/support/traits/is_same_aof_uda.hpp>

#include "move_to_src_logdata_fwd.hpp"

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

struct S2 {
    uint32_t f0;
    int64_t  f1;
};
BOOST_FUSION_ADAPT_STRUCT(S2, f0, f1);

#define SPIRIT_X3_ALTERNATIVE_VARIANT boost::variant
//#define SHOW_ISSUE707_ERROR
#ifdef SHOW_ISSUE707_ERROR
struct LogData {
    int16_t                f0;
    SPIRIT_X3_ALTERNATIVE_VARIANT<S1, S2> f1;
};
BOOST_FUSION_ADAPT_STRUCT(LogData, f0, f1);
#else
using LogData =
    SPIRIT_X3_ALTERNATIVE_VARIANT<S1, S2>
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
#include "move_to_src_logdata.hpp"

int main() {
  #ifdef USE_TRACE_SCOPE
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif
    std::cout<<std::boolalpha;
    auto s = std::string
#ifdef SHOW_ISSUE707_ERROR
    {
R"(  10 1.01 100 1000
     20      200 2000)"
    };
#else
    {
R"(     1.01 100 1000
             200 2000)"
    };
#endif//SHOW_ISSUE707_ERROR
    auto list_subject=
#ifdef SHOW_ISSUE707_ERROR
       x3::int16  //LogData.f0
    >> (//LogData.f1:variant<S1,S2>
#endif//SHOW_ISSUE707_ERROR
         (//S1:
            x3::double_ //S1.f0
         >> x3::uint32  //S1.f1
         >> x3::int64   //S1.f2
         )
       | (//S2:
            x3::uint32  //S2.f0 
         >> x3::int64   //S2.f1
         )
#ifdef SHOW_ISSUE707_ERROR
       )
#endif//SHOW_ISSUE707_ERROR
    ;
  #if 0
    {
      using list_type=decltype(list_subject);
      using list_aof=typename x3::traits::attribute_of<list_type,x3::unused_type>::type;
      std::cout<<__func__<<":list_aof="<<demangle_fmt_type<list_aof>()<<";\n";
      using same_aof_LogData=x3::traits::is_same_enough<list_aof,LogData>;
      bool constexpr same_aof_LogData_value=same_aof_LogData::value;
      std::cout<<__func__<<":same_aof_LogData="<<same_aof_LogData_value<<";\n";
      if constexpr (!same_aof_LogData_value)
      {
        using subs_aof_LogData=x3::traits::is_substitute<list_aof,LogData>;
        bool constexpr subs_aof_LogData_value=subs_aof_LogData::value;
        std::cout<<__func__<<":subs_aof_LogData="<<subs_aof_LogData_value<<";\n";
        std::cout<<__func__<<":subs_aof_LogData::trace_tmpl:\n";
        subs_aof_LogData::trace_tmpl();
        //x3::traits::is_substitute_trace_tmpls<list_aof,LogData>();
      }
    }
  #endif
  #if 1
    {
      auto dataSet = std::vector<LogData>{};
      if 
      ( x3::phrase_parse
        ( s.begin(), s.end()
        , list_subject % x3::eol
        , x3::blank
      #if 1
        , dataSet
      #else
        , x3::unused
      #endif
        )
      )
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
  #endif
}
