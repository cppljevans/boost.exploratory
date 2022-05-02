#pragma once
#include <variant>
#include <boost/mpl/list.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp> //detail::remove_forward

namespace boost { namespace spirit { namespace x3
{
    template <typename _First, typename ..._Rest>
    struct std_variant
    : public std::variant<_First, _Rest...>
    {
        // tell spirit that this is an adapted variant
        struct adapted_variant_tag;

        using variant_type = std::variant<_First, _Rest...>;
        using types        = mpl::list<typename detail::remove_forward<_Rest>::type...>;
        using base_type    = variant_type;
        using base_type::operator=;
        auto which()const
        {  
            return base_type::index();
        }
        base_type& base(){ return *this;}
        base_type const& base()const{ return *this;}
        template <typename F>
        typename F::result_type apply_visitor(F const& v)
        {
            return std::visit(v,base());
        }

        template <typename F>
        typename F::result_type apply_visitor(F const& v) const
        {
            return std::visit(v,base());
        }

        template <typename F>
        typename F::result_type apply_visitor(F& v)
        {
            return std::visit(v,base());
        }

        template <typename F>
        typename F::result_type apply_visitor(F& v) const
        {
            return std::visit(v,base());
        }
        template< typename Out>
        Out&
        print(Out&sout)const
        {
        ; sout<<std::string("std_variant( index=")<<this->index()
        ; base_type const&std_var=*this;
        ; std::visit //based on https://sodocumentation.net/cplusplus/topic/5239/std--variant@2021-10-01.1820CST
          ( [&sout](auto&& e)->void
            { sout <<", val="<< e <<')'
            ;}
          , std_var
          );
        ; return sout
        ;}
        friend std::ostream&
        operator<<(std::ostream&sout, std_variant const&x)
        { return x.print(sout);}
    };//std_variant

}}}
namespace boost
{
    template <typename T, typename ...Types>
    inline T const&
    get(boost::spirit::x3::std_variant<Types...> const& x)
    {
        return std::get<T>(x);
    }

    template <typename T, typename ...Types>
    inline T&
    get(boost::spirit::x3::std_variant<Types...>& x)
    {
        return std::get<T>(x);
    }

    template <typename T, typename ...Types>
    inline T const*
    get(boost::spirit::x3::std_variant<Types...> const* x) noexcept
    {
        return std::get_if<T>(x);
    }

    template <typename T, typename ...Types>
    inline T*
    get(boost::spirit::x3::std_variant<Types...>* x) noexcept
    {
        return std::get_if<T>(x);
    }
}
