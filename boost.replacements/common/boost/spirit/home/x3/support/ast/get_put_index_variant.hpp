#pragma once
#include <boost/variant.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/spirit/home/x3/support/ast/std_variant.hpp>
namespace boost
{
    template 
    < long I
    , typename ...Types
    >
    auto&
    get(spirit::x3::std_variant<Types...>&x)
    {
        return std::get<I>(x);
    }
    template 
    < long I
    , typename ...Types
    >
    auto const&
    get(spirit::x3::std_variant<Types...>const&x)
    {
        return std::get<I>(x);
    }
    template
    < long I
    , typename ...Types
    >
    auto&
    get(variant<Types...>&x)
    {
        using T=typename mpl::at_c<mpl::vector<Types...>,I>::type;
        return get<T>(x);
    }
    template
    < long I
    , typename ...Types
    >
    auto const&
    get(variant<Types...>const&x)
    {
        using T=typename mpl::at_c<mpl::vector<Types...>,I>::type;
        return get<T>(x);
    }
    template 
    < long I
    , typename ...Types
    , typename T=typename mpl::at_c<mpl::vector<Types...>,I>::type
    >
    inline T&
    put(variant<Types...>&x, T const&t=T())
    {
        x=t;
        return get<T>(x);
    }
    template 
    < long I
    , typename ...Types
    , typename T=typename mpl::at_c<mpl::vector<Types...>,I>::type
    >
    inline T&
    put(spirit::x3::std_variant<Types...>&x, T const&t=T())
    {
        return x.template emplace<I>(t);
    }
    template
    < typename ...Types
    >
    inline auto
    index(variant<Types...>const&x)
    {
        return x.which();
    }
    template
    < typename ...Types
    >
    constexpr inline auto
    index(spirit::x3::std_variant<Types...>const&x)
    {
        return x.index();
    }

}
