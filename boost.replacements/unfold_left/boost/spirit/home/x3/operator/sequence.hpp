/*=============================================================================
    Copyright (c) 2021-2021 Larry Joe Evans
      added use of USE_UNFOLD_LEFT and parse_fold function.
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_SEQUENCE_JAN_06_2013_1015AM)
#define BOOST_SPIRIT_X3_SEQUENCE_JAN_06_2013_1015AM
#define BOOST_SPIRIT_X3_SEQUENCE_TRACE
#ifdef BOOST_SPIRIT_X3_SEQUENCE_TRACE
  #define BOOST_SPIRIT_X3_SEQUENCE_FILE "unfold_left/*/operator/sequence.hpp"
  #include <boost/utility/trace_scope.hpp>
  #include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#endif//BOOST_SPIRIT_X3_SEQUENCE_TRACE

#ifdef USE_COLLAPSE_LEFT
  #include <boost/fusion/iterator/equal_to.hpp>
  #include <boost/spirit/home/x3/support/traits/attribute_of_binary.hpp>
  #include <boost/spirit/home/x3/operator/detail/sequence.hpp>
  #include <type_traits>
#endif//USE_COLLAPSE_LEFT
#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/directive/expect.hpp>

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename Context, typename... SubParsers>
    struct attribute_of<x3::sequence<SubParsers...>, Context>
    {
        using 
      type=
      #ifdef USE_COLLAPSE_LEFT
        typename x3::detail::attribute_of_sequence
        < fusion::deque
        , typename attribute_of<SubParsers,Context>::type...
        >::type
      #else
        fusion::deque
        < typename attribute_of<SubParsers,Context>::type...
        >
      #endif//USE_COLLAPSE_LEFT
        ;
    };
}}}}

namespace boost { namespace spirit { namespace x3
{
      template
      < typename... SubParsers
      >
    struct sequence
      : n_ary_parser<sequence<SubParsers...>> 
      {
          typedef n_ary_parser<sequence<SubParsers...>> base_type;
          constexpr sequence(typename base_type::sub_parsers_t const& sub_parsers)
          : base_type(sub_parsers)
          {
            //std::cout<<BOOST_SPIRIT_X3_SEQUENCE_FILE<<':'<<__LINE__<<":"<<__func__<<":base_type="<<demangle_fmt_type<base_type>()<<";\n";
          }
      private:
            template 
            < typename Iterator
            , typename Context
            , typename RContext
            , std::size_t... Indices
            >
            bool 
          parse_fold
            ( Iterator& first
            , Iterator const& last
            , Context const& context
            , RContext& rcontext
            , std::integer_sequence< std::size_t, Indices...>
            ) const
            //For when Attribute is unused_type
            {
                Iterator save = first;
                bool result = 
                  (  true
                  && ...
                  && base_type::template sub_parser<Indices>().parse
                     ( first
                     , last
                     , context
                     , rcontext
                     , unused
                     )
                  );
                if(!result)first = save;
                //std::cout<<"sequence:"<<__func__<<"attr=unused:result="<<result<<";\n";
                return result;
            }
          #ifndef USE_COLLAPSE_LEFT
            template 
            < typename Iterator
            , typename Context
            , typename RContext
            , typename Attribute
            , std::size_t... Indices
            >
            bool 
          parse_fold
            ( Iterator& first
            , Iterator const& last
            , Context const& context
            , RContext& rcontext
            , Attribute& attr
            , std::integer_sequence< std::size_t, Indices...>
            ) const
            //For when Attribute is not unused_type
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
                     //without collapsed attributes, the number of elements
                     //of sub_parser's is the same as the number of elements
                     //of Attribute; hence, the same Indices can be used
                     //for both.
                     )
                  );
                if(!result)first = save;
                //std::cout<<"sequence:"<<__func__<<":result="<<result<<";\n";
                return result;
            }
          #endif//USE_COLLAPSE_LEFT
      public:
  
          template <typename Iterator, typename Context
            , typename RContext>
          bool parse(
              Iterator& first, Iterator const& last
            , Context const& context, RContext& rcontext, unused_type) const
          {
            #ifdef BOOST_SPIRIT_X3_SEQUENCE_TRACE
              boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_SEQUENCE_FILE,':',__LINE__,":sequence::parse(*, unused_type)"));
              std::cout<<__FILE__<<':'<<__LINE__<<':'<<__func__
                <<"\n:demangle_type(sub_parsers)=\n"<<demangle_type(base_type::sub_parsers)
                <<"\n:type(attr)=unused_type"
                <<";\n";
            #endif
              bool result=
                parse_fold
                ( first
                , last
                , context
                , rcontext
                , base_type::indices
                );
            #ifdef BOOST_SPIRIT_X3_SEQUENCE_TRACE
              std::cout<<"sequence::"<<__func__<<"(*, unused_type):result="<<result<<";\n";
            #endif
              return result;
          }
          
          template <typename Iterator, typename Context
            , typename RContext, typename Attribute>
          bool parse(
              Iterator& first, Iterator const& last
            , Context const& context, RContext& rcontext, Attribute& attr) const
          {
            #ifdef BOOST_SPIRIT_X3_SEQUENCE_TRACE
              boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_SEQUENCE_FILE,':',__LINE__,':',__func__,"(*, Attribute&)"));
              std::cout
                <<":demangle_fmt_type(sub_parsers)=\n"<<demangle_fmt_type(base_type::sub_parsers)<<";\n"
                <<":demangle_fmt_type(attr)=\n"<<demangle_fmt_type(attr)<<";\n"
                ;
            #endif
              #ifdef USE_COLLAPSE_LEFT
                bool result=detail::parse_sequence
                //with collapsed attributes, must make provision
                //for subparsers with attributes of unused_type
                //since Attribute is assumed to have unused_type's
                //removed.
                ( *this
                , first
                , last
                , context
                , rcontext
                , attr
                , typename traits::attribute_category<Attribute>::type()
                )
                ;
              #else
                bool result=parse_fold
                ( first
                , last
                , context
                , rcontext
                , attr
                , base_type::indices
                )
                ;
              #endif//USE_COLLAPSE_LEFT
              #ifdef BOOST_SPIRIT_X3_SEQUENCE_TRACE
                std::cout<<"sequence::"<<__func__<<"(*, Attribute&):result="<<result<<";\n";
              #endif
              return result;
          }
      };//struct sequence
    
      template <typename Left, typename Right>
      auto constexpr
    operator>>(Left const& left, Right const& right)
      {
        return n_ary_make_parser<sequence>( as_parser(left), as_parser(right) );
      }

      template <typename Left, typename Right>
      auto constexpr
    operator>(Left const& left, Right const& right)
      {
        return left >> expect[right];
      }
}}}

#endif//!defined(BOOST_SPIRIT_X3_SEQUENCE_JAN_06_2013_1015AM)
