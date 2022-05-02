/*=============================================================================
    Copyright (c) 2022 Larry Joe Evans

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_MAKE_TRANSFORM_PARSER_JANUARY_01_2022_0853AM)
#define BOOST_SPIRIT_X3_MAKE_TRANSFORM_PARSER_JANUARY_01_2022_0853AM
#ifdef USE_UNFOLD_LEFT
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>

namespace boost{namespace spirit{namespace x3
{
  template<typename Parser>
using attribute_of_default=typename
  traits::attribute_of
  < Parser
  , x3::unused_type
  >::type
  ;
  struct
transform_parser_id
  ;
  template
  < typename FromParser
  , typename ToAttribute=attribute_of_default<FromParser>
  >
struct transform_parser_attribute
  ;
  template
  < typename FromParser
  , typename ToAttribute
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
      using to_attribute_type = ToAttribute;
      using from_attribute_type = attribute_of_default<FromParser>;
      static bool const has_attribute = true;
      template<typename It, typename Ctx, typename RCtx>
      bool parse(It& f, It l, Ctx&, RCtx&, ToAttribute& to_attr) const
      {
          using to_attribute_type=ToAttribute;
          using from_attribute_type=attribute_of_default<FromParser>;
          using transform = 
              traits::transform_attribute
              < to_attribute_type
              , from_attribute_type
              , transform_parser_id
              >;
          auto from_attr=transform::pre(to_attr);
          bool result=
            x3::phrase_parse(
              f,
              l,
              from_parser, x3::blank,
              from_attr);
          if(result)
          {
              transform::post(to_attr,from_attr);
          }
          return result;
      }
  };
  template
  < typename FromParser
  >
struct transform_parser_attribute
  < FromParser
  , attribute_of_default<FromParser>//ToAttribute
  >
  : x3::parser
    < transform_parser_attribute
      < FromParser
      , attribute_of_default<FromParser>
      >
    >
  //The is simply a parser identical to FromParser because
  //the ToAttribute is same as FromParser attribute.
  {
      transform_parser_attribute(FromParser p):from_parser(p){}
      FromParser from_parser;
      using from_attribute_type = attribute_of_default<FromParser>;
      using attribute_type = from_attribute_type;
      static bool const has_attribute = true;
  
      template<typename It, typename Ctx, typename RCtx>
      bool parse(It& f, It l, Ctx&, RCtx&, from_attribute_type& from_attr) const
      {
          bool result=
            x3::phrase_parse(
              f,
              l,
              from_parser, x3::blank,
              from_attr);
          return result;
      }
  };
  template
  < typename ToAttribute
  , typename FromParser
  >
  auto
make_transform_parser
  ( FromParser from_parser
  )
  {  return transform_parser_attribute<FromParser,ToAttribute>{from_parser};
  }
}}}
#endif//USE_UNFOLD_LEFT
#endif
