/*=============================================================================
    Copyright (c) 2022 Larry Joe Evans
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2012 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_ATTRIBUTE_TRANSFORM_NOV_9_2022_1831CST)
#define BOOST_SPIRIT_X3_ATTRIBUTE_TRANSFORM_NOV_9_2022_1831CST
#include <boost/spirit/home/x3/support/traits/move_to.hpp>
#ifdef TRANSFORM_TRACE
  #include <type_traits>
#endif//TRANSFORM_TRACE
namespace boost { namespace spirit { namespace x3 { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    //  transform_attribute
    //
    //  Sometimes the user needs to transform the attribute types for certain
    //  attributes. This template can be used as a customization point, where
    //  the user is able specify specific transformation rules for any attribute
    //  type.
    ///////////////////////////////////////////////////////////////////////////
      struct 
    transform_tag_default
      ;
      template 
      < typename Exposed 
        //Attribute type that's transformed from, then transformed to.
      , typename Transformed
        //"in between transform", i.e. used internally, i.e. not exposed.
      , typename Tag=transform_tag_default
        //distinguishes between transforms with same Exposed and Transformed
        //but different transformations.
      >
      struct 
    transform_attribute
      {
        typedef Transformed type;

        static Transformed pre(Exposed&) 
          { return Transformed(); 
          }
        static auto post(Exposed& val, Transformed&& attribute)->decltype(traits::move_to(std::forward<Transformed>(attribute), val))
          {
          #if defined(TRANSFORM_TRACE)
            std::cout<<"default_transform_attribute:"<<__func__<<":equal? not;\n";
            std::cout<<"Exposed="<<demangle_fmt_type<Exposed>();
            std::cout<<"Transformed="<<demangle_fmt_type<Transformed>();
            constexpr bool is_convertible=std::is_convertible<Exposed,Transformed>::value;
            std::cout<<"is_convertible="<<is_convertible<<";\n";
            if constexpr(is_convertible)
              traits::move_to(std::forward<Transformed>(attribute), val);
          #else
            traits::move_to(std::forward<Transformed>(attribute), val);
          #endif//TRANSFORM_TRACE
          }
      };
    // handle case where no transformation is required as the types are the same
      template 
      < typename Attribute
      , typename Tag
      >
      struct 
    transform_attribute
      < Attribute
      , Attribute
      , Tag
      >
      {
        typedef Attribute& type;

        static type pre(Attribute& val) 
          { return val;
          }
        static void post(Attribute&, Attribute const&) 
        {
          #if defined(TRANSFORM_TRACE)
            std::cout<<"transform_attribute:"<<__func__<<":equalExposedTransformed? yes;\n";
          #endif//TRANSFORM_TRACE
        }
          
      };
}}}}

#endif
