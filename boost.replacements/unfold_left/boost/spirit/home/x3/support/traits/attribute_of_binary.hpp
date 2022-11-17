#ifndef BOOST_SPIRIT_X3_SUPPORT_TRAITS_ATTRIBUTE_OF_BINARY
#define BOOST_SPIRIT_X3_SUPPORT_TRAITS_ATTRIBUTE_OF_BINARY
#pragma push_macro("FILE_SHORT")
#define FILE_SHORT "traits/attribute_of_binary.hpp"

#ifdef USE_UNFOLD_LEFT
  #ifdef USE_COLLAPSE_LEFT
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/is_unused.hpp>
#include <boost/mp11.hpp>
namespace boost { namespace spirit { namespace x3 { namespace detail
{
      
      template 
      < template <typename...> class C //a variant container for attributes
      , typename... A
      >
    struct attribute_of_variant
      { 
          using no_unused=
            mp11::mp_copy_if
            < mp11::mp_list<A...>
            , traits::not_unused
            >
            ;
          using no_dups=
            mp11::mp_unique
            < no_unused
            >
            ;
          using type=
            mp11::mp_rename
            < no_dups
            , C
            >
            ;
      };
      template 
      < template <typename...> class C //a sequence container for attributes
      , typename... A
      >
    struct attribute_of_sequence
      { 
          using no_unused=
            mp11::mp_copy_if
            < mp11::mp_list<A...>
            , traits::not_unused
            >
            ;
          using type=
            mp11::mp_rename
            < no_unused
            , C
            >
            ;
      };
}}}}
  #else
    #pragma GCC error "attribute_of_binary unused when defined(USE_UNFOLD_LEFT) && !defined(USE_COLLAPSE_LEFT)."
  #endif//USE_COLLAPSE_LEFT
#else
//duplicate the normal spirit file:
/*=============================================================================
    Copyright (c) 2020 Nikita Kniazev

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/type_traits/type_identity.hpp>
#define USE_TRANSFER_VARIANT_UNIQUE
#ifdef  USE_TRANSFER_VARIANT_UNIQUE
  #include <boost/spirit/home/x3/support/traits/is_variant.hpp>
  #include <boost/utility/enable_if.hpp>
  #include <boost/mp11.hpp>
  namespace mp11=boost::mp11;
#endif//USE_TRANSFER_VARIANT_UNIQUE


namespace boost { namespace spirit { namespace x3 { namespace detail
{

    template <typename... T>
    struct type_sequence
      {
        using type = type_sequence;

        static const int size = sizeof...(T);

        template <typename... U>
        using append = type_sequence<T..., U...>;

        template <typename... U>
        using prepend = type_sequence<U..., T...>;

        template <typename U>
        using extend = typename U::template prepend<T...>;
        
        #ifdef USE_TRANSFER_VARIANT_UNIQUE
            template 
            < template <typename...> class U
            , typename Enable = void
            >
          struct maybe_variant_transfer
            //If U is not a variant, simply transfer to U.
            { using type=U<T...>;
              static void trace_tmpl()
              {
              ; boost::trace_scope ts(stringify(":__LINE__=",__LINE__))
              ; std::cout<<":type="<<demangle_fmt_type<type>()<<";\n";
              ;}
            };
            template 
            < template <typename...> class U
            >
          struct maybe_variant_transfer
            < U
            , typename boost::enable_if
              < typename x3::traits::is_variant
                < U
                  < char//anything but void, just to allow is_variant to work.
                  >
                >
              >::type
            >
            //If U is a variant, assure the types are unique.
            { 
              using mp_list_T=mp11::mp_list<T...>
                ;
              using unique_t=mp11::mp_unique<mp_list_T>
                ;
                template
                < typename S
                >
              struct unpack_transfer
                ;
                template
                < typename... S
                >
              struct unpack_transfer
                < mp11::mp_list<S...>
                >
                { using type=U<S...>;
                };
              using type=typename unpack_transfer< unique_t>::type
                ;
              static void trace_tmpl()
              {
              ; boost::trace_scope ts(stringify(":__LINE__=",__LINE__))
              ; std::cout<<":type="<<demangle_fmt_type<type>()<<";\n";
              ;}
            };
          template <template <typename...> class U>
          using maybe_transfer = maybe_variant_transfer<U>;
          
          template <template <typename...> class U>
          using transfer_to = typename maybe_transfer<U>::type;
          
          template<template <typename...> class U>
          static void trace_tmpl()
          {
          ; boost::trace_scope ts(stringify(":__LINE__=",__LINE__))
          ; std::cout<<":transfer_to=\n"<<demangle_fmt_type<transfer_to<U>>()<<";\n"
          ; std::cout<<":maybe_transfer<U>=\n"<<demangle_fmt_type<maybe_transfer<U>>()<<";\n"
          ; std::cout<<":maybe_transfer<U>::trace_tmpl():\n"
          ; maybe_transfer<U>::trace_tmpl()
          ;}
          static void trace_tmpl()
          {
          ; boost::trace_scope ts(stringify(":__LINE__=",__LINE__))
          ;}
        #else        
          template <template <typename...> class U>
          using transfer_to = U<T...>;
          
          static void trace_tmpl()
          {
          ; boost::trace_scope ts(stringify(":__LINE__=",__LINE__))
          ;}
        #endif//USE_TRANSFER_VARIANT_UNIQUE

      };

    template <typename Attribute>
    struct types_of_binary_init 
      : type_sequence<Attribute> 
      {
        using super=type_sequence<Attribute>;
        static void trace_tmpl()
        { 
          std::cout<<FILE_SHORT<<":"<<__LINE__<<";\n";
          std::cout<<":self="<<demangle_fmt_type<types_of_binary_init>()<<";\n";
          std::cout<<indent_buf_in;
          super::trace_tmpl();
          std::cout<<indent_buf_out;
        }
      };
    template <>
    struct types_of_binary_init<unused_type> 
      : type_sequence<> 
      {
        using super=type_sequence<>;
        static void trace_tmpl()
        { 
          std::cout<<FILE_SHORT<<":"<<__LINE__<<";\n";
          std::cout<<":self="<<demangle_fmt_type<types_of_binary_init>()<<";\n";
          std::cout<<indent_buf_in;
          super::trace_tmpl();
          std::cout<<indent_buf_out;
        }
      };
    template <>
    struct types_of_binary_init<unused_type const> 
      : type_sequence<> 
      {
        using super=type_sequence<>;
        static void trace_tmpl()
        { 
          std::cout<<FILE_SHORT<<":"<<__LINE__<<";\n";
          std::cout<<":self="<<demangle_fmt_type<types_of_binary_init>()<<";\n";
          std::cout<<indent_buf_in;
          super::trace_tmpl();
          std::cout<<indent_buf_out;
        }
      };
    template <template <typename, typename> class B, typename P, typename C>
    struct get_types_of_binary
      : types_of_binary_init<typename traits::attribute_of<P, C>::type> 
      {
        using aof_P=traits::attribute_of<P, C>;
        using super=types_of_binary_init<typename traits::attribute_of<P, C>::type>;
        static void trace_tmpl()
        { 
          std::cout<<FILE_SHORT<<":"<<__LINE__<<":get_types_of_binary<B,P,C>;\n";
          std::cout<<":self="<<demangle_fmt_type<get_types_of_binary>()<<";\n";
          std::cout<<indent_buf_in;
          aof_P::trace_tmpl();
          super::trace_tmpl();
          std::cout<<indent_buf_out;
        }
      };
    template <template <typename, typename> class B, typename L, typename R, typename C>
    struct get_types_of_binary<B, B<L, R>, C>
      : get_types_of_binary<B, L, C>::template extend<get_types_of_binary<B, R, C>> 
      {
        using types_left=get_types_of_binary<B, L, C>;
        using types_right=get_types_of_binary<B, R, C>;
        using super=typename types_left::template extend<types_right>;
        static void trace_tmpl()
        { 
          boost::trace_scope ts(stringify(FILE_SHORT,":", __LINE__,":get_types_of_binary<B, B<L, R>, C>"));
          std::cout<<":self="<<demangle_fmt_type<get_types_of_binary>()<<";\n";
          std::cout<<"types_left::trace_tmpl:\n";
          types_left::trace_tmpl();
          std::cout<<"types_right::trace_tmpl:\n";
          types_right::trace_tmpl();
          std::cout<<"super::trace_tmpl:\n";
          super::trace_tmpl();
        }
      };

      template 
      < template <typename...> class A
      , typename T
      , int = T::size
      >
    struct type_sequence_to_attribute 
      { using type = 
          typename T::template transfer_to<A>
          ; 
        static void trace_tmpl()
        { 
          boost::trace_scope ts(stringify(FILE_SHORT,":", __LINE__,":type_sequence_to_attribute"));
          std::cout<<":self=\n"<<demangle_fmt_type<type_sequence_to_attribute>()<<";\n";
          std::cout<<":type="<<demangle_fmt_type<type>()<<";\n";
        }
      };
      template 
      < template <typename...> class A
      , typename T
      >
    struct type_sequence_to_attribute<A, T, 1> 
      : T::template transfer_to<type_identity> 
      {
        using super=typename T::template transfer_to<type_identity>;
        static void trace_tmpl()
        { 
          std::cout<<__LINE__<<":self=\n"<<demangle_fmt_type<type_sequence_to_attribute>()<<";\n";
          std::cout<<":super="<<demangle_type<super>()<<";\n";
          std::cout<<":type="<<demangle_fmt_type<typename super::type>()<<";\n";
        }
      };
      template 
      < template <typename...> class A
      , typename T
      >
    struct type_sequence_to_attribute<A, T, 0> 
      { using type = unused_type; 
        static void trace_tmpl()
        { 
          std::cout<<__LINE__<<":self=\n"<<demangle_fmt_type<type_sequence_to_attribute>()<<";\n";
          std::cout<<":type="<<demangle_fmt_type<type>()<<";\n";
        }
      };

      template 
      < template <typename...> class A //container for attributes of B
      , template <typename Left, typename Right> class B //binary parser
      , typename L //Left of a B<Left,Right>
      , typename R //Right of a B<Left,Right>
      , typename C //Context arg to attribute_of
      >
    struct attribute_of_binary
      : type_sequence_to_attribute
        < A
        , typename get_types_of_binary
          < B
          , B<L, R>
          , C
          >::type
        >
      {
        static void trace_tmpl()
        { 
          boost::trace_scope ts(stringify(FILE_SHORT,":", __LINE__,":attribute_of_binary"));
          using get_types=
            get_types_of_binary
            < B
            , B<L, R>
            , C
            >;
          using get_types_type=typename get_types::type;
          using super=
            type_sequence_to_attribute
            < A
            , typename get_types_of_binary
              < B
              , B<L, R>
              , C
              >::type
            >;
          std::cout<<":get_types::trace_tmpl()\n";
          get_types::trace_tmpl();
          std::cout<<":get_types_type="<<demangle_fmt_type<get_types_type>()<<";\n";
          std::cout<<":super::trace_tmpl()\n";
          super::trace_tmpl();
        }
      };
}}}}
#endif//USE_UNFOLD_LEFT
#pragma pop_macro("FILE_SHORT")
#endif//BOOST_SPIRIT_X3_SUPPORT_TRAITS_ATTRIBUTE_OF_BINARY
