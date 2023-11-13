/*=============================================================================
    Copyright (c) 2022 Larry Joe Evans
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2012 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_NOV_9_2022_1831CST)
#define BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_NOV_9_2022_1831CST
//#define BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_TRACE
#ifdef BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_TRACE
  #define BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_FILE "make_transform_parser/*/traits/transform_attribute_tagged.hpp"
  #include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
  #include <boost/utility/trace_scope.hpp>
  #include <type_traits>
#endif//BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_TRACE
#include <boost/spirit/home/x3/core/rule_internal_id.hpp>
#include <boost/spirit/home/x3/core/rule_external_id.hpp>
#include <boost/spirit/home/x3/nonterminal/detail/transform_attribute.hpp>
namespace boost { namespace spirit { namespace x3 { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    //  transform_attribute_tagged
    //
    //  Sometimes the user needs to transform the attribute types for certain
    //  attributes. This template can be used as a customization point, where
    //  the user is able specify specific transformation rules for any attribute
    //  type.
    ///////////////////////////////////////////////////////////////////////////
      struct 
    transform_tag_default
      {};
      template
      < typename TagId
      >
      struct
    transform_tag_id
      {};
      template
      < typename Exposed 
        //Attribute type that's transformed from, then transformed to.
      , typename Transformed
        //"in between transform", i.e. used internally, i.e. not exposed.
      , typename TransformTag=transform_tag_default
        //distinguishes between transforms with same Exposed and Transformed
        //but different transformations.
        //One particulary important distinction is when TransformTag==
        //  internal_rule_id<ID>
        //Or:
        //  external_rule_id<ID>
        //where ID occurs in some rule<ID,Attribute> declaration.
      >
      struct 
    transform_attribute_tagged
    #if 1
      ;
    #else
      {
        using type=Transformed;

      #if defined(BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_TRACE)
        static void trace_tmpl(int line, std::string func="trace_tmpl")
        {
            std::cout
              <<stringify
                ( BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_FILE
                , ':', __LINE__
                , ":called from:", line
                , ':', func
                )
              <<"\n";
              ;
            std::cout<<":Exposed=\n"<<demangle_type<Exposed>()<<";\n";
            std::cout<<":Transformed=\n"<<demangle_type<Transformed>()<<";\n";
            std::cout<<":TransformTag=\n"<<demangle_type<TransformTag>()<<";\n";
            std::cout<<std::flush;
        }
      #endif//defined(BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_TRACE)
      
            static 
          type 
        pre(Exposed&) 
          { 
          #if defined(BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_TRACE)
            trace_tmpl(__LINE__,__func__);
          #endif
            return type(); 
          }
            static 
          auto
        post(Exposed& exposed, Transformed const& transformed)
          {
          #if defined(BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_TRACE)
            trace_tmpl(__LINE__,__func__);
          #endif
          //#define BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_SKIP_MOVE_TO
          #if defined(BOOST_SPIRIT_X3_TRAITS_TRANSFORM_ATTRIBUTE_TAGGED_SKIP_MOVE_TO)
          #else
            traits::move_to(transformed, exposed);
          #endif
            return true;
          }
      };
    #endif
      template
      < typename Exposed
      , typename Transformed
      , typename RuleId
      >
      struct 
    transform_attribute_tagged
      < Exposed
      , Transformed
      , rule_external_id<RuleId>
      >
      : transform_attribute<Exposed,Transformed,x3::parser_id>
      {
      };
      template 
      < typename Exposed 
      , typename Transformed
      , typename RuleId
      >
      struct 
    transform_attribute_tagged
      < Exposed
      , Transformed
      , rule_internal_id<RuleId>
        //this is used for "internal" transformation between
        //a rules attribute(Exposed) and the attribute of the rule's rhs(Transformed).
      >
      : transform_attribute<Exposed,Transformed,x3::parser_id>
      {
        using super_t=transform_attribute<Exposed,Transformed,x3::parser_id>;
        static auto post(Exposed& exposed, Transformed&& transformed)
          { super_t::post(exposed,transformed)
          ; return true
          ;}
      };
}}}}

#endif
