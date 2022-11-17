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
#include <boost/spirit/home/x3/directive/expect.hpp>
#ifdef USE_UNFOLD_LEFT
  #ifdef USE_COLLAPSE_LEFT
    #include <boost/fusion/iterator/equal_to.hpp>
    #include <boost/spirit/home/x3/support/traits/attribute_of_binary.hpp>
    #include <type_traits>
  #endif//USE_COLLAPSE_LEFT
#else
  #include <boost/spirit/home/x3/support/traits/attribute_of_binary.hpp>
  #include <boost/spirit/home/x3/operator/detail/sequence.hpp>
#endif//USE_UNFOLD_LEFT

#pragma push_macro("FILE_SHORT")
#define FILE_SHORT "unfold_left/*/operator/sequence.hpp"

namespace boost { namespace spirit { namespace x3
{
  #if defined(USE_UNFOLD_LEFT) && defined(USE_COLLAPSE_LEFT)
    template
    < typename Iterator
    , typename Context
    , typename RContext
    , typename Parsers
    , typename Attributes
    >
    struct sequence_parse_fold
    {
        using attr_category = typename traits::attribute_category<Attributes>::type;
        static_assert(is_same<traits::tuple_attribute, attr_category>::value,
            "The parser expects tuple-like attribute type");
            
        static int constexpr parsers_size = fusion::result_of::size<Parsers>::value;
        static int constexpr attributes_size = fusion::result_of::size<Attributes>::value;

        // If you got an error here, then you are trying to pass
        // a fusion sequence with the wrong number of elements
        // as that expected by the (sequence) parser.
        static_assert(
            attributes_size >= parsers_size
          , "Size of the passed attribute is less than expected."
        );
      
        sequence_parse_fold
          ( Iterator& first
          , Iterator const& last
          , Context& context
          , RContext& rcontext
          , Parsers& parsers
          , Attributes& attributes
          )
          : _first(first)
          , _last(last)
          , _context(context)
          , _rcontext(rcontext)
          , _parsers(parsers)
          , _attributes(attributes)
          {
          ; std::cout<<":parsers_size="<<parsers_size<<";\n"
          ; std::cout<<":attributes_size="<<attributes_size<<";\n"
          ;}
          
          bool
        operator()()
          { return parse_at<0,0>()
          ;}
          
    private:
          template
          < int ParserIndex
          , int AttributeIndex
          >
          std::enable_if_t
          < ParserIndex == parsers_size
          , bool
          >
        parse_at
          (
          )
          { bool constexpr result=true
          ; std::cout<<FILE_SHORT<<':'<<__LINE__<<':'<<__func__<<"<ParserIndex == parsers_size>\n:result="<<result<<";\n"
          ; return result
          ;} 
          template
          < int ParserIndex
          , int AttributeIndex
          >
          std::enable_if_t
          < ParserIndex < parsers_size
          , bool
          >
        parse_at
          (
          )
          { 
          ; boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',__func__,"<ParserIndex < parsers_size>"))
          ; std::cout
            <<":ParserIndex="<<ParserIndex
            <<":AttributeIndex="<<AttributeIndex
            <<";\n"
          ; auto const&parser=fusion::at_c<ParserIndex>(_parsers)
          ; auto&attribute=fusion::at_c<AttributeIndex>(_attributes)
          ; std::cout<<":parser=\n"<<demangle_fmt_type(parser)<<";\n"
          ; std::cout<<":attribute=\n"<<demangle_fmt_type(attribute)<<";\n"
          ; bool result=parser.parse(_first,_last,_context,_rcontext,attribute)
          ; if(result)
            { result= parse_at<ParserIndex+1,AttributeIndex+1>()
            ;}
          ; std::cout<<":result="<<result<<";\n"
          ; return result
          ;} 
        
        Iterator& _first;
        Iterator const& _last;
        Context& _context;
        RContext& _rcontext;
        Parsers& _parsers;
        Attributes& _attributes;
    };
  #endif//defined(USE_UNFOLD_LEFT) && defined(USE_COLLAPSE_LEFT)
  
    template
  #ifdef USE_UNFOLD_LEFT
    < typename... SubParsers
  #else
    < typename Left, typename Right
  #endif
    >
    struct sequence
  #ifdef USE_UNFOLD_LEFT
    : n_ary_parser<sequence<SubParsers...>> 
  #else
    : binary_parser<Left, Right, sequence<Left, Right>>
  #endif
    {
      #ifdef USE_UNFOLD_LEFT
        typedef n_ary_parser<sequence<SubParsers...>> base_type;
        constexpr sequence(typename base_type::sub_parsers_t const& sub_parsers)
        : base_type(sub_parsers)
        {
          //std::cout<<FILE_SHORT<<':'<<__LINE__<<":"<<__func__<<":base_type="<<demangle_fmt_type<base_type>()<<";\n";
        }
      #else
        typedef binary_parser<Left, Right, sequence<Left, Right>> base_type;
        constexpr sequence(Left const&left, Right const&right)
        : base_type(left,right)
        {
          //std::cout<<FILE_SHORT<<':'<<__LINE__<<":"<<__func__<<":base_type="<<demangle_fmt_type<base_type>()<<";\n";
        }
      #endif//USE_UNFOLD_LEFT
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
                 , unused
                 )
              );
            if(!result)first = save;
            //std::cout<<"sequence:"<<__func__<<"attr=unused:result="<<result<<";\n";
            return result;
        }
    #ifdef USE_COLLAPSE_LEFT
    #else
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
    #endif//USE_COLLAPSE_LEFT
  #endif//USE_UNFOLD_LEFT    
    public:

        template <typename Iterator, typename Context
          , typename RContext>
        bool parse(
            Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, unused_type) const
        {
          #if 0
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":sequence::parse(...unused_type)"));
            std::cout<<__FILE__<<':'<<__LINE__<<':'<<__func__
              <<"\n:demangle_type(sub_parsers)=\n"<<demangle_type(base_type::sub_parsers)
              <<"\n:type(attr)=unused_type"
              <<";\n";
          #endif
          #ifdef USE_UNFOLD_LEFT
            bool result=
              parse_fold
              ( first
              , last
              , context
              , rcontext
              , base_type::indices
              );
          #else
            bool result= detail::parse_sequence(*this, first, last, context, rcontext, attr
              , typename traits::attribute_category<unused_type>::type());
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
            #ifdef USE_COLLAPSE_LEFT
                sequence_parse_fold
                < Iterator
                , Context const
                , RContext
                , decltype(this->base_type::sub_parsers)const
                , Attribute
                >
              parse_fold
                ( first
                , last
                , context
                , rcontext
                , this->base_type::sub_parsers
                , attr
                )
                ;
              bool result=
                parse_fold
                (
                );
            #else
              bool result=parse_fold
              ( first
              , last
              , context
              , rcontext
              , attr
              , base_type::indices
              );
            #endif//USE_COLLAPSE_LEFT
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
    template <typename Left, typename Right>
    constexpr auto
    operator>>(Left const& left, Right const& right)
    {
        return n_ary_make_parser<sequence>( as_parser(left), as_parser(right) );
    }
  #else
    template <typename Left, typename Right>
    constexpr sequence<
        typename extension::as_parser<Left>::value_type
      , typename extension::as_parser<Right>::value_type>
    operator>>(Left const& left, Right const& right)
    {
        return { as_parser(left), as_parser(right) };
    }
  #endif//USE_UNFOLD_LEFT

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
  #else
    template <typename Left, typename Right, typename Context>
    struct attribute_of<x3::sequence<Left, Right>, Context>
        : x3::detail::attribute_of_binary<fusion::deque, x3::sequence, Left, Right, Context> {};
  #endif//USE_UNFOLD_LEFT
}}}}
#pragma pop_macro("FILE_SHORT")
