/*=============================================================================
    Copyright (c) 2022 Larry Joe Evans
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1U_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_JAN_08_2012_0326PM)
#define BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_JAN_08_2012_0326PM
#define BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
#ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
  #define BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE "rule_defn/*/nonterminal/detail/rule_parser.hpp"
  #include <boost/utility/trace_scope.hpp>
  #include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#endif
#include <boost/utility/demangle_type.hpp>

#include <boost/core/ignore_unused.hpp>
#include <boost/spirit/home/x3/auxiliary/guard.hpp>
#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/directive/expect.hpp>
#include <boost/spirit/home/x3/nonterminal/parse_rule.hpp>
#include <boost/spirit/home/x3/core/gen_transform_parser.hpp>
#ifdef USE_RULE_INTERNAL_ID
  #include <boost/spirit/home/x3/core/rule_internal_id.hpp>
#endif  
#include <boost/utility/addressof.hpp>

#if defined(BOOST_SPIRIT_X3_DEBUG)
#include <boost/spirit/home/x3/nonterminal/simple_trace.hpp>
#endif

#include <type_traits>

namespace boost { namespace spirit { namespace x3
{
    struct parse_pass_context_tag;

namespace detail
{
#if defined(BOOST_SPIRIT_X3_DEBUG)
    template <typename Iterator, typename Attribute, typename ID>
    struct context_debug
    {
        context_debug
          ( Iterator const& first
          , Iterator const& last
          , Attribute const& attr
          , bool const& ok_parse //was parse successful?
          )
          : ok_parse(ok_parse)
          , first(first)
          , last(last)
          , attr(attr)
          , rule_id(demangle_type<ID*>())
          , f(detail::get_simple_trace())
        {
            f(first, last, attr, pre_parse, rule_id);
        }

        ~context_debug()
        {
            auto status = ok_parse ? successful_parse : failed_parse ;
            f(first, last, attr, status, rule_id);
        }

        bool const& ok_parse;
        Iterator const& first;
        Iterator const& last;
        Attribute const& attr;
        std::string rule_id;
        detail::simple_trace_type& f;
    };
#endif

    template <typename ID, typename Iterator, typename Context, typename Enable = void>
    struct has_on_error : mpl::false_ {};

    template <typename ID, typename Iterator, typename Context>
    struct has_on_error<ID, Iterator, Context,
            decltype(void(
                std::declval<ID>().on_error(
                    std::declval<Iterator&>()
                  , std::declval<Iterator>()
                  , std::declval<expectation_failure<Iterator>>()
                  , std::declval<Context>()
                )
            ))
        >
      : mpl::true_
    {};

    template <typename ID, typename Iterator, typename Attribute, typename Context, typename Enable = void>
    struct has_on_success : mpl::false_ {};

    template <typename ID, typename Iterator, typename Attribute, typename Context>
    struct has_on_success<ID, Iterator, Context, Attribute,
            decltype(void(
                std::declval<ID>().on_success(
                    std::declval<Iterator&>()
                  , std::declval<Iterator&>()
                  , std::declval<Attribute&>()
                  , std::declval<Context>()
                )
            ))
        >
      : mpl::true_
    {};
    
    #if 0 && defined(BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE)
      #define MAKE_RULE_CONTEXT_TRACE
    #endif
    template <typename ID, typename RHS, typename Context>
    Context const&
    make_rule_context
      ( RHS const& /* rhs */
      , Context const& context
      , mpl::false_ /* do_make? */
      )
      {
        #ifdef MAKE_RULE_CONTEXT_TRACE
          std::cout
            <<BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE
            <<':'<<__LINE__
            <<':'<<__func__<<"(*,not do_make?)"
            <<";\n"
            ;
        #endif
          return context;
      }

    template <typename ID, typename RHS, typename Context>
    auto make_rule_context
      ( RHS const& rhs
      , Context const& context
      , mpl::true_ /* do_make? */ 
      )
      {
        #ifdef MAKE_RULE_CONTEXT_TRACE
          std::cout
            <<BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE
            <<':'<<__LINE__
            <<':'<<__func__<<"(*,yes do_make?)"
            <<";\n"
            ;
        #endif
          return make_unique_context<ID>(rhs, context);
      }

      template 
      < typename ID
      , bool skip_definition_injection
      , typename RHS
      , typename Context
      >
    auto make_rule_context_if
      ( RHS const& rhs
      , Context const& context
      )
      {
        #ifdef MAKE_RULE_CONTEXT_TRACE
          boost::trace_scope ts
            ( stringify
              ( BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE
              , ':'
              , __LINE__
              , ':'
              , __func__
              )
            );
        #endif
          // see if the user has a BOOST_SPIRIT_DEFINE for this rule
          auto rule_id=(ID*)0;
          auto defn_v=rule_defn(rule_id);
          using defn_t=decltype(defn_v);
          
          // If there is no BOOST_SPIRIT_DEFINE for this rule,
          // we'll make a context for this rule tagged by its ID
          // so we can extract the rule later on in the default
          // (generic) parse_rule function.
          using is_defn_undefined=is_same<defn_t,detail::undefined_parser<ID>>;
          using do_make_t=
            std::conditional_t<skip_definition_injection, mpl::false_, is_defn_undefined>;
          auto constexpr do_make_v=do_make_t();
          auto made_rule_context=
            detail::make_rule_context<ID>
            ( rhs
            , context
            , do_make_v
            );
        #ifdef MAKE_RULE_CONTEXT_TRACE
          std::cout<<":rule_id="<<demangle_type(rule_id)<<";\n";
          std::cout<<":rhs="<<demangle_fmt_type(rhs)<<";\n";
          std::cout<<":skip_definition_injection="<<skip_definition_injection<<";\n";
          std::cout<<":is_defn_undefined="<<is_defn_undefined::value<<";\n";
          std::cout<<":do_make_v="<<demangle_type(do_make_v)<<";\n";
        #endif
          return made_rule_context;
      }
    template 
      < typename Attribute
      , typename ID
      , bool skip_definition_injection
      >
    struct rule_parser
    {
        template <typename Iterator, typename Context, typename ActualAttribute>
        static bool call_on_success(
            Iterator& /* before */, Iterator& /* after */
          , Context const& /* context */, ActualAttribute& /* attr */
          , mpl::false_ /* No on_success handler */ )
        {
            return true;
        }

        template <typename Iterator, typename Context, typename ActualAttribute>
        static bool call_on_success(
            Iterator& before, Iterator& after
          , Context const& context, ActualAttribute& attr
          , mpl::true_ /* Has on_success handler */)
        {
            x3::skip_over(before, after, context);
            bool pass = true;
            ID().on_success(
                before
              , after
              , attr
              , make_context<parse_pass_context_tag>(pass, context)
            );
            return pass;
        }

          template 
          < typename RHS
          , typename Iterator
          , typename Context
          , typename RContext
          , typename ActualAttribute
          >
        static bool parse_rhs_main
          ( RHS const& rhs
          , Iterator& first
          , Iterator const& last
          , Context const& context
          , RContext& rcontext
          , ActualAttribute& attr
          , mpl::false_ /* on_error not found */
          )
        {
          #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            boost::trace_scope ts
              ( stringify
                ( BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE
                , ':'
                , __LINE__
                , ':'
                , __func__
                , "(on_error=false)"
                )
              );
            std::cout<<":ID*="<<demangle_type<ID*>()<<";\n";
            std::cout<<":rhs="<<demangle_fmt_type(rhs)<<";\n";
            std::cout<<":skip_definition_injection="<<skip_definition_injection<<";\n";
            std::string input(first,last);
            std::cout<<":input="<<input<<";\n";
            std::cout<<":attr="<<demangle_fmt_type(attr)<<";\n";
          #endif//BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            Iterator start = first;
            bool r = rhs.parse
              ( first
              , last
              , context
              , rcontext
              , attr
              );
          #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            std::cout<<":r="<<r<<";\n";
          #endif//BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            if (r)
            {
                r = call_on_success(start, first, context, attr
                  , has_on_success<ID, Iterator, Context, ActualAttribute>());
            }
            return r;
        }

        template <typename RHS, typename Iterator, typename Context
          , typename RContext, typename ActualAttribute>
        static bool parse_rhs_main
          ( RHS const& rhs
          , Iterator& first
          , Iterator const& last
          , Context const& context
          , RContext& rcontext
          , ActualAttribute& attr
          , mpl::true_ /* on_error is found */
          )
        {
          #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE,':',__LINE__,':',__func__,"(on_error=true)"));
            //std::cout<<":rule_id="<<demangle_type<detail::rule_id<ID>>()<<";\n";
          #endif//BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
        
            for (;;)
            {
                try
                {
                    bool result=parse_rhs_main
                      ( rhs
                      , first
                      , last
                      , context
                      , rcontext
                      , attr
                      , mpl::false_()
                      );
                  #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
                    std::cout<<BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE<<':'<<__LINE__<<":result="<<result<<";\n";
                  #endif//BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
                    return result;
                }
                catch (expectation_failure<Iterator> const& x)
                {
                    auto error_result=ID().on_error(first, last, x, context);
                  #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
                    std::cout<<":ID()="<<demangle_type(ID())<<":\n";
                    std::cout<<":error_result="<<(unsigned)error_result<<";\n";
                  #endif//BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
                    switch (error_result)
                    {
                        case error_handler_result::fail:
                            return false;
                        case error_handler_result::retry:
                            continue;
                        case error_handler_result::accept:
                            return true;
                        case error_handler_result::rethrow:
                            throw;
                    }
                }
            }
        }

        template <typename RHS, typename Iterator
          , typename Context, typename RContext, typename ActualAttribute>
        static bool parse_rhs_main(
            RHS const& rhs
          , Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, ActualAttribute& attr)
        {
            return parse_rhs_main(
                rhs, first, last, context, rcontext, attr
              , has_on_error<ID, Iterator, Context>()
            );
        }

        template <typename RHS, typename Iterator
          , typename Context, typename RContext, typename ActualAttribute>
        static bool parse_rhs
          ( RHS const& rhs
          , Iterator& first
          , Iterator const& last
          , Context const& context
          , RContext& rcontext
          , ActualAttribute& attr
          , mpl::false_ //has_action_and_not_explicit_propagation_t
          )
        {
          #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE,':',__LINE__,':',__func__,":has_action*=false_"));
          #endif//BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            return 
              parse_rhs_main
              ( rhs
              , first
              , last
              , context
              , rcontext
              , attr
              );
        }

        template <typename RHS, typename Iterator
          , typename Context, typename RContext, typename ActualAttribute>
        static bool parse_rhs
          ( RHS const& rhs
          , Iterator& first
          , Iterator const& last
          , Context const& context
          , RContext& rcontext
          , ActualAttribute&
          , mpl::true_ //has_action_and_not_explicit_propagation_t
          )
        {
          #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE,':',__LINE__,':',__func__,":has_action*=true_"));
          #endif//BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            return 
              parse_rhs_main
              ( rhs
              , first
              , last
              , context
              , rcontext
              , unused//Attribute
              );
        }

          template 
          < typename RHS
          , typename Iterator
          , typename Context
          , typename ActualAttribute
          , typename ExplicitAttrPropagation
          >
        static bool call_rule_definition
          ( RHS const& rhs
          , Iterator& first
          , Iterator const& last
          , Context const& context
          , ActualAttribute& attr_
          , ExplicitAttrPropagation//~=force_attribute in rule<ID,Attribute,force_attribute>
          )
        {
            bool constexpr has_action_and_not_explicit_propagation_v
              =  RHS::has_action
              && !ExplicitAttrPropagation::value
              ;
            using has_action_and_not_explicit_propagation_t
              = mpl::bool_<has_action_and_not_explicit_propagation_v>
              ;
          #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE,':',__LINE__,':',__func__));
            using aof=typename traits::attribute_of<RHS,Context>::type;
            std::cout
              //<<":rule_id<ID>="<<demangle_type<rule_id<ID>>()<<";\n"
              <<":RHS="<<demangle_fmt_type<RHS>()<<";\n"
              <<":aof="<<demangle_fmt_type<aof>()<<";\n"
              <<":attr_="<<demangle_fmt_type(attr_)<<";\n"
              <<":has_action="<<RHS::has_action<<";\n"
              <<":ExplicitAttrPropagation="<<ExplicitAttrPropagation::value<<";\n"
              <<":has_action_and_not_explicit_propagation_t::value="<<has_action_and_not_explicit_propagation_t::value<<";\n"
              ;
          #endif//BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            bool ok_parse
              //Creates a place to hold the result of parse_rhs
              //called inside the following scope.
              ;
            {
             // Create a scope to cause the dbg variable below (within
             // the #if defined(USE_SPIRIT_X3_DEBUG)...#endif below) 
             // to call it's DTOR before any
             // modifications are made to the attribute passed
             // to parse_rhs (such as might be done in
             // transform::post when, for example,
             // Attribute is a recursive variant).
              #ifdef USE_RULE_INTERNAL_ID
                #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
                  boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE,':',__LINE__,':',__func__,":transform_internal_yes:"));
                #endif
                using rhs_attr=
                  typename traits::attribute_of<RHS,Context>::type
                  ;
                using transform_t=
                  traits::transform_attribute_tagged
                  < ActualAttribute
                  , rhs_attr
                  , rule_internal_id<ID>
                  >;
                transform_t transform_v;
                using transform_type=typename transform_t::type;
                transform_type rhs_actual=
                  transform_v.pre
                  ( attr_
                  );
#if defined(BOOST_SPIRIT_X3_DEBUG)
                context_debug<Iterator,transform_type,ID>
                dbg(first, last, rhs_actual, ok_parse);
#endif
                #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
                  std::cout
                    <<":rhs_actual="<<demangle_fmt_type(rhs_actual)<<";\n";
                #endif//BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
                ok_parse = parse_rhs
                  ( rhs
                  , first
                  , last
                  , context
                  , attr_//RContext&
                    //Rule Context, to allow actions to access rule attribute.
                    //see val_context in core/call.hpp.
                  , rhs_actual
                  , has_action_and_not_explicit_propagation_t()
                  );
                if(ok_parse)
                { 
                    ok_parse=transform_v.post(attr_,rhs_actual);
                }
              #else//!USE_RULE_INTERNAL_ID
                #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
                  boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_FILE,':',__LINE__,':',__func__,":transform_internal_not:"));
                #endif
#if defined(BOOST_SPIRIT_X3_DEBUG)
                context_debug<Iterator,ActualAttribute,ID>
                dbg(first, last, attr_, ok_parse);
#endif
                ok_parse = parse_rhs
                  ( rhs
                  , first
                  , last
                  , context
                  , attr_//RContext&
                    //Rule Context, to allow actions to access rule attribute.
                    //see val_context in core/call.hpp.
                  , attr_
                  , has_action_and_not_explicit_propagation_t()
                  );
              #endif//USE_RULE_INTERNAL_ID
            }//dbg variable scope(if needed)
          #ifdef BOOST_SPIRIT_X3_DETAIL_RULE_PARSER_TRACE
            std::cout<<":ok_parse="<<ok_parse<<";\n";
          #endif
            return ok_parse;
        }
    };
}}}}
#endif
