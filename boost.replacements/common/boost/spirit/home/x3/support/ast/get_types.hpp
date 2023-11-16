#pragma once
/*=============================================================================
    Copyright (c) 2022 Larry Joe Evans

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/mpl/vector.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit { namespace x3
{
    template<typename>
    struct get_types;
    
    template<typename ...Types>
    struct get_types<boost::variant<Types...>>
    {
        using type=mpl::vector<Types...>;
    };
    template<typename ...Types>
    struct get_types<x3::variant<Types...>>
    {
        using type=mpl::vector<Types...>;
    };
}}}
