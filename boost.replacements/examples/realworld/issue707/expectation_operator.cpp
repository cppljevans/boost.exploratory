#include <boost/spirit/home/x3.hpp>
int
main()
{
    using namespace boost::spirit;
    using namespace boost::spirit::x3::ascii;
    using boost::spirit::x3::lit;
    using boost::spirit::x3::expect;
    auto expect_a_b=char_('a') > char_('b');
    ++expect_a_b;
    return 0;
}    
