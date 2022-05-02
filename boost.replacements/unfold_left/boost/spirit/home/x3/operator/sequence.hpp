#pragma once
/*=============================================================================
    Copyright (c) 2021-2021 Larry Joe Evans
      added use of USE_UNFOLD_LEFT and parse_fold function.
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/directive/expect.hpp>
#ifdef USE_UNFOLD_LEFT
#else
#include <boost/spirit/home/x3/support/traits/attribute_of_binary.hpp>
#include <boost/spirit/home/x3/operator/detail/sequence.hpp>
#endif//USE_UNFOLD_LEFT

namespace boost { namespace spirit { namespace x3
{
    template
  #ifdef USE_UNFOLD_LEFT
    < typename... SubParsers
  #else
    < typename Left, typename Right
  #endif
    >
    struct sequence
  #ifdef USE_UNFOLD_LEFT
    : n_ary_parser<sequence<SubParsers...>,SubParsers...> 
  #else
    : binary_parser<Left, Right, sequence<Left, Right>>
  #endif
    {
      #ifdef USE_UNFOLD_LEFT
        typedef n_ary_parser<sequence<SubParsers...>,SubParsers...> base_type;
      #else
        typedef binary_parser<Left, Right, sequence<Left, Right>> base_type;
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
          , std::integer_sequence< std::size_t, Indices...>
          ) const
        {
            Iterator save = first;
            bool result = 
              ( true
              && ...
              && base_type::template sub_parser<Indices>().parse
                 ( first
                 , last
                 , context
                 , rcontext
                 , attr
                 )
              );
            if(!result)first = save;
            //std::cout<<"sequence:"<<__func__<<"attr=unused:result="<<result<<";\n";
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
            Iterator save = first;
            bool result = 
              ( true
              && ...
              && this->template sub_parser<Indices>().parse
                 ( first
                 , last
                 , context
                 , rcontext
                 , fusion::at_c<Indices>(attr)
                 )
              );
            if(!result)first = save;
            //std::cout<<"sequence:"<<__func__<<":result="<<result<<";\n";
            return result;
        }
  #endif//USE_UNFOLD_LEFT    
    public:
        
      #ifdef USE_UNFOLD_LEFT
        template<typename Left, typename Right>
        constexpr sequence(Left const&left, Right const&right)
        : base_type(left,right)
        {}
        template<typename... Left, typename Right>
        constexpr sequence(sequence<Left...>const&left, Right const&right)
        : base_type(left, right, sequence<Left...>::base_type::indices)
        {}
      #else
        constexpr sequence(Left const&left, Right const&right)
        : base_type(left,right)
        {}
      #endif//USE_UNFOLD_LEFT
        template <typename Iterator, typename Context
          , typename RContext>
        bool parse(
            Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, unused_type attr) const
        {
            boost::trace_scope ts("sequence::parse(...unused_type)");
          #if 0
            std::cout<<__FILE__<<':'<<__LINE__<<':'<<__func__
              <<"\n:demangle_type(sub_parsers)=\n"<<demangle_type(base_type::sub_parsers)
              <<"\n:demangle_type(attr)=\n"<<demangle_type(attr)
              <<";\n";
          #endif
          #ifdef USE_UNFOLD_LEFT
            bool result=parse_fold(
              first, last
            , context, rcontext, attr
            , base_type::indices
            );
          #else
            bool result= detail::parse_sequence(*this, first, last, context, rcontext, attr
              , typename traits::attribute_category<unused_type>::type());
          #endif//USE_UNFOLD_LEFT
            return result;
        }
      #ifdef USE_SEQ_PARSE_STD_VECTOR_SPECIALIZATION
      xxxxxxxxxxxxx
        template <typename Iterator, typename Context
          , typename RContext, typename Attribute>
        bool parse(
            Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, std::vector<Attribute>& attr) const
        {
          #if 1
            boost::trace_scope ts("sequence::parse(..., std::vector<Attribute>&)");
            std::cout<<__FILE__<<':'<<__LINE__
            #ifdef USE_UNFOLD_LEFT
              <<"\n:demangle_fmt_type(sub_parsers)=\n"<<demangle_fmt_type(base_type::sub_parsers)
            #else
              <<"\n:demangle_fmt_type<Left>=\n"<<demangle_fmt_type<Left>()
              <<"\n:demangle_fmt_type<Right>=\n"<<demangle_fmt_type<Right>()
            #endif//USE_UNFOLD_LEFT
              <<"\n:demangle_fmt_type(attr)=\n"<<demangle_fmt_type(attr)
              <<";\n";
          #endif
          return false;
        }
      #endif//USE_STD_VECTOR_SPECIALIZATION
        template <typename Iterator, typename Context
          , typename RContext, typename Attribute>
        bool parse(
            Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attr) const
        {
          #if 1
            boost::trace_scope ts("sequence::parse(..., Attribute&)");
            std::cout<<__FILE__<<':'<<__LINE__
            #ifdef USE_UNFOLD_LEFT
              <<"\n:demangle_fmt_type(sub_parsers)=\n"<<demangle_fmt_type(base_type::sub_parsers)
            #else
              <<"\n:demangle_fmt_type<Left>=\n"<<demangle_fmt_type<Left>()
              <<"\n:demangle_fmt_type<Right>=\n"<<demangle_fmt_type<Right>()
            #endif//USE_UNFOLD_LEFT
              <<"\n:demangle_fmt_type(attr)=\n"<<demangle_fmt_type(attr)
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
            using attribute_category=typename traits::attribute_category<Attribute>::type;
            std::cout<<"attribute_category=\n"<<demangle_fmt_type<attribute_category>()<<";\n";
            bool result=detail::parse_sequence(*this, first, last, context, rcontext, attr
              , attribute_category());
            std::cout<<"sequence::"<<__func__<<":result="<<result<<";\n";
          #endif//USE_UNFOLD_LEFT
            return result;
        }
    };
  #ifdef USE_UNFOLD_LEFT
    template <typename... SubParsers>
    constexpr sequence<SubParsers...> const&
    as_parser(sequence<SubParsers...> const& p)
    {
        return p;
    }
  #endif//USE_UNFOLD_LEFT

    template <typename Left, typename Right>
    constexpr sequence<
        typename extension::as_parser<Left>::value_type
      , typename extension::as_parser<Right>::value_type>
    operator>>(Left const& left, Right const& right)
    {
        return { as_parser(left), as_parser(right) };
    }

    template <typename... Left, typename Right>
    constexpr sequence<
        Left...
      , typename extension::as_parser<Right>::value_type
      >
    operator>>(sequence<Left...> const& left, Right const& right)
    {
        return { left, as_parser(right) };
    }

    template <typename Left, typename Right>
    constexpr auto operator>(Left const& left, Right const& right)
    {
        return left >> expect[right];
    }
}}}

namespace boost { namespace spirit { namespace x3 { namespace traits
{
  #ifdef USE_UNFOLD_LEFT
    template <typename Context, typename... SubParsers>
    struct attribute_of<x3::sequence<SubParsers...>, Context>
    {
       using type=fusion::deque<typename attribute_of<SubParsers,Context>::type...>;
    };
  #else
    template <typename Left, typename Right, typename Context>
    struct attribute_of<x3::sequence<Left, Right>, Context>
        : x3::detail::attribute_of_binary<fusion::deque, x3::sequence, Left, Right, Context> {};
  #endif//USE_UNFOLD_LEFT
}}}}
