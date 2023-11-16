/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_ATTRIBUTE_OF_JAN_7_2012_0914AM)
#define BOOST_SPIRIT_X3_ATTRIBUTE_OF_JAN_7_2012_0914AM
#define BOOST_SPIRIT_X3_ATTRIBUTE_TRACE
#ifdef BOOST_SPIRIT_X3_ATTRIBUTE_TRACE
  #define BOOST_SPIRIT_X3_ATTRIBUTE_FILE "traits/attribute_of.hpp"
#endif

#include <boost/spirit/home/x3/support/utility/sfinae.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    // Get the attribute type of a component. By default, this gets the
    // Component's attribute_type typedef or instantiates a nested attribute
    // metafunction. Components may specialize this if such an attribute_type
    // is not readily available (e.g. expensive to compute at compile time).
    ///////////////////////////////////////////////////////////////////////////
    template <typename Component, typename Context, typename Enable = void>
    struct attribute_of;

    namespace detail
    {
        template <typename Component, typename Context, typename Enable = void>
        struct default_attribute_of;

        template <typename Component, typename Context>
        struct default_attribute_of
          < Component
          , Context
          , typename disable_if_substitution_failure
            < typename Component::attribute_type
            >::type
          >
          : mpl::identity<typename Component::attribute_type> 
          {
            static void trace_tmpl()
            {
              std::cout<<BOOST_SPIRIT_X3_ATTRIBUTE_FILE<<":"<<__LINE__<<":self=default_attribute_of:Component::attribute_type;\n";
              using disable_if_type=typename disable_if_substitution_failure
                < typename Component::attribute_type
                >::type;
              std::cout<<BOOST_SPIRIT_X3_ATTRIBUTE_FILE<<":disable_if_type=\n"<<demangle_fmt_type<disable_if_type>()<<";\n";
              using super=mpl::identity<typename Component::attribute_type>;
              std::cout<<BOOST_SPIRIT_X3_ATTRIBUTE_FILE<<":super::type="<<demangle_fmt_type<typename super::type>()<<";\n";
            }
          };

        template <typename Component, typename Context>
        struct default_attribute_of
          < Component
          , Context
          , typename disable_if_substitution_failure
            < typename Component::template attribute<Context>::type
            >::type
          >
          : Component::template attribute<Context> 
          {
            static void trace_tmpl()
            {
              std::cout<<BOOST_SPIRIT_X3_ATTRIBUTE_FILE<<":"<<__LINE__<<":self=default_attribute_of:Component::template attribute;\n";
              using disable_if_type=typename disable_if_substitution_failure
                < typename Component::template attribute<Context>::type
                >::type;
              std::cout<<BOOST_SPIRIT_X3_ATTRIBUTE_FILE<<":disable_if_type=\n"<<demangle_fmt_type<disable_if_type>()<<";\n";
              using super=typename Component::template attribute<Context>;
              std::cout<<BOOST_SPIRIT_X3_ATTRIBUTE_FILE<<":super::type="<<demangle_fmt_type<typename super::type>()<<";\n";
            }
          };

        template <typename Component, typename Context>
        struct default_attribute_of
          < Component
          , Context
          , typename enable_if_c
            < Component::is_pass_through_unary
            >::type
          >
          : attribute_of<typename Component::subject_type, Context>
          {
            static void trace_tmpl()
            {
              std::cout<<BOOST_SPIRIT_X3_ATTRIBUTE_FILE<<":"<<__LINE__<<":self=default_attribute_of:attribute_of<Component::subject_type>;\n";
              using super=attribute_of<typename Component::subject_type, Context>;
              std::cout<<BOOST_SPIRIT_X3_ATTRIBUTE_FILE<<":super::"<<__func__<<"();\n";
              std::cout<<indent_buf_in;
              super::trace_tmpl();
              std::cout<<indent_buf_out;
            }
          };
    }

    template <typename Component, typename Context, typename Enable>
    struct attribute_of 
      : detail::default_attribute_of
        < std::remove_const_t<Component>
        , Context
        > 
      {
        static void trace_tmpl()
        { 
          boost::trace_scope ts(stringify(BOOST_SPIRIT_X3_ATTRIBUTE_FILE,":", __LINE__,":attribute_of"));
          using super=detail::default_attribute_of<Component, Context>;
          std::cout<<BOOST_SPIRIT_X3_ATTRIBUTE_FILE<<":super::"<<__func__<<"();\n";
          std::cout<<indent_buf_in;
          super::trace_tmpl();
          std::cout<<indent_buf_out;
        }
      };
  #if defined(USE_UNFOLD_LEFT) && defined(USE_COLLAPSE_LEFT)
    template <typename... Component, typename Context, typename Enable>
    struct attribute_of
      < fusion::deque<Component...>
      , Context
      , Enable
      >
      {
          using
        type=
          fusion::deque
          < attribute_of
            < Component
            , Context
            >...
          >
          ;
      };
    
  #endif//defined(USE_UNFOLD_LEFT) && defined(USE_COLLAPSE_LEFT)
}}}}

#endif
