/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_SEQUENCE_DETAIL_JAN_06_2013_1015AM)
#define BOOST_SPIRIT_X3_SEQUENCE_DETAIL_JAN_06_2013_1015AM
#define BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
#ifdef BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
  #define BOOST_SPIRIT_X3_SEQUENCE_DETAIL_FILE "unfold_left/*/operator/detail/sequence.hpp"
#endif

#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_category.hpp>
#include <boost/spirit/home/x3/support/traits/has_attribute.hpp>
#include <boost/spirit/home/x3/support/traits/is_substitute.hpp>
#include <boost/spirit/home/x3/support/traits/container_traits.hpp>
#include <boost/spirit/home/x3/support/traits/tuple_traits.hpp>
#include <boost/spirit/home/x3/core/detail/parse_into_container.hpp>

#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/advance.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/empty.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/iterator_range.hpp>

#include <boost/mpl/if.hpp>

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_same.hpp>

#include <iterator> // for std::make_move_iterator


namespace boost { namespace spirit { namespace x3
{
    template
    < typename... SubParsers
    >
    struct sequence;
}}}

namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename Parser, typename Context, typename Enable = void>
    struct sequence_size
    {
        static int const value = traits::has_attribute<Parser, Context>::value;
    };

    template <typename Parser, typename Context>
    struct sequence_size_subject
      : sequence_size<typename Parser::subject_type, Context> {};

    template <typename Parser, typename Context>
    struct sequence_size<Parser, Context
      , typename enable_if_c<(Parser::is_pass_through_unary)>::type>
      : sequence_size_subject<Parser, Context> {};

    template <typename... SubParsers, typename Context>
    struct sequence_size<sequence<SubParsers...>, Context>
    {
        static int const value =
          ( 0  
          + ...
          + sequence_size<SubParsers, Context>::value
          );
    };

    struct pass_sequence_attribute_unused
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
    struct pass_sequence_attribute_size_one_view
    {
          typedef typename fusion::result_of::deref
          < typename fusion::result_of::begin<Attribute>::type
          >::type 
        type;

        static type call(Attribute& attribute)
        {
            return fusion::deref(fusion::begin(attribute));
        }
    };

    template <typename Attribute>
    struct pass_through_sequence_attribute
    {
        typedef Attribute& type;

        template <typename Attribute_>
        static Attribute_&
        call(Attribute_& attribute)
        {
            return attribute;
        }
    };

    template <typename Parser, typename Attribute, typename Enable = void>
    struct pass_sequence_attribute
      : mpl::if_
        < traits::is_size_one_view<Attribute>
        , pass_sequence_attribute_size_one_view<Attribute>
        , pass_through_sequence_attribute<Attribute>
        >::type 
    {};

    template <typename... SubParsers, typename Attribute>
    struct pass_sequence_attribute
      < sequence<SubParsers...>
      , Attribute
      >
      : pass_through_sequence_attribute<Attribute> 
    {};

    template <typename Parser, typename Attribute>
    struct pass_sequence_attribute_subject
      : pass_sequence_attribute<typename Parser::subject_type, Attribute> 
    {};

    template <typename Parser, typename Attribute>
    struct pass_sequence_attribute
      < Parser
      , Attribute
      , typename enable_if_c<(Parser::is_pass_through_unary)>::type
      >
      : pass_sequence_attribute_subject<Parser, Attribute> 
    {};

    
      template
      < typename SequenceMaybe
      >
      struct
    make_sequence
      {
            static
          bool constexpr
        is_seq=
          fusion::traits::is_sequence<SequenceMaybe>::type::value
          ;
          using
        type=
          std::conditional_t
          < is_seq
          , SequenceMaybe&
          , fusion::deque<SequenceMaybe&>
          >
          ;
            static
          type
        make
          ( SequenceMaybe& x
          )
          {
          ; if constexpr(is_seq)
            { return x
            ;}
            else
            { return type(x)
            ;}
          ;}
      };
      template
      < typename Iterator
      , typename Context
      , typename RContext
      , typename Parsers
      , typename Attributes
      >
      struct 
    sequence_parse_fold
      {
        using attr_category = typename traits::attribute_category<Attributes>::type;
        //static_assert(is_same<traits::tuple_attribute, attr_category>::value,"The parser expects tuple-like attribute type");
            
        using parsers_type=std::remove_const_t<Parsers>;
        using parsers_attribute=typename traits::attribute_of<parsers_type,Context>::type;
        using parsers_seq=typename make_sequence<parsers_attribute>::type;
        using parsers_refless=std::remove_cvref_t<parsers_seq>;
        static int constexpr parsers_size = fusion::result_of::size<parsers_refless>::value;
        using attributes_seq=typename make_sequence<Attributes>::type;
        using attributes_refless=std::remove_cvref_t<attributes_seq>;
        static int constexpr attributes_size = fusion::result_of::size<attributes_refless>::value;

      #define SEQUENCE_PARSE_FOLD_TRACE
      #ifndef SEQUENCE_PARSE_FOLD_TRACE
        // If you got an error here, then you are trying to pass
        // a fusion sequence with the wrong number of elements
        // as that expected by the (sequence) parser.
        static_assert(
            attributes_size >= parsers_size
          , "Size of the passed attribute is less than expected."
        );
      #endif
      
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
          , _attributes
            ( make_sequence<Attributes>::make(attributes)
            )
          {
          ; std::cout<<":parsers_size="<<parsers_size<<";\n"
          ; std::cout<<":attributes_size="<<attributes_size<<";\n"
          ;}
          
          bool
        parse()
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
          ; std::cout<<BOOST_SPIRIT_X3_SEQUENCE_FILE<<':'<<__LINE__<<':'<<__func__<<"<ParserIndex == parsers_size>\n:result="<<result<<";\n"
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
          ; boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_SEQUENCE_FILE,':',__LINE__,':',__func__,"<ParserIndex < parsers_size>"))
          ; std::cout
            <<":ParserIndex="<<ParserIndex
            <<":AttributeIndex="<<AttributeIndex
            <<";\n"
          ; using parser_type=typename fusion::result_of::value_at_c<Parsers,ParserIndex>::type
          ; parser_type const&parser=fusion::at_c<ParserIndex>(_parsers)
          ; using parser_attr=typename traits::attribute_of<parser_type,Context>::type
          ; bool constexpr is_unused=std::is_same_v<unused_type,parser_attr>
          ; std::cout<<":is_unused="<<is_unused<<";\n"
          ; bool result=false
          ; if constexpr(is_unused)
            { result=parser.parse(_first,_last,_context,_rcontext,unused)
            ; if(result)
              { result= parse_at<ParserIndex+1,AttributeIndex>()
              ;}
            ;}
            else
            {
            ; auto&attribute=fusion::at_c<AttributeIndex>(_attributes)
            ; std::cout<<":attribute=\n"<<demangle_fmt_type(attribute)<<";\n"
            ; result=parser.parse(_first,_last,_context,_rcontext,attribute)
            ; if(result)
              { result= parse_at<ParserIndex+1,AttributeIndex+1>()
              ;}
            ;}
          ; std::cout<<":result="<<result<<";\n"
          ; return result
          ;} 
        
        Iterator& _first;
        Iterator const& _last;
        Context& _context;
        RContext& _rcontext;
        Parsers& _parsers;
        typename make_sequence<Attributes>::type _attributes;
      };
      
    template <typename Parser, typename Iterator, typename Context
      , typename RContext, typename Attribute, typename AttributeCategory>
    bool parse_sequence(
        Parser const& parser, Iterator& first, Iterator const& last
      , Context const& context, RContext& rcontext, Attribute& attr
      , AttributeCategory//ignored
      )
    {
      #ifdef BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
        boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_SEQUENCE_DETAIL_FILE,':',__LINE__,':',__func__,"(...,AttributeCategory//ignored)"));
        using aof_type=typename traits::attribute_of<Parser,Context>::type;
        std::cout<<"demangle_fmt_type<Parser>()=\n"<<demangle_fmt_type<Parser>()<<";\n";
        std::cout<<"demangle_fmt_type<aof_type>()=\n"<<demangle_fmt_type<aof_type>()<<";\n";
        std::cout<<"demangle_fmt_type<Attribute>()=\n"<<demangle_fmt_type<Attribute>()<<";\n";
        //std::cout<<"demangle_fmt_type<RContext>()=\n"<<demangle_fmt_type<RContext>()<<";\n";
      #endif//BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE

        Iterator save = first;
        
          sequence_parse_fold
          < Iterator
          , Context const
          , RContext
          , typename Parser::sub_parsers_t const
          , Attribute
          >
        parse_fold
          ( first
          , last
          , context
          , rcontext
          , parser.sub_parsers
          , attr
          )
          ;
        bool result=
          parse_fold.parse
          (
          );
        if(!result) first = save;
        return result;
    }

    template <typename Parser, typename Context>
    constexpr bool pass_sequence_container_attribute
        = sequence_size<Parser, Context>::value > 1;

    template <typename Parser, typename Iterator, typename Context
      , typename RContext, typename Attribute>
    typename enable_if_c<pass_sequence_container_attribute<Parser, Context>, bool>::type
    parse_sequence_container(
        Parser const& parser
      , Iterator& first, Iterator const& last, Context const& context
      , RContext& rcontext, Attribute& attr)
    {
      #ifdef BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
        boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_SEQUENCE_DETAIL_FILE,':',__LINE__,':',__func__,":enable_if_c<pass_sequence_container_attribute>"));
      #endif
        bool result=parser.parse(first, last, context, rcontext, attr);
      #ifdef BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
        std::cout<<":result="<<result<<";\n";
      #endif
        return result;
    }

    template <typename Parser, typename Iterator, typename Context
      , typename RContext, typename Attribute>
    typename disable_if_c<pass_sequence_container_attribute<Parser, Context>, bool>::type
    parse_sequence_container(
        Parser const& parser
      , Iterator& first, Iterator const& last, Context const& context
      , RContext& rcontext, Attribute& attr)
    {
      #ifdef BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
        boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_SEQUENCE_DETAIL_FILE,':',__LINE__,':',__func__,":disable_if_c<pass_sequence_container_attribute>"));
      #endif
        bool result=parse_into_container(parser, first, last, context, rcontext, attr);
      #ifdef BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
        std::cout<<":result="<<result<<";\n";
      #endif
        return result;
    }

    template <typename Parser, typename Iterator, typename Context
      , typename RContext, typename Attribute>
    bool parse_sequence(
        Parser const& parser , Iterator& first, Iterator const& last
      , Context const& context, RContext& rcontext, Attribute& attr
      , traits::container_attribute)
    {
      #ifdef BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
        boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_SEQUENCE_DETAIL_FILE,':',__LINE__,':',__func__,"(...,container_attribute)"));
        std::cout<<"Parser=\n"<<demangle_fmt_type<Parser>()<<";\n";
        std::cout<<"Attribute=\n"<<demangle_fmt_type<Attribute>()<<";\n";
      #endif
        Iterator save = first;
        auto parses_into_container=
          [&]
          < std::size_t... Indices
          >
          ( std::integer_sequence< std::size_t, Indices...>)
          { return
            (  true
            && ...
            && detail::parse_sequence_container
               ( parser.template sub_parser<Indices>()
               , first
               , last
               , context
               , rcontext
               , attr
               )
            );
          };
        bool result=parses_into_container(Parser::indices);
        #ifdef BOOST_SPIRIT_X3_ALTERNATIVE_DETAIL_TRACE
          std::cout<<":result="<<result<<";\n";
        #endif
        if(!result) first = save;
        return result;
    }

    template <typename Parser, typename Iterator, typename Context
      , typename RContext, typename Attribute>
    bool parse_sequence_assoc(
        Parser const& parser , Iterator& first, Iterator const& last
	  , Context const& context, RContext& rcontext, Attribute& attr, mpl::false_ /*should_split*/)
    {
	    return parse_into_container(parser, first, last, context, rcontext, attr);
    }

    template <typename Parser, typename Iterator, typename Context
      , typename RContext, typename Attribute>
    bool parse_sequence_assoc(
        Parser const& parser , Iterator& first, Iterator const& last
	  , Context const& context, RContext& rcontext, Attribute& attr, mpl::true_ /*should_split*/)
    {
        Iterator save = first;
        if (parser.left.parse( first, last, context, rcontext, attr)
            && parser.right.parse(first, last, context, rcontext, attr))
            return true;
        first = save;
        return false;
    }

    template <typename Parser, typename Iterator, typename Context
      , typename RContext, typename Attribute>
    bool parse_sequence(
        Parser const& parser, Iterator& first, Iterator const& last
      , Context const& context, RContext& rcontext, Attribute& attr
      , traits::associative_attribute)
    {
        // we can come here in 2 cases:
        // - when sequence is key >> value and therefore must
        // be parsed with tuple synthesized attribute and then
        // that tuple is used to save into associative attribute provided here.
        // Example:  key >> value;
        //
        // - when either this->left or this->right provides full key-value
        // pair (like in case 1) and another one provides nothing.
        // Example:  eps >> rule<class x; fusion::map<...> >
        //
        // first case must be parsed as whole, and second one should
        // be parsed separately for left and right.

        typedef typename traits::attribute_of<
            decltype(parser.left), Context>::type l_attr_type;
        typedef typename traits::attribute_of<
            decltype(parser.right), Context>::type r_attr_type;

        typedef typename
            mpl::or_<
                is_same<l_attr_type, unused_type>
              , is_same<r_attr_type, unused_type> >
        should_split;

      #ifdef BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
        boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_SEQUENCE_DETAIL_FILE,':',__LINE__,':',__func__,""));
        std::cout<<"should_split="<<should_split::value<<";\n";
      #endif
        return parse_sequence_assoc(parser, first, last, context, rcontext, attr
          , should_split());
    }

    template <typename Left, typename Right, typename Context, typename RContext>
    struct parse_into_container_impl
    < sequence<Left, Right>
    , Context
    , RContext
    >
    {
        static auto this_type_name()
        { return std::string {"parse_into_container_impl<sequence<L,R>>"};
        }
        
        typedef sequence<Left, Right> parser_type;

        template <typename Iterator, typename Attribute>
        static bool call(
            parser_type const& parser
          , Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attr
          , mpl::false_
            //parser attribute cannot substitute for 
            //container attr's contained value
          )
        {
            // inform user what went wrong if we jumped here in attempt to
            // parse incompatible sequence into fusion::map
            static_assert(!is_same< typename traits::attribute_category<Attribute>::type,
                  traits::associative_attribute>::value,
                  "To parse directly into fusion::map sequence must produce tuple attribute "
                  "where type of first element is existing key in fusion::map and second element "
                  "is value to be stored under that key");

            Attribute attr_{};
          #ifdef BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_SEQUENCE_DETAIL_FILE,':',__LINE__,':',this_type_name(),"::",__func__,"(...,is_sub=false_)"));
          #endif
            bool result=parse_sequence(parser, first, last, context, rcontext, attr_, traits::container_attribute());
            if (result)
            {
                traits::append(attr, std::make_move_iterator(traits::begin(attr_)),
                                 std::make_move_iterator(traits::end(attr_)));
            }
            return result;
        }

        template <typename Iterator, typename Attribute>
        static bool call(
            parser_type const& parser
          , Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attr
          , mpl::true_//parser attribute can substitute for attr.
          )
        {
          #ifdef BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_SEQUENCE_DETAIL_FILE,':',__LINE__,':',this_type_name(),"::",__func__,"(...,is_sub=true_)"));
          #endif
            return parse_into_container_base_impl<parser_type>::call(
                parser, first, last, context, rcontext, attr);
        }

        template <typename Iterator, typename Attribute>
        static bool call(
            parser_type const& parser
          , Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attr)
        {
            typedef typename
                traits::attribute_of<parser_type, Context>::type
            aof_type;

            typedef typename
                traits::container_value<Attribute>::type
            container_value_type;

          #ifdef BOOST_SPIRIT_X3_SEQUENCE_DETAIL_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_SEQUENCE_DETAIL_FILE,':',__LINE__,':',this_type_name(),"::",__func__,"(...)"));
            //std::cout<<"parser_type="<<demangle_fmt_type<parser_type>()<<";\n";
            std::cout<<"attribute_of<parser_type>=aof_type="<<demangle_fmt_type<aof_type>()<<";\n";
            std::cout<<"container_value_type="<<demangle_fmt_type<container_value_type>()<<";\n";
          #endif
            using is_subs_aof_in_container=typename traits::is_substitute<aof_type, container_value_type>
              //is parser attribute a substitute for container value?
              ;
          #ifdef BOOST_SPIRIT_X3_IS_SUBSTITUTE_TRACE
            is_subs_aof_in_container::trace_tmpl();
          #endif
            auto constexpr is_sub_type=typename is_subs_aof_in_container::type();
            return call(parser, first, last, context, rcontext, attr, is_sub_type);
        }
    };

}}}}

#endif
