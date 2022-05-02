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
#include <boost/spirit/home/x3/support/templ_expr/demangle_fmt_type.hpp>
#include <boost/utility/trace_scope.hpp>
#endif

#include "move_to_src_logdata_fwd.hpp"

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

struct S2 {
    uint32_t f0;
    int64_t  f1;
};
BOOST_FUSION_ADAPT_STRUCT(S2, f0, f1);

struct LogData {
    int16_t                f0;
    boost::variant<S1, S2> f1;
};
BOOST_FUSION_ADAPT_STRUCT(LogData, f0, f1);

#include "move_to_src_logdata.hpp"

int main() {
  #ifdef USE_TRACE_SCOPE
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif
    auto s = std::string
    {
R"(  10 1.01 100 1000
     20      200 2000)"
    };
    auto dataSet = std::vector<LogData>{};
    if (x3::phrase_parse(
        s.begin(), s.end(),
        (
            x3::int16  //LogData.f0
         >> (//LogData.f1:variant<S1,S2>
              (//S1:
                 x3::double_ //S1.f0
              >> x3::uint32  //S1.f1
              >> x3::int64   //S1.f2
              )
            | (//S2:
                 x3::uint32  //S2.f0 
              >> x3::int64   //S2.f1
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
