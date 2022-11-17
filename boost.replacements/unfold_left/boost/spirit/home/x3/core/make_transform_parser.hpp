/*=============================================================================
    Copyright (c) 2022 Larry Joe Evans

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_MAKE_TRANSFORM_PARSER_JANUARY_01_2022_0853AM)
#define BOOST_SPIRIT_X3_MAKE_TRANSFORM_PARSER_JANUARY_01_2022_0853AM
#ifdef USE_UNFOLD_LEFT
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/transform_attribute.hpp>
#include <boost/spirit/home/x3/core/parse.hpp>

namespace boost{namespace spirit{namespace x3{namespace traits
{
  template
  < typename FromParser
  , typename ToAttribute
  , typename TransformTag=transform_tag_default
  >
struct transform_parser_attribute
  : x3::parser
    < transform_parser_attribute
      < FromParser
      , ToAttribute
      >
    >
  //A parser using FromParser, but transforming FromParser's attribute 
  //to a ToAttribute.
  {
      transform_parser_attribute(FromParser p):from_parser(p){}
      FromParser from_parser;
      using attribute_type = ToAttribute;
      static bool const has_attribute = true;
      template<typename It, typename Ctx>
      bool parse(It& f, It l, Ctx&context, ToAttribute& to_attr) const
      {
          using to_attribute_type=ToAttribute;
          using from_attribute_type=typename attribute_of<FromParser,Ctx>::type;
          using transform = 
              traits::transform_attribute
              < to_attribute_type
              , from_attribute_type
              , TransformTag
              >;
          auto const& skipper = x3::get<skipper_tag>(context);
          auto from_attr=transform::pre(to_attr);
          bool result=
            x3::phrase_parse
            ( f
            , l
            , from_parser 
            , skipper
            , from_attr
            );
          if(result)
          {
              transform::post(to_attr,from_attr);
          }
          return result;
      }
  };
  template
  < typename ToAttribute
  , typename FromParser
  , typename TransformTag=transform_tag_default
  >
  auto
make_transform_parser
  ( FromParser from_parser
  )
  {  return transform_parser_attribute<FromParser,ToAttribute,TransformTag>{from_parser};
  }
}}}}
#endif//USE_UNFOLD_LEFT
#endif
