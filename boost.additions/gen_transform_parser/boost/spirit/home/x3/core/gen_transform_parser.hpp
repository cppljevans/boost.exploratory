/*=============================================================================
    Copyright (c) 2022 Larry Joe Evans

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_JANUARY_01_2022_0853AM)
#define BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_JANUARY_01_2022_0853AM
//#define BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
#ifdef BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
  #define BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_FILE "gen_transform_parser/*/core/gen_transform_parser.hpp"
  #include <boost/spirit/home/x3/support/traits/print_attribute.hpp>
#endif
#if defined(USE_DEBUG)||defined(USE_RULE_DEFN)
  #include <boost/spirit/home/x3/nonterminal/rule_fwd.hpp>
  #include <boost/spirit/home/x3/nonterminal/detail/rule_id.hpp>
#endif
#include <boost/spirit/home/x3/support/traits/transform_attribute_tagged.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/core/parse.hpp>

namespace boost{namespace spirit{namespace x3{namespace traits
{
  template
  < typename Derived
  , typename ToAttribute
  , typename TransformTag
  >
struct transform_parser_attribute_crtp
  : x3::parser
    < transform_parser_attribute_crtp
      < Derived
      , ToAttribute
      , TransformTag
      >
    >
  {
        auto&
      derived()
        { return static_cast<Derived&>(*this)
        ;}
        auto const&
      derived()const
        { return static_cast<Derived const&>(*this)
        ;}
        template
        < typename Iterator
        , typename Ctx
        , typename Rctx
        >
        bool 
      parse
        ( Iterator& f, Iterator l
        , Ctx&context
        , Rctx&r_ctx//rule context, i.e. the attribute of the calling rule.
        , ToAttribute& to_attr
        ) const
        {
        ; auto from_parser_v=static_cast<Derived const*>(this)->parser()
        ; using from_parser_t=std::remove_const_t<decltype(from_parser_v)>
        ; using from_attribute_type=typename attribute_of<from_parser_t,Ctx>::type
        ;
          using transform = 
              traits::transform_attribute_tagged
              < ToAttribute
              , from_attribute_type
              , TransformTag
              >;
          using transform_type=typename transform::type;
        #ifdef BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
          boost::trace_scope ts
            ( stringify
              ( BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_FILE
              , ':', __LINE__
              , ':', __func__,"(,Ctx&,Rctx&,ToAttribute&)"
              )
            );
          std::cout<<":from_parser_t=\n"<<demangle_fmt_type<from_parser_t>()<<";\n";
          std::cout<<":ToAttribute=\n"<<demangle_fmt_type<ToAttribute>()<<";\n";
          std::cout<<":TransformTag=\n"<<demangle_fmt_type<TransformTag>()<<";\n";
          std::cout<<":from_attribute_type=\n"<<demangle_fmt_type<from_attribute_type>()<<";\n";          
          std::cout<<":transform_type=\n"<<demangle_fmt_type<transform_type>()<<";\n";
          //transform::trace_tmpl();
        #endif//BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
            transform_type 
          from_attr
            =transform::pre
             ( to_attr
           #ifdef USE_TRANSFORM_ATTRIBUTE_TAGGED_INHERITED
             , r_ctx 
               //this allows construction of from_attr to
               //inherit from r_ctx, which is the attribute of
               //calling rule, if it exists.
           #endif
             );
          bool result=
            derived().parse_from
            ( f, l
            , context
            , r_ctx
            , from_attr
            );
        #ifdef BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
          std::cout<<BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_FILE<<':'<<__LINE__<<":result.parse="<<result<<";\n";
        #endif//BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
          if(result)
          {
              result=transform::post(to_attr,from_attr);
            #ifdef BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
              std::cout<<BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_FILE<<':'<<__LINE__<<":result.post="<<result<<";\n";
              std::cout<<":input="<<std::string(f,l)<<";\n";
              //std::cout<<":to_attr="<<to_attr<<";\n";
            #endif//BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
          }
          return result;
        }
  };
  template
  < typename FromParser
  , typename ToAttribute
  , typename TransformTag=transform_tag_default
  >
struct transform_parser_attribute
  : transform_parser_attribute_crtp
    < transform_parser_attribute
      < FromParser
      , ToAttribute
      , TransformTag
      >
    , ToAttribute
    , TransformTag
    >
  //A parser using FromParser, but transforming FromParser's attribute 
  //to a ToAttribute.
  {
      using attribute_type=ToAttribute;
      FromParser from_parser;
      auto parser()const
        { return from_parser
        ;}
        template
        < typename Iterator
        , typename Ctx
        , typename Rctx
        , typename Attribute
        >
        bool 
      parse_from
        ( Iterator& f, Iterator l
        , Ctx&context
        , Rctx&r_ctx//rule context, i.e. the attribute of the calling rule.
        , Attribute& attr
        ) const
        {
        #if 1 && defined(BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE)
          boost::trace_scope ts
          ( stringify
            ( BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_FILE
            , ':', __LINE__
            , ':', __func__
            )
          );
          std::cout<<":*this="<<demangle_fmt_type<transform_parser_attribute>()<<";\n";
          std::cout<<":Attribute=\n"<<demangle_fmt_type<Attribute>()<<";\n";
        #endif//BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
        ; bool result=from_parser.parse(f,l,context,r_ctx,attr);
        #if 1 && defined(BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE)
          std::cout<<":result="<<result<<";\n"
        #endif
        ; return result;
        ;}
      transform_parser_attribute(FromParser p)
        : from_parser(p)
        {
          #if 0 && defined(BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE)
            boost::trace_scope ts
            ( stringify
              ( BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_FILE
              , ':', __LINE__
              , ':', __func__,"(FromParser p)"
              )
            );
            std::cout<<":*this="<<demangle_fmt_type<transform_parser_attribute>()<<";\n";
          #endif//BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
        }
  };
struct ident_transform
  {};
  template
  < typename FromParser
  , typename ToAttribute
  >
struct transform_parser_attribute
  < FromParser
  , ToAttribute
  , ident_transform
  > 
  : x3::parser
    < transform_parser_attribute
      < FromParser
      , ToAttribute
      , ident_transform
      >
    >
  {
  private:
      FromParser from_parser;
  public:
      using attribute_type = ToAttribute;
      transform_parser_attribute(FromParser p)
        : from_parser(p)
        {
          #if 0 && defined(BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE)
            boost::trace_scope ts
            ( stringify
              ( BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_FILE
              , ':', __LINE__
              , ':', __func__,"(FromParser p)"
              )
            );
            std::cout<<":*this="<<demangle_fmt_type<transform_parser_attribute>()<<";\n";
          #endif//BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
        }
        template
        < typename Iterator
        , typename Ctx
        , typename Rctx
        >
        bool 
      parse
        ( Iterator& f, Iterator l
        , Ctx&context
        , Rctx&r_ctx
        , ToAttribute& to_attr
        ) const
        { 
          #ifdef BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
            boost::trace_scope ts
              ( stringify
                ( BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_FILE
                , ':', __LINE__
                , ':', __func__,"(,Ctx&,Rctx&,ToAttribute&)"
                )
              );
            std::cout<<":FromParser="<<demangle_fmt_type<FromParser>()<<";\n";
            std::cout<<":ToAttribute="<<demangle_fmt_type<ToAttribute>()<<";\n";
            std::cout<<":TransformTag="<<demangle_fmt_type<ident_transform>()<<";\n";
            std::cout<<":Ctx="<<demangle_fmt_type<Ctx>()<<";\n";
            std::cout<<":Rctx="<<demangle_fmt_type<Rctx>()<<";\n";
          #endif//BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
        ; bool result=from_parser.parse(f,l,context,r_ctx,to_attr)
          #ifdef BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
          ; std::cout<<BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_FILE<<':'<<__LINE__<<":result.parse="<<result<<";\n";
            std::cout<<":input="<<std::string(f,l)<<";\n";
            std::cout<<":to_attr=";
            traits::print_attribute(std::cout,to_attr);
            std::cout<<";\n";
          #endif//BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE
        ; return result
        ;}
  };
  template
  < typename ToAttribute
  , typename FromParser
  , typename TransformTag=transform_tag_default
  >
  auto
gen_transform_parser
  ( FromParser from_parser
  )
  {  return transform_parser_attribute<FromParser,ToAttribute,TransformTag>{from_parser};
  }
//{as_defn
//the following copy&pasted with only minor modifications from sehe's 'as' 
// alias template(https://en.cppreference.com/w/cpp/language/type_alias)
// here:
//  https://stackoverflow.com/questions/56819120/spirit-x3-how-to-get-attribute-type-to-match-rule-type?rq=1
//=========
      template
      < typename ToAttribute
      , typename TransformTag=transform_tag_default
      >
      struct 
    as_defn_type
      {
            template 
            < typename FromParser
            >
            constexpr auto 
          operator[](FromParser e) const 
            { return 
                x3::rule<TransformTag, ToAttribute> {} = e
                //rule_definition<TransformTag,FromParser,ToAttribute>
                ;
            }
      };
        template
        < typename ToAttribute
        , typename TransformTag=transform_tag_default
        >
        static inline constexpr 
      as_defn_type
        < ToAttribute
        , TransformTag
        > 
    as_defn
      ;  
//}as_defn
      template 
      < typename ToAttribute
      , typename TransformTag=transform_tag_default
      >
      struct 
    as_attr_type 
      {
            template <typename Parser>
            constexpr auto 
          operator[](Parser e) const 
            { 
            #if 0 && defined(BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_TRACE)
              boost::trace_scope ts
                ( stringify
                  ( BOOST_SPIRIT_X3_CORE_GEN_TRANSFORM_PARSER_FILE
                  , ':', __LINE__
                  , ':', __func__,"(Parser e)"
                  )
                );
              std::cout<<":ToAttribute=\n"<<demangle_fmt_type<ToAttribute>()<<";\n";
              std::cout<<":Parser=\n"<<demangle_fmt_type<Parser>()<<";\n";
            #endif
              return 
                gen_transform_parser
                < ToAttribute
                , Parser
                , TransformTag
                >
                ( e
                );
            }
      };
        template
        < typename ToAttribute
        , typename TransformTag=transform_tag_default
        >
        static inline constexpr 
      as_attr_type
        < ToAttribute
        , TransformTag
        > 
    as_attr
      ;
      template 
      < typename ToAttribute
      >
      struct 
    as_ident_type 
      {
            template <typename Parser>
            constexpr auto 
          operator[](Parser e) const 
            { return 
                gen_transform_parser
                < ToAttribute
                , Parser
                , ident_transform
                >
                ( e
                );
            }
      };
        template 
        < typename ToAttribute
        >
        static inline constexpr 
      as_ident_type
        < ToAttribute
        > 
    as_ident
      ;  
      template
      < typename TransformTag=ident_transform
      >
      struct 
    as_aof_ident_type 
      {
            template <typename FromParser>
            constexpr auto 
          operator[](FromParser e) const 
            { using ToAttribute=typename
                attribute_of
                < FromParser
                , x3::unused_type
                >::type
                ;
              return 
                gen_transform_parser
                < ToAttribute
                , FromParser
                , TransformTag
                >
                ( e
                );
            }
      };
        static inline constexpr 
      as_aof_ident_type
    as_aof_ident
      ;  
//}the following copy&pasted...
}//traits
}//x3
}//spirit
}//boost
#endif
