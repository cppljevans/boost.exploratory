/*=============================================================================
    Copyright (c) 2022 Larry Joe Evans

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_IS_UNUSED_AUG_03_2022_0235PM)
#define BOOST_SPIRIT_X3_IS_UNUSED_AUG_03_2022_0235PM

#include <boost/mpl/bool.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    // is_unused<T>: metafunction that evaluates to mpl::true_ if a type T 
    // is unused_type, mpl::false_ otherwise
    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct is_unused
      : mpl::false_
    {};

    template <>
    struct is_unused<unused_type>
      : mpl::true_
    {};
    
    template <typename T>
    struct not_unused
      : mpl::not_<is_unused<T>>
    {};
}}}}

#endif
