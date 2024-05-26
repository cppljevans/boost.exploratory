/*=============================================================================
    Copyright (c) 2022 Larry Joe Evans

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_PARSE_RULE_2023_05_03_1240)
#define BOOST_SPIRIT_X3_PARSE_RULE_2023_05_03_1240
//#define BOOST_SPIRIT_X3_PARSE_RULE_HPP_TRACE
#ifdef BOOST_SPIRIT_X3_PARSE_RULE_HPP_TRACE
  #define BOOST_SPIRIT_X3_PARSE_RULE_HPP_FILE "rule_defn/*/nonterminal/parse_rule.hpp"
#endif//BOOST_SPIRIT_X3_PARSE_RULE_HPP_TRACE
#include <boost/spirit/home/x3/nonterminal/parse_rule_rhs.hpp>
#include <boost/spirit/home/x3/nonterminal/rule_defn.hpp>
#include <boost/spirit/home/x3/nonterminal/rule_fwd.hpp>

namespace boost { namespace spirit { namespace x3
{
      template
      < typename RuleId
      , typename Iterator
      , typename Context
      , typename RuleAttribute
      >
      bool 
    parse_rule
      ( RuleId*rule_id_ptr
      , Iterator& first, Iterator const& last
      , Context const& context
      , RuleAttribute& attr
      )
      {
            auto defn_v=rule_defn(rule_id_ptr);
            using rule_t=rule<RuleId,RuleAttribute>;
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
            std::cout
              <<":rule_t=\n"<<demangle_fmt_type<rule_t>()
              <<"\n:rule_id_ptr=\n"<<demangle_fmt_type(rule_id_ptr)
              <<"\n:defn_v=\n"<<demangle_fmt_type(defn_v)
              <<";\n"
              <<std::flush
              ;
          #endif
            using defn_t=decltype(defn_v);
            bool result=false;
            using is_defn_undefined=is_same<defn_t,detail::undefined_parser<RuleId>>;
            if constexpr(is_defn_undefined::value)
            {
                auto context_rule_v=x3::get<typename rule_t::id>(context);
                using context_rule_t=decltype(context_rule_v);
                bool constexpr rule_undefined=is_same<context_rule_t, unused_type>::value;
              //#define BOOST_SPIRIT_X3_CONTEXT_RULE_UNDEFINED_TRACE
              #ifdef BOOST_SPIRIT_X3_CONTEXT_RULE_UNDEFINED_TRACE
                std::cout<<":parse_rule.hpp:"<<__LINE__<<":rule_undefined, rule_id_ptr="<<demangle_fmt_type(rule_id_ptr)<<";\n";
                return false;
              #else
                static_assert
                ( !rule_undefined
                , "use BOOST_SPIRIT_DEFINE or rule_definition to define this rule."
                );
                result=context_rule_v.parse(first, last, context, unused, attr);
              #endif
            }
            else
            {
                result=
                  parse_rule_rhs<rule_t>
                  ( first, last
                  , context
                  , attr
                  , defn_v
                  );
            }
            return result;
      }
}}}
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/variadic/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#define BOOST_SPIRIT_DEFINE_(r, data, rule_name)                               \
      auto                                                                     \
    rule_defn                                                                  \
      ( typename std::remove_cvref_t<decltype(rule_name)>::id*                 \
      )                                                                        \
      {                                                                        \
        return BOOST_JOIN(rule_name, _def);                                    \
      }                                                                        \
      /***/
#define BOOST_SPIRIT_DEFINE(...) BOOST_PP_SEQ_FOR_EACH(                        \
    BOOST_SPIRIT_DEFINE_, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))            \
    /***/

#define BOOST_SPIRIT_DECLARE_(r, data, rule_type)                              \
      template                                                                 \
      < typename Iterator                                                      \
      , typename Context                                                       \
      >                                                                        \
      bool                                                                     \
    parse_rule                                                                 \
      ( typename rule_type::id*                                                \
      , Iterator& first, Iterator const& last                                  \
      , Context const& context                                                 \
      , rule_type::attribute_type& attr                                        \
      );                                                                       \
      /***/
#define BOOST_SPIRIT_DECLARE(...) BOOST_PP_SEQ_FOR_EACH(                       \
    BOOST_SPIRIT_DECLARE_, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))           \
      /***/
#endif//include guard
