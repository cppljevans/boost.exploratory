//OriginalSource:
//  copy&pasted on 2021-12-29.0553CST from 1st code listing here:
/*
https://github.com/boostorg/spirit/issues/707
 */
//===================
#include <cstdio>
#include <string>
#include <vector>

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/variant.hpp>

namespace x3 = boost::spirit::x3;

struct S1 {
    double           bStar;
    uint64_t         f1;
    uint64_t         f2;
    std::vector<int> f3;
};
BOOST_FUSION_ADAPT_STRUCT(S1, bStar, f1, f2, f3);

struct S2 {
    uint64_t         f1;
    uint64_t         f2;
    std::vector<int> f3;
};
BOOST_FUSION_ADAPT_STRUCT(S2, f1, f2, f3);

struct LogData {
    int      f3;
    int      f4;
    uint64_t f5;
    boost::variant<S1, S2> f6;
};
BOOST_FUSION_ADAPT_STRUCT(LogData, f3, f4, f5, f6);

int main(int argc, char *argv[]) {
    auto s = std::string{
R"(  39 45 13833 1.186186 2796264 221200 {13,  9,  4, 10,  2,  3}
     34 44 45264 1.227986 2796264 884752 {14,  4,  0,  9, 11,  2})"};
    auto dataSet = std::vector<LogData>{};
    if (x3::phrase_parse(
        s.begin(), s.end(),
        (
            x3::int_ >> x3::int_ >> x3::uint64
         >> (
                (
                    x3::double_ >> x3::uint64 >> x3::uint64
                 >> '{' >> (x3::int_ % ',') >> '}'
                )
              | (
                    x3::uint64 >> x3::uint64
                 >> '{' >> (x3::int_ % ',') >> '}'
                )
            )
        ) % x3::eol,
        x3::blank,
        dataSet
    ))
    {
        printf("Parsing success\n");
    }
    else
    {
        printf("Parsing failed\n");
    }
}
