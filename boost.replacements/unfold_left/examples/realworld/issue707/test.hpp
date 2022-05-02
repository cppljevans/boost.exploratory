//OriginalSource:
//  copy&pasted selected parts of test/x3/test.hpp on 2022-01-08 
/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_TEST_FEBRUARY_01_2007_0605PM)
#define BOOST_SPIRIT_TEST_FEBRUARY_01_2007_0605PM

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
#endif
