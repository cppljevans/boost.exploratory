#pragma once
/*=============================================================================
    Copyright (c) 2021-2021 Larry Joe Evans
      added use of USE_UNFOLD_LEFT and parse_fold function.
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/utility/trace_scope.hpp>
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#ifdef USE_UNFOLD_LEFT
  #ifdef USE_COLLAPSE_LEFT
    #include <boost/spirit/home/x3/support/traits/attribute_of_binary.hpp>
  #endif//USE_COLLAPSE_LEFT
  #include <boost/spirit/home/x3/support/ast/get_put_index_variant.hpp>
#else
  #include <boost/spirit/home/x3/support/traits/attribute_of_binary.hpp>
  #include <boost/spirit/home/x3/operator/detail/alternative.hpp>
  #include <boost/variant/variant_fwd.hpp>
#endif//USE_UNFOLD_LEFT

#pragma push_macro("FILE_SHORT")
#define FILE_SHORT "unfold_left/*/operator/alternative.hpp"

namespace boost { namespace spirit { namespace x3
{
    template
  #ifdef USE_UNFOLD_LEFT
    < typename... SubParsers
  #else
    < typename Left, typename Right
  #endif
    >
    struct alternative
  #ifdef USE_UNFOLD_LEFT
    : n_ary_parser<alternative<SubParsers...>>
  #else
    : binary_parser<Left, Right, alternative<Left, Right>>
  #endif
    {
      #ifdef USE_UNFOLD_LEFT
        typedef n_ary_parser<alternative<SubParsers...>> base_type;
        constexpr alternative(typename base_type::sub_parsers_t const& sub_parsers)
        : base_type(sub_parsers)
        {
          //std::cout<<FILE_SHORT<<':'<<__LINE__<<":"<<__func__<<":base_type="<<demangle_fmt_type<base_type>()<<";\n";
        }
      #else
        typedef binary_parser<Left, Right, alternative<Left, Right>> base_type;
        constexpr alternative(Left const&left, Right const&right)
        : base_type(left,right)
        {
          //std::cout<<FILE_SHORT<<':'<<__LINE__<<":"<<__func__<<":base_type="<<demangle_fmt_type<base_type>()<<";\n";
        }
      #endif
  #ifdef USE_UNFOLD_LEFT
    private:
          template 
          < typename Iterator
          , typename Context
          , typename RContext
          , std::size_t... Indices
          >
        bool parse_fold
          ( Iterator& first
          , Iterator const& last
          , Context const& context
          , RContext& rcontext
          , unused_type attr
          , std::integer_sequence<std::size_t, Indices...>
          ) const
        {
            bool result = 
              ( false
              || ...
              || this->template sub_parser<Indices>().parse
                 ( first
                 , last
                 , context
                 , rcontext
                 , attr
                 )
              );
            //std::cout<<FILE_SHORT<<':'<<__LINE__<<":alternative:"<<__func__<<"(unused attr):result="<<result<<";\n";
            return result;
        }
          template
          < typename Iterator
          , typename Context
          , typename RContext
          , typename Attribute
          , std::size_t... Indices
          >
        bool parse_fold
          ( Iterator& first
          , Iterator const& last
          , Context const& context
          , RContext& rcontext
          , Attribute& attr
          , std::integer_sequence< std::size_t, Indices...>
          ) const
        {
            bool result = 
              ( false
              || ...
              || this->template sub_parser<Indices>().parse
                 ( first
                 , last
                 , context
                 , rcontext
                 , put<Indices>(attr)
                 )
              );
            std::cout<<"alternative:"<<__func__<<"(Attribute& attr):result="<<result<<";\n";
            return result;
        }
  #endif//USE_UNFOLD_LEFT    
    public:
        template <typename Iterator, typename Context
          , typename RContext>
        bool parse(
            Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, unused_type attr) const
        {
          #if 0
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',__func__,"(unused_type)"));
            std::cout
            #ifdef USE_UNFOLD_LEFT
              <<"\n:demangle_fmt_type(sub_parsers)=\n"<<demangle_fmt_type(base_type::sub_parsers)
            #else
              <<"\n:demangle_fmt_type<Left>=\n"<<demangle_fmt_type<Left>()
              <<"\n:demangle_fmt_type<Right>=\n"<<demangle_fmt_type<Right>()
            #endif//USE_UNFOLD_LEFT
              <<";\n";
          #endif
          #ifdef USE_UNFOLD_LEFT
            bool result=parse_fold
            ( first
            , last
            , context
            , rcontext
            , attr
            , base_type::indices
            );
          #else
            bool result= this->left.parse(first, last, context, rcontext, attr)
               || this->right.parse(first, last, context, rcontext, attr);
          #endif//USE_UNFOLD_LEFT
            return result;
        }
        template <typename Iterator, typename Context
          , typename RContext, typename Attribute>
        bool parse(
            Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attr) const
        {
          #if 1
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',__func__,"(..., Attribute&)"));
            std::cout
            #ifdef USE_UNFOLD_LEFT
              <<":demangle_fmt_type(sub_parsers)=\n"<<demangle_fmt_type(base_type::sub_parsers)<<";\n"
            #else
              <<":demangle_fmt_type<Left>=\n"<<demangle_fmt_type<Left>()<<";\n"
              <<":demangle_fmt_type<Right>=\n"<<demangle_fmt_type<Right>()<<";\n"
            #endif//USE_UNFOLD_LEFT
              <<":demangle_fmt_type(attr)=\n"<<demangle_fmt_type(attr)<<";\n"
              ;
          #endif
          #ifdef USE_UNFOLD_LEFT
            bool result=parse_fold
            ( first
            , last
            , context
            , rcontext
            , attr
            , base_type::indices
            );
          #else
            std::cout<<"parse<Left>...\n";
            bool result=detail::parse_alternative(this->left, first, last, context, rcontext, attr);
            std::cout<<"result<Left>="<<result<<";\n";
            if(!result)
            {
              std::cout<<"parse<Right>...\n";
               result=detail::parse_alternative(this->right, first, last, context, rcontext, attr);
               std::cout<<"result<Right>="<<result<<";\n";
            }
          #endif//USE_UNFOLD_LEFT
            return result;
        }
    };

  #ifdef USE_UNFOLD_LEFT
    template <typename Left, typename Right>
    constexpr auto
    operator|(Left const& left, Right const& right)
    {
        return n_ary_make_parser<alternative>( as_parser(left), as_parser(right) );
    }
  #else
    template <typename Left, typename Right>
    constexpr alternative<
        typename extension::as_parser<Left>::value_type
      , typename extension::as_parser<Right>::value_type>
    operator|(Left const& left, Right const& right)
    {
        return { as_parser(left), as_parser(right) };
    }
  #endif//USE_UNFOLD_LEFT
}}}
#ifdef USE_STD_VARIANT
  #include <boost/spirit/home/x3/support/ast/std_variant.hpp>
  #define SPIRIT_X3_ALTERNATIVE_VARIANT boost::spirit::x3::std_variant
#else
  #include <boost/variant.hpp>
  #define SPIRIT_X3_ALTERNATIVE_VARIANT boost::variant
#endif//USE_STD_VARIANT

namespace boost { namespace spirit { namespace x3 { namespace traits
{
  #ifdef USE_UNFOLD_LEFT
    template <typename Context, typename... SubParsers>
    struct attribute_of<x3::alternative<SubParsers...>, Context>
    {
        using 
      type=
      #ifdef USE_COLLAPSE_LEFT
        typename x3::detail::attribute_of_variant
        < SPIRIT_X3_ALTERNATIVE_VARIANT
        , typename attribute_of<SubParsers,Context>::type...
        >::type
      #else
        SPIRIT_X3_ALTERNATIVE_VARIANT
        < typename attribute_of<SubParsers,Context>::type...
        >
      #endif
        ;
    };
  #else
    template <typename Left, typename Right, typename Context>
    struct attribute_of<x3::alternative<Left, Right>, Context>
        : x3::detail::attribute_of_binary<SPIRIT_X3_ALTERNATIVE_VARIANT, x3::alternative, Left, Right, Context> {};
  #endif//USE_UNFOLD_LEFT
}}}}
#pragma pop_macro("FILE_SHORT")
