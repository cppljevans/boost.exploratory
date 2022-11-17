/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_JAN_07_2013_1245PM)
#define BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_JAN_07_2013_1245PM

#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/pseudo_attribute.hpp>
#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
//#include <boost/spirit/home/x3=support/traits/tuple_traits.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>
#include <boost/spirit/home/x3/support/traits/variant_find_substitute.hpp>
#include <boost/spirit/home/x3/core/detail/parse_into_container.hpp>

#include <boost/mpl/if.hpp>

#include <boost/fusion/include/front.hpp>

#include <boost/type_traits/is_same.hpp>
#include <type_traits>

#pragma push_macro("FILE_SHORT")
#define FILE_SHORT "detail/alternative.hpp"

namespace boost { namespace spirit { namespace x3
{
    template <typename Left, typename Right>
    struct alternative;
}}}

namespace boost { namespace spirit { namespace x3 { namespace detail
{
    struct pass_variant_unused
    {
        typedef unused_type type;

        template <typename T>
        static unused_type
        call(T&)
        {
            return unused_type();
        }
    };

    template <typename Attribute>
    struct pass_variant_used
    {
        typedef Attribute& type;

        static Attribute&
        call(Attribute& v)
        {
            return v;
        }
    };

    template <>
    struct pass_variant_used<unused_type> 
        : pass_variant_unused 
    {};

    template <typename Parser, typename Attribute, typename Context
      , typename Enable = void>
    struct pass_parser_attribute
    {
    private:
        static auto constexpr class_label="pass_parser_attribute<Parser, Attribute,...>";
        
        using attribute_of_type=typename traits::attribute_of<Parser, Context>::type;
        using var_find_sub=traits::variant_find_substitute<Attribute, attribute_of_type>;
        using var_find_sub_type=typename var_find_sub::type;
    public:
        using type=typename
          mpl::if_
          < is_same<Attribute, var_find_sub_type>
          , Attribute&
          , var_find_sub_type
          >::type;
    private:
        template <typename Attribute_>
        static Attribute_&
        call(Attribute_& attribute, mpl::true_)
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',class_label,"::",__func__,"(Attribute_&, Attribute_==var_find_sub_type)"));
            std::cout<<"return Attribute_=\n"<<demangle_fmt_type<Attribute_>()<<";\n";
            Attribute_ result;
            return result;
        }

        template <typename Attribute_>
        static type
        call(Attribute_&, mpl::false_)
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',class_label,"::",__func__,"(Attribute_&, Attribute_!=var_find_sub_type)"));
            std::cout<<"return type=\n"<<demangle_fmt_type<type>()<<";\n";
            type result;
            return result;
        }
    public:
        template <typename Attribute_>
        static type
        call(Attribute_& attribute)
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',class_label,"::",__func__,"(Attribute_&)"));
            var_find_sub::trace_tmpl();
            using found_unref=typename remove_reference<type>::type;
            using same_Attribute_found_type_t=typename is_same<Attribute_,found_unref>::type;
            same_Attribute_found_type_t same_Attribute_found_type_v;
            std::cout<<":same_Attribute_found_type_t="<<demangle_fmt_type<same_Attribute_found_type_t>()<<";\n";
            type result=call(attribute, same_Attribute_found_type_v);
            return result;
        }
    };

    // Pass non-variant attributes as-is
    template <typename Parser, typename Attribute, typename Context
      , typename Enable = void>
    struct pass_non_variant_attribute
    {
        typedef Attribute& type;

        static Attribute&
        call(Attribute& attribute)
        {
            return attribute;
        }
    };

    // Unwrap single element sequences
    template <typename Parser, typename Attribute, typename Context>
    struct pass_non_variant_attribute<Parser, Attribute, Context,
        typename enable_if<traits::is_size_one_sequence<Attribute>>::type>
    {
        typedef typename remove_reference<
            typename fusion::result_of::front<Attribute>::type>::type
        attr_type;

        typedef pass_parser_attribute<Parser, attr_type, Context> pass;
        typedef typename pass::type type;

        template <typename Attribute_>
        static type
        call(Attribute_& attr)
        {
            return pass::call(fusion::front(attr));
        }
    };

    template <typename Parser, typename Attribute, typename Context>
    struct pass_parser_attribute<Parser, Attribute, Context,
        typename enable_if_c<(!traits::is_variant<Attribute>::value)>::type>
        : pass_non_variant_attribute<Parser, Attribute, Context>
    {
        static void trace_tmpl()
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":pass_variant_attribute<Parser, Attribute,...>"));
        }
    };

    template <typename Parser, typename Context>
    struct pass_parser_attribute<Parser, unused_type, Context>
        : pass_variant_unused 
    {
        static void trace_tmpl()
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":pass_variant_attribute<Parser, unused_type,...>"));
        }
    };

    template <typename Parser, typename Attribute, typename Context>
    struct pass_variant_attribute
        : mpl::if_c
          < traits::has_attribute<Parser, Context>::value
          , pass_parser_attribute<Parser, Attribute, Context>
          , pass_variant_unused
          >::type
    {
        static void trace_tmpl()
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":pass_variant_attribute<Parser, Attribute,...>::",__func__));
            using has_attribute=traits::has_attribute<Parser, Context>;
            auto constexpr has_attribute_value=has_attribute::value;
            std::cout<<":has_attribute_value="<<has_attribute_value<<";\n";
            if constexpr(has_attribute_value)
            {
                using pass_par_attribute=pass_parser_attribute<Parser, Attribute, Context>;
                std::cout<<"pass_par_attribute=\n"<<demangle_fmt_type<pass_par_attribute>()<<";\n";
            }
        }
    };

    template <typename L, typename R, typename Attribute, typename Context>
    struct pass_variant_attribute<alternative<L, R>, Attribute, Context>
        : mpl::if_c
          < traits::has_attribute<alternative<L, R>, Context>::value
          , pass_variant_used<Attribute>
          , pass_variant_unused
          >::type
    {
        static void trace_tmpl()
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":pass_variant_attribute<alternative<L,R>,...>"));
            using has_attribute=traits::has_attribute<alternative<L, R>, Context>;
            auto constexpr has_attribute_value=has_attribute::value;
            std::cout<<":has_attribute_value="<<has_attribute_value<<";\n";
            if constexpr(has_attribute_value)
            {
                using pass_var_used=pass_variant_used<Attribute>;
                std::cout<<"pass_var_used=\n"<<demangle_fmt_type<pass_var_used>()<<";\n";
            }
        }
    };

    template <bool Condition>
    struct move_if
    {
        template<typename T1, typename T2>
        static void call(T1& /* attr_ */, T2& /* attr */) 
        {
        }
    };

    template <>
    struct move_if<true>
    {
        template<typename T1, typename T2>
        static void call(T1& attr_, T2& attribute)
        {
            traits::move_to(attr_, attribute);
        }
    };

    template <typename Parser, typename Iterator, typename Context
      , typename RContext, typename Attribute>
    bool parse_alternative(Parser const& p, Iterator& first, Iterator const& last
      , Context const& context, RContext& rcontext, Attribute& attribute)
    {
        using pass = detail::pass_variant_attribute<Parser, Attribute, Context>;
        using pseudo = traits::pseudo_attribute<Context, typename pass::type, Iterator>;

        boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,"::parse_alternative(Parser const&,...,Attribute&)"));
        std::cout<<"pass=\n"<<demangle_fmt_type<pass>()<<";\n";
        std::cout<<"pseudo=\n"<<demangle_fmt_type<pseudo>()<<";\n";
        pass::trace_tmpl();
        typename pseudo::type attr_ = pseudo::call(first, last, pass::call(attribute));
        std::cout<<"attr_=\n"<<demangle_fmt_type(attr_)<<";\n";
        bool result=p.parse(first, last, context, rcontext, attr_);
        std::cout<<"result="<<result<<";\n";
        if (result)
        { 
            bool constexpr not_ref=!std::is_reference<decltype(attr_)>::value;
            std::cout<<"not_ref="<<not_ref<<";\n";
            move_if<not_ref>::call(attr_, attribute);
        }
        return result;
    }

    template <typename Subject>
    struct alternative_helper : unary_parser<Subject, alternative_helper<Subject>>
    {
        static bool const is_pass_through_unary = true;

        using unary_parser<Subject, alternative_helper<Subject>>::unary_parser;

        template <typename Iterator, typename Context
          , typename RContext, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attr) const
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":alternative_helper<Subject>::parse",__LINE__));
            std::cout<<":Subject=\n"<<demangle_fmt_type<Subject>()<<";\n";
            std::cout<<":Attribute=\n"<<demangle_fmt_type<Attribute>()<<";\n";
            return detail::parse_alternative(this->subject, first, last, context, rcontext, attr);
        }
    };

    template <typename Left, typename Right, typename Context, typename RContext>
    struct parse_into_container_impl
    < alternative<Left, Right>
    , Context
    , RContext
    >
    {
        static std::string this_type_name()
        { return "parse_into_container_impl<alternative<Left, Right>,...>"
        ;}
        
        typedef alternative<Left, Right> parser_type;

        template <typename Iterator, typename Attribute>
        static bool call
          ( parser_type const& parser
          , Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attribute
          , mpl::false_//is_variant<container_value>
          )
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',this_type_name(),"::call(not_variant<container_value>)"));
            bool result=detail::parse_into_container(parser.left, first, last, context, rcontext, attribute);
            if(!result)
            {
                result=detail::parse_into_container(parser.right, first, last, context, rcontext, attribute);
            }
            std::cout<<":result="<<result<<";\n";
            return result;
        }

        template <typename Iterator, typename Attribute>
        static bool call
          ( parser_type const& parser
          , Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attribute
          , mpl::true_ //is_variant<container_value>
          )
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',this_type_name(),"::call(yes_variant<container_value>)"));
            alternative_helper<Left> help_left{parser.left};
            bool result=detail::parse_into_container(help_left, first, last, context, rcontext, attribute);
            if(!result)
            {   alternative_helper<Right> help_right{parser.right};
                result=detail::parse_into_container(help_right, first, last, context, rcontext, attribute);
            }
            std::cout<<":result="<<result<<";\n";
            return result;
        }

        template <typename Iterator, typename Attribute>
        static bool call(
            parser_type const& parser
          , Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attribute)
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',this_type_name(),"::call(is_variant<container_value>?)"));
            using container_value_type=typename traits::container_value<Attribute>::type;
            std::cout<<":container_value_type="<<demangle_fmt_type<container_value_type>()<<";\n";
            auto constexpr is_variant=typename traits::is_variant<container_value_type>::type{};
            bool result=call(parser, first, last, context, rcontext, attribute, is_variant);
            //std::cout<<":result="<<result<<";\n";
            return result;
        }
    };

}}}}

#pragma pop_macro("FILE_SHORT")
#endif
