//cp'ed from the 2nd linked file at:
//  https://github.com/boostorg/spirit/issues/721
//============================
#include <boost/spirit/home/x3.hpp>
#include <boost/variant/variant.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include <string>
#include <vector>
#include <iostream>
using namespace std;
int main()
{
    namespace x3 = boost::spirit::x3;
    using namespace x3;
    using x3::double_;

    std::string x = "";
    auto b = x.begin();
    auto e = x.end();
    
    struct A{};
    struct B{};
    struct C{};
    using X = boost::variant<A,B>;
    boost::variant<C, std::pair<double, X>> target;

    bool parsed = x3::phrase_parse(b,e, 

      ( double_ >> lit("=")  >> ( attr(A{}) | attr(B{}) ))
      | attr(C{})
        
     , x3::space, target );
    cout << "parsed?" << parsed;

}
