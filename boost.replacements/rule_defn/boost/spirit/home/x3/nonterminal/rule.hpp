/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_RULE_JAN_08_2012_0326PM)
#define BOOST_SPIRIT_X3_RULE_JAN_08_2012_0326PM
#define BOOST_SPIRIT_X3_RULE_TRACE
#ifdef BOOST_SPIRIT_X3_RULE_TRACE
  #define BOOST_SPIRIT_X3_RULE_FILE "rule_defn/*/nonterminal/rule.hpp"
#endif
#define BOOST_SPIRIT_X3_RULE_GEN_FLAG
#if defined(BOOST_SPIRIT_X3_RULE_GEN_FLAG) && defined(BOOST_SPIRIT_X3_RULE_TRACE)
  #define BOOST_SPIRIT_X3_RULE_GEN_TRACE 
#endif

#include <boost/spirit/home/x3/nonterminal/detail/rule_parser.hpp>
#include <boost/spirit/home/x3/core/rule_external_id.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/spirit/home/x3/support/context.hpp>
#include <type_traits>

#if !defined(BOOST_SPIRIT_X3_NO_RTTI)
#include <typeinfo>
#endif

namespace boost { namespace spirit { namespace x3
{
      using
    rule_name_type=
      char const*
      ;
      template
      < typename ID
      , typename RHS
      , typename Attribute
      , bool force_attribute_
      , bool skip_definition_injection = false
      >
    struct rule_definition 
      : parser
        < rule_definition
          < ID
          , RHS
          , Attribute
          , force_attribute_
          , skip_definition_injection
          >
        >
    {
        typedef rule_definition<ID, RHS, Attribute, force_attribute_, skip_definition_injection> this_type;
        typedef ID id;
        typedef RHS rhs_type;
        typedef rule<ID, Attribute, force_attribute_> lhs_type;
        typedef Attribute attribute_type;

        static bool const has_attribute =
            !is_same<Attribute, unused_type>::value;
        static bool const handles_container =
            traits::is_container<Attribute>::value;
        static bool const force_attribute =
            force_attribute_;

        constexpr rule_definition(RHS const& rhs, rule_name_type name)
          : rhs(rhs), name(name) 
          {
          #ifdef BOOST_SPIRIT_X3_RULE_GEN_TRACE
            std::cout
              <<BOOST_SPIRIT_X3_RULE_FILE
              <<':'<<__LINE__
              <<':'<<__func__
              <<":ID*="<<demangle_fmt_type<ID*>()<<'\n'
              <<":name="<<name<<'\n'
              <<":force_attribute="<<force_attribute<<'\n'
              <<":skip_definition_injection="<<skip_definition_injection
              <<";\n"
              ;
          #endif
          }

        template <typename Iterator, typename Context, typename Attribute_>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, unused_type, Attribute_& attr_) const
        {
          #ifdef BOOST_SPIRIT_X3_RULE_TRACE
            boost::trace_scope ts
            ( stringify
              ( BOOST_SPIRIT_X3_RULE_FILE
              , ':'
              , __LINE__
              , ':'
              , __func__
              , "(...,Attribute_&)const"
              )
            );
            std::cout
              <<":*this=\n"
                <<demangle_fmt_type<rule_definition>()
                <<";\n"
              <<":attribute_type=\n"
                <<demangle_fmt_type<attribute_type>()
                <<";\n"
              <<":Attribute_=\n"
                <<demangle_fmt_type<Attribute_>()
                <<";\n"
              ;
          #endif
            using transform = 
                traits::transform_attribute_tagged
                < Attribute_
                , attribute_type
                , rule_external_id<ID>
                >
                ;
            using transform_attr = typename transform::type;
            transform_attr tr_attr = transform::pre(attr_);
          #ifdef BOOST_SPIRIT_X3_RULE_TRACE
            std::cout
              <<BOOST_SPIRIT_X3_RULE_FILE
              <<':'<<__LINE__<<";\n"
              <<":transform=\n"<<demangle_fmt_type<transform>()<<";\n"
              <<":transform_attr=\n"<<demangle_fmt_type<transform_attr>()<<";\n"
              ;
          #endif
          #if 0
            bool const result=false;
          #else
            auto maybe_rule_context=detail::make_rule_context_if<ID,skip_definition_injection>(rhs,context);
            bool const result=detail::rule_parser<attribute_type, ID, skip_definition_injection>
                ::call_rule_definition
                  ( rhs, first, last
                  , maybe_rule_context
                  , tr_attr
                  , mpl::bool_<force_attribute>()
                  );
          #endif
            if(result)
            {
                transform::post(attr_, tr_attr);
            }
          #ifdef BOOST_SPIRIT_X3_RULE_TRACE
            std::cout<<":result="<<result<<";\n";
            std::cout<<":input="<<std::string(first,last)<<";\n";
            #ifdef WORK_AROUND_DEBUG_CPP_TRACE
              std::cout<<":attr=";
              print_vec(std::cout,attr)<<";\n";
            #endif
          #endif
            return result;
        }

        RHS rhs;
        rule_name_type name;
    };

      template
      < typename ID
      , typename Attribute
      , bool force_attribute_
      >
    struct rule : parser<rule<ID, Attribute, force_attribute_>>
    {
        static_assert(!std::is_reference<Attribute>::value,
                      "Reference qualifier on rule attribute type is meaningless");

        typedef ID id;
        typedef Attribute attribute_type;
        static bool const has_attribute =
            !std::is_same<std::remove_const_t<Attribute>, unused_type>::value;
        static bool const handles_container =
            traits::is_container<Attribute>::value;
        static bool const force_attribute = force_attribute_;

#if !defined(BOOST_SPIRIT_X3_NO_RTTI)
        rule() 
          : name
            ( typeid(rule).name()
            ) 
            {}
#else
        constexpr rule() : name("unnamed") {}
#endif

        constexpr rule(rule_name_type name)
          : name(name) {}

        constexpr rule(rule const& r)
          : name(r.name)
        {
            // Assert that we are not copying an unitialized static rule. If
            // the static is in another TU, it may be initialized after we copy
            // it. If so, its name member will be nullptr.
            BOOST_ASSERT_MSG(r.name, "uninitialized rule"); // static initialization order fiasco
        }

        template <typename RHS>
        #ifndef BOOST_SPIRIT_X3_RULE_TRACE
          constexpr
        #endif//BOOST_SPIRIT_X3_RULE_TRACE
          rule_definition
          < ID
          , typename extension::as_parser<RHS>::value_type
          , Attribute
          , force_attribute_
          >
        operator=(RHS const& rhs) const&
        {
          #ifdef BOOST_SPIRIT_X3_RULE_TRACE
            std::cout
              <<BOOST_SPIRIT_X3_RULE_FILE<<':'<<__LINE__<<':'<<__func__<<"(...)const&\n"
              <<":ID*="<<demangle_fmt_type<ID*>()
              <<";\n";
          #endif
            return { as_parser(rhs), name };
        }

        template <typename RHS>
        #ifndef BOOST_SPIRIT_X3_RULE_TRACE
          constexpr
        #endif//BOOST_SPIRIT_X3_RULE_TRACE
          rule_definition
          < ID
          , typename extension::as_parser<RHS>::value_type
          , Attribute
          , true
          >
        operator%=(RHS const& rhs) const&
        {
          #ifdef BOOST_SPIRIT_X3_RULE_TRACE
            std::cout
              <<BOOST_SPIRIT_X3_RULE_FILE<<':'<<__LINE__<<':'<<__func__<<"(...)const&\n"
              <<":ID*="<<demangle_fmt_type<ID*>()
              <<";\n";
          #endif
            return { as_parser(rhs), name };
        }

        // When a rule placeholder constructed and immediately consumed it cannot be used recursively,
        // that's why the rule definition injection into a parser context can be skipped.
        // This optimization has a huge impact on compile times because immediate rules are commonly
        // used to cast an attribute like `as`/`attr_cast` does in Qi.
        template <typename RHS>
        #ifndef BOOST_SPIRIT_X3_RULE_TRACE
          constexpr
        #endif//BOOST_SPIRIT_X3_RULE_TRACE
          rule_definition
          < ID
          , typename extension::as_parser<RHS>::value_type
          , Attribute
          , force_attribute_
          , true
          >
        operator=(RHS const& rhs) const&&
        {
          #ifdef BOOST_SPIRIT_X3_RULE_GEN_TRACE
            boost::trace_scope ts
              ( stringify
                (BOOST_SPIRIT_X3_RULE_FILE,':',__LINE__,':',__func__,"(...)const&&")
              );
            std::cout
              <<":ID*="<<demangle_fmt_type<ID*>()
              <<";\n";
          #endif
            return { as_parser(rhs), name };
        }

        template <typename RHS>
        #ifndef BOOST_SPIRIT_X3_RULE_TRACE
          constexpr
        #endif
          rule_definition
          < ID
          , typename extension::as_parser<RHS>::value_type
          , Attribute
          , true
          , true
          >
        operator%=(RHS const& rhs) const&&
        {
          #ifdef BOOST_SPIRIT_X3_RULE_TRACE
            std::cout
              <<BOOST_SPIRIT_X3_RULE_FILE<<':'<<__LINE__<<':'<<__func__<<"(...)const&&\n"
              <<":ID*="<<demangle_fmt_type<ID*>()
              <<";\n";
          #endif
            return { as_parser(rhs), name };
        }

          template 
          < typename Iterator
          , typename Context
          , typename Attribute_
          >
        bool parse
          ( Iterator& first, Iterator const& last
          , Context const& context
          , unused_type
          , Attribute_& attr_
          ) const
        {
          #ifdef BOOST_SPIRIT_X3_RULE_TRACE
            boost::trace_scope ts
              ( stringify
                ( BOOST_SPIRIT_X3_RULE_FILE
                , ':'
                , __LINE__
                , ':'
                , __func__
                , ":Attribute_& attr"
                )
              );
            std::cout<<":ID*=\n"<<demangle_fmt_type<ID*>()<<";\n";
            std::cout<<":attribute_type=\n"<<demangle_fmt_type<attribute_type>()<<";\n";
            std::cout<<":Attribute_=\n"<<demangle_fmt_type<Attribute_>()<<";\n";
            std::cout<<":has_attribute="<<has_attribute<<";\n";
          #endif//BOOST_SPIRIT_X3_RULE_TRACE
          #if 1
            static_assert(has_attribute,
                "The rule does not have an attribute. Check your parser.");
          #else//used when debugging
            if(!has_attribute)
              std::cout<<"The rule does not have an attribute. Check your parser.\n";
          #endif                
            using transform = 
                traits::transform_attribute_tagged
                < Attribute_
                , attribute_type
                , rule_external_id<ID>
                >
                ;
            using transform_attr = typename transform::type;
            transform_attr tr_attr = transform::pre(attr_);

            bool const result=parse_rule((ID*)0, first, last, context, tr_attr);
            if(result)
            {
                transform::post(attr_, std::forward<transform_attr>(tr_attr));
            }
            return result;
        }

        template <typename Iterator, typename Context>
        bool parse
          ( Iterator& first, Iterator const& last
          , Context const& context
          , unused_type
          , unused_type
          ) const
        {
          #ifdef BOOST_SPIRIT_X3_RULE_TRACE
            boost::trace_scope ts
              ( stringify
                ( BOOST_SPIRIT_X3_RULE_FILE
                , ':'
                , __LINE__
                , ':'
                , __func__
                , ":unused_type"
                )
              );
          #endif//BOOST_SPIRIT_X3_RULE_TRACE
            // make sure we pass exactly the rule attribute type
            attribute_type no_attr{};
            return parse_rule((ID*)0, first, last, context, no_attr);
        }

        rule_name_type name;
    };

    namespace traits
    {
        template <typename T, typename Enable = void>
        struct is_rule : mpl::false_ {};

        template <typename ID, typename Attribute, bool force_attribute>
        struct is_rule<rule<ID, Attribute, force_attribute>> : mpl::true_ {};

        template <typename ID, typename Attribute, typename RHS, bool force_attribute, bool skip_definition_injection>
        struct is_rule<rule_definition<ID, RHS, Attribute, force_attribute, skip_definition_injection>> : mpl::true_ {};
    }

    template <typename T>
    struct get_info<T, typename enable_if<traits::is_rule<T>>::type>
    {
        std::ostream& operator()(std::ostream& sout, T const& r) const
        {
            BOOST_ASSERT_MSG(r.name, "uninitialized rule"); // static initialization order fiasco
            return sout<<(r.name? r.name : "uninitialized");
        }
        std::string operator()(T const& r) const
        {
            BOOST_ASSERT_MSG(r.name, "uninitialized rule"); // static initialization order fiasco
            return r.name? r.name : "uninitialized";
        }
    };
    
    
}}}
#endif
