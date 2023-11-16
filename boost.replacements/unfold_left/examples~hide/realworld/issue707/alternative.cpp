//OriginalSource:
//  copy&pasted selected parts of test/x3/alternative.cpp on 2022-01-08
/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <boost/spirit/home/x3.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/include/at.hpp>

#include <string>
#include <iostream>
#include <vector>
//{test.hpp
//OriginalSource:
//  copy&pasted selected parts of test/x3/test.hpp on 2022-01-08 
#include <boost/spirit/home/x3/core/parse.hpp>

namespace spirit_test
{
    template <typename Char, typename Parser, typename Attr>
    bool test_attr(Char const* in, Parser const& p
        , Attr& attr, bool full_match = true)
    {
        Char const* last = in;
        while (*last)
            last++;
        return boost::spirit::x3::parse(in, last, p, attr)
            && (!full_match || (in == last));
    }
}
//}test.hpp

int
main()
{
    using spirit_test::test_attr;

    using boost::spirit::x3::attr;
    bool result;
    { // conversion between alternatives
    #if 0
        struct X {};
        struct Y {};
        struct Z {};
        boost::variant<X, Y, Z> v;
        boost::variant<Y, X> x{X{}};
        v = x; // boost::variant supports that convertion
     
        auto const p = 'x' >> attr(x) | 'z' >> attr(Z{});
        result=test_attr("z", p, v);
        std::cout<<__LINE__<<":result="<<result<<";\n";
        if(result)
        {   result=(boost::get<Z>(&v) != nullptr);
            std::cout<<__LINE__<<":result="<<result<<";\n";
        }
        result=test_attr("x", p, v);
        std::cout<<__LINE__<<":result="<<result<<";\n";
        if(result)
        {   result=(boost::get<X>(&v) != nullptr);
            std::cout<<__LINE__<<":result="<<result<<";\n";
        }
    #endif
    }

    { // regression test for #679
    #if 0
        using Qaz = std::vector<boost::variant<int>>;
        using Foo = std::vector<boost::variant<Qaz, int>>;
        using Bar = std::vector<boost::variant<Foo, int>>;
        Bar x;
        result=test_attr("abaabb", +('a' >> attr(Foo{}) | 'b' >> attr(int{})), x);
        std::cout<<__LINE__<<":result="<<result<<";\n";
    #endif
    }
    return 0;
}
