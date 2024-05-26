#pragma once
#include <boost/spirit/home/x3/nonterminal/detail/rule_parser_fwd.hpp>
namespace boost { namespace spirit { namespace x3
{
      template
      < typename Rule
      , typename Iterator
      , typename Context
      , typename RHS
      >
      bool 
    parse_rule_rhs
      ( Iterator& first, Iterator const& last
      , Context const& context
      , typename Rule::attribute_type& attr
      , RHS& rhs
      )
      {
          using RuleId=typename Rule::id;
          using Attribute=typename Rule::attribute_type;
        #ifdef BOOST_SPIRIT_X3_PARSE_RULE_HPP_TRACE
          boost::trace_scope ts
            ( stringify
              ( BOOST_SPIRIT_X3_PARSE_RULE_HPP_FILE
              , ':'
              , __LINE__
              , ':'
              , __func__
              )
            );
        #endif
          bool const result=
            detail::rule_parser<Attribute, RuleId, true>
            ::call_rule_definition
              ( rhs
              , first, last
              , context
              , attr
              , ::boost::mpl::bool_<Rule::force_attribute>()
              );
          return result;
      }
}}}
