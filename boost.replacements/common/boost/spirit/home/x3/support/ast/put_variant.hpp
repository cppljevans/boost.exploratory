#pragma once
#include <boost/variant.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector.hpp>
namespace boost
{
    template 
    < long I
    , typename ...Types
    , typename T=typename mpl::at_c<mpl::vector<Types...>,I>::type
    >
    inline T&
    put(boost::variant<Types...>&x, T const&t=T())
    {
        x=t;
        return get<T>(x);
    }

}
