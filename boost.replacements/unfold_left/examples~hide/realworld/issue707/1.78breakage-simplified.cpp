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
#include <boost/utility/demangle_fmt_type.hpp>
#include <boost/utility/trace_scope.hpp>
#endif

//#define MOVE_TO_SRC_LOGDATA
#ifdef MOVE_TO_SRC_LOGDATA
struct LogData;
namespace boost{namespace spirit{namespace x3{namespace traits{
    template<typename Source>
    inline void move_to(Source& src, LogData& dest)
    ;
}}}}
#endif//MOVE_TO_SRC_LOGDATA

#include <boost/variant.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>

namespace x3 = boost::spirit::x3;

struct S1 {
    double           f0;
    uint32_t         f1;
    uint64_t         f2;
    std::vector<int32_t> f3;
};
BOOST_FUSION_ADAPT_STRUCT(S1, f0, f1, f2, f3);

struct S2 {
    uint32_t         f0;
    uint64_t         f1;
    std::vector<int32_t> f2;
};
BOOST_FUSION_ADAPT_STRUCT(S2, f0, f1, f2);

struct LogData {
    int16_t  f0;
    uint16_t f1;
    boost::variant<S1, S2> f2;
};
BOOST_FUSION_ADAPT_STRUCT(LogData, f0, f1, f2);

#ifdef MOVE_TO_SRC_LOGDATA
namespace boost{namespace spirit{namespace x3{namespace traits{
    unsigned move_to_count=0;
    template<typename Source>
    void print_to_cout(Source& src)
    {  std::cout<<src;
    }
    void print_to_cout(unsigned char& src)
    {  std::cout<<"'"<<char(src)<<"'";
    }
    template<typename Source>
    inline void move_to(Source& src, LogData& dest)
    {
        dest.f1=++move_to_count;
        std::cout
        <<__FILE__<<':'
        <<__LINE__<<':'
        <<__func__
        <<":move_to_count="<<move_to_count
        <<":demangle_fmt_type(src)="
        <<demangle_fmt_type(src)
        <<":src=";
        print_to_cout(src);
        std::cout<<":dest=LogData"
        <<";\n";
    }
}}}}
#endif//MOVE_TO_SRC_LOGDATA

int main(int argc, char *argv[]) {
  #ifdef USE_TRACE_SCOPE
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif
    auto s = std::string{
R"(  10 11 1.01 100 101 {1000, 1001}
     20 21      200 201 {2000, 2001})"};
    auto dataSet = std::vector<LogData>{};
    if (x3::phrase_parse(
        s.begin(), s.end(),
        (
            x3::int16  //LogData.f0
         >> x3::uint16 //LogData.f1
         >> (//LogData.f2:variant<S1,S2>
              (//S1:
                 x3::double_ //S1.f0
              >> x3::uint32  //S1.f1
              >> x3::uint64  //S1.f2
              >> '{' //unused_type
              >> (x3::int32 % ',')//S1.f3
              >> '}' //unused_type
              )
            | (//S2:
                 x3::uint32  //S2.f0 
              >> x3::uint64  //S2.f1
              >> '{' //unused_type
              >> (x3::int32 % ',') //S2.f2
              >> '}' //unused_type
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
