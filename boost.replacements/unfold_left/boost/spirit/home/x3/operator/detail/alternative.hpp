/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_JAN_07_2013_1245PM)
#define BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_JAN_07_2013_1245PM
#define BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
#ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
  #define BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE "unfold_left/*/operator/detail/alternative.hpp"
#endif//BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE

#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/pseudo_attribute.hpp>
#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
//#include <boost/spirit/home/x3=support/traits/tuple_traits.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>
#ifdef USE_STD_VARIANT
  #include <boost/spirit/home/x3/support/ast/std_variant.hpp>
#else
  #include <boost/variant/variant_fwd.hpp>
#endif
#include <boost/spirit/home/x3/support/traits/variant_find_substitute.hpp>
#include <boost/spirit/home/x3/core/detail/parse_into_container.hpp>

#include <boost/mpl/if.hpp>

#include <boost/fusion/include/front.hpp>

#include <boost/type_traits/is_same.hpp>
#include <type_traits>

namespace boost { namespace spirit { namespace x3
{
    template 
  #ifdef USE_UNFOLD_LEFT
    < typename...
    >
  #else
    template 
    < typename Left
    , typename Right
    >
  #endif
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
        using attribute_of_type=typename traits::attribute_of<Parser, Context>::type;
        using var_find_sub=traits::variant_find_substitute<Attribute, attribute_of_type>;
        using var_find_sub_type=typename var_find_sub::type;
    public:
        using type=typename
          mpl::if_
          < is_same<Attribute, var_find_sub_type>
          , Attribute&
          , var_find_sub_type
          >::type
          ;
    private:
      //#define BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_PASS_PARSER_ATTRIBUTE_TRACE
        template <typename Attribute_>
        static Attribute_&
        call(Attribute_& attribute, mpl::true_)
        {
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_PASS_PARSER_ATTRIBUTE_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,':',demangle_fmt_type<pass_parser_attribute>(),"::",__func__,"(Attribute_&, Attribute_==var_find_sub_type)"));
            std::cout<<"return Attribute_=\n"<<demangle_fmt_type<Attribute_>()<<";\n";
          #endif
            return attribute;
        }

        template <typename Attribute_>
        static type
        call(Attribute_&, mpl::false_)
        {
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_PASS_PARSER_ATTRIBUTE_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,':',demangle_fmt_type<pass_parser_attribute>(),"::",__func__,"(Attribute_&, Attribute_!=var_find_sub_type)"));
            std::cout<<"return type=\n"<<demangle_fmt_type<type>()<<";\n";
          #endif
            type result;
            return result;
        }
    public:
        static void trace_tmpl()
        {
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_PASS_PARSER_ATTRIBUTE_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,':',demangle_fmt_type<pass_parser_attribute>()));
          #endif
        }
        template <typename Attribute_>
        static type
        call(Attribute_& attribute)
        {
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_PASS_PARSER_ATTRIBUTE_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,':',demangle_fmt_type<pass_parser_attribute>(),"::",__func__,"(Attribute_&)"));
            var_find_sub::trace_tmpl();
          #endif
            using found_unref=typename remove_reference<type>::type;
            using same_Attribute_found_type_t=typename is_same<Attribute_,found_unref>::type;
            same_Attribute_found_type_t same_Attribute_found_type_v;
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_PASS_PARSER_ATTRIBUTE_TRACE
            std::cout<<":same_Attribute_found_type_t="<<demangle_fmt_type<same_Attribute_found_type_t>()<<";\n";
          #endif
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
            //boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,':',demangle_fmt_type<pass_parser_attribute>()));
        }
    };

    template <typename Parser, typename Context>
    struct pass_parser_attribute<Parser, unused_type, Context>
        : pass_variant_unused 
    {
        static void trace_tmpl()
        {
            //boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,':',demangle_fmt_type<pass_parser_attribute>()));
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
          //#define PASS_VARIANT_ATTRIBUTE_TRACE
          #ifdef PASS_VARIANT_ATTRIBUTE_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,":pass_variant_attribute<Parser, Attribute,...>::",__func__));
            using has_attribute=traits::has_attribute<Parser, Context>;
            auto constexpr has_attribute_value=has_attribute::value;
            std::cout<<":has_attribute_value="<<has_attribute_value<<";\n";
            if constexpr(has_attribute_value)
            {
                using pass_par_attribute=pass_parser_attribute<Parser, Attribute, Context>;
                pass_par_attribute::trace_tmpl();
            }
          #endif//PASS_VARIANT_ATTRIBUTE_TRACE
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
          #ifdef PASS_VARIANT_ATTRIBUTE_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,":pass_variant_attribute<alternative<L,R>,...>"));
            using has_attribute=traits::has_attribute<alternative<L, R>, Context>;
            auto constexpr has_attribute_value=has_attribute::value;
            std::cout<<":has_attribute_value="<<has_attribute_value<<";\n";
            if constexpr(has_attribute_value)
            {
                using pass_var_used=pass_variant_used<Attribute>;
                std::cout<<"pass_var_used=\n"<<demangle_fmt_type<pass_var_used>()<<";\n";
            }
          #endif//PASS_VARIANT_ATTRIBUTE_TRACE
        }
    };

    //#define BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_MOVE_IF_TRACE
    template <bool Condition>
    struct move_if
    {
        template<typename T1, typename T2>
        static void call(T1& src, T2& dest) 
        {
          bool const diff_address=boost::addressof(src) != boost::addressof(dest);
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_MOVE_IF_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,':',demangle_fmt_type<move_if>()));
            std::cout<<":type_of(src)=\n"<<demangle_fmt_type(src)<<";\n";
            std::cout<<":type_of(dest)=\n"<<demangle_fmt_type(dest)<<";\n";
            std::cout<<":src="<<src<<";\n";
            std::cout<<":dest="<<dest<<";\n";
            std::cout<<":diff_address="<<diff_address<<";\n";
          #endif
          if(diff_address) 
          {   traits::move_to(src, dest);
          }
        }
    };

    template <>
    struct move_if<true>
    {
        template<typename T1, typename T2>
        static void call(T1& src, T2& dest)
        {
          #if defined(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_MOVE_IF_TRACE)
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,':',demangle_fmt_type<move_if>()));
            std::cout<<":type_of(src)=\n"<<demangle_fmt_type(src)<<";\n";
            std::cout<<":type_of(dest)=\n"<<demangle_fmt_type(dest)<<";\n";
            //std::cout<<":src="<<src<<";\n";
            //std::cout<<":dest="<<dest<<";\n";
          #endif
            traits::move_to(src, dest);
        }
    };

    template <typename Parser, typename Iterator, typename Context
      , typename RContext, typename Attribute>
    bool parse_alternative(Parser const& p, Iterator& first, Iterator const& last
      , Context const& context, RContext& rcontext, Attribute& attribute)
    {
        using pass = detail::pass_variant_attribute<Parser, Attribute, Context>;
        using pseudo = traits::pseudo_attribute<Context, typename pass::type, Iterator>;
        typename pseudo::type attr_ = pseudo::call(first, last, pass::call(attribute));
      #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
        boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,"::parse_alternative(Parser const&,...,Attribute&)"));
        std::cout<<":Parser=\n"<<demangle_fmt_type<Parser>()<<";\n";
        std::cout<<":Attribute=\n"<<demangle_fmt_type<Attribute>()<<";\n";
        std::cout<<":attr_=\n"<<demangle_fmt_type(attr_)<<";\n";
        pass::trace_tmpl();
      #endif
        bool result=p.parse(first, last, context, rcontext, attr_);
      #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
        std::cout<<"result="<<result<<";\n";
      #endif
        if (result)
        { 
            bool constexpr not_ref=!std::is_reference<decltype(attr_)>::value;
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
            std::cout<<"not_ref="<<not_ref<<";\n";
          #endif
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
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,":alternative_helper<Subject>::parse:",__LINE__));
            std::cout<<":Subject=\n"<<demangle_fmt_type<Subject>()<<";\n";
            std::cout<<":Attribute=\n"<<demangle_fmt_type<Attribute>()<<";\n";
          #endif
            return detail::parse_alternative(this->subject, first, last, context, rcontext, attr);
        }
    };

    template <typename... SubParsers, typename Context, typename RContext>
    struct parse_into_container_impl
    < alternative<SubParsers...>
    , Context
    , RContext
    >
    {
        static std::string this_type_name()
        { return "parse_into_container_impl<alternative<Left, Right>,...>"
        ;}
        
        typedef alternative<SubParsers...> parser_type;

        template <typename Iterator, typename Attribute>
        static bool call
          ( parser_type const& parser
          , Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attribute
          , mpl::false_//is_variant<container_value>
          )
        {
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,':',this_type_name(),"::call(not_variant<container_value>)"));
          #endif
            auto parses_into_container=
              [&]
              < std::size_t... Indices
              >
              ( std::integer_sequence< std::size_t, Indices...>)
              { return
                (  false
                || ...
                || detail::parse_into_container
                   ( parser.template sub_parser<Indices>()
                   , first
                   , last
                   , context
                   , rcontext
                   , attribute
                   )
                );
              };
            bool result=parses_into_container(parser_type::indices);
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
            std::cout<<":result="<<result<<";\n";
          #endif
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
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,':',this_type_name(),"::call(yes_variant<container_value>)"));
            std::cout<<":parser_into_container(help_left);\n";
          #endif
            auto parses_into_container=
              [&]
              < std::size_t... Indices
              >
              ( std::integer_sequence< std::size_t, Indices...>)
              { return
                (  false
                || ...
                || detail::parse_into_container
                   ( alternative_helper<SubParsers>(parser.template sub_parser<Indices>())
                   , first
                   , last
                   , context
                   , rcontext
                   , attribute
                   )
                );
              };
            bool result=parses_into_container(parser_type::indices);
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
            std::cout<<":result="<<result<<";\n";
          #endif
            return result;
        }

        template <typename Iterator, typename Attribute>
        static bool call(
            parser_type const& parser
          , Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attribute)
        {
            using container_value_type=typename traits::container_value<Attribute>::type;
            auto constexpr is_variant=typename traits::is_variant<container_value_type>::type{};
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_FILE,':',__LINE__,':',this_type_name(),"::call(is_variant<container_value>?)"));
            std::cout<<":container_value_type="<<demangle_fmt_type<container_value_type>()<<";\n";
          #endif
            bool result=call(parser, first, last, context, rcontext, attribute, is_variant);
          #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
            std::cout<<":result="<<result<<";\n";
          #endif
            return result;
        }
    };

}}}}

#endif
