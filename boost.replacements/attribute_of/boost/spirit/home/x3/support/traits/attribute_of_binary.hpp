/*=============================================================================
    Copyright (c) 2020 Nikita Kniazev
s
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_X3_SUPPORT_TRAITS_ATTRIBUTE_OF_BINARY
#define BOOST_SPIRIT_X3_SUPPORT_TRAITS_ATTRIBUTE_OF_BINARY
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/type_traits/type_identity.hpp>

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

        template <template <typename...> class U>
        using transfer_to = U<T...>;
    };

      template <typename Attribute>
    struct types_of_binary_init 
      : type_sequence<Attribute> 
      {};
  #ifndef USE_ATTRIBUTE_OF_NO_COLLAPSE
    //do attribute collapse
    template <>
    struct types_of_binary_init<unused_type> : type_sequence<> {};
    template <>
    struct types_of_binary_init<unused_type const> : type_sequence<> {};
  #else
    //do not do attribute collapse
  #endif//USE_ATTRIBUTE_OF_NO_COLLAPSE
      
      template 
      < template <typename, typename> class B //binary parser template
      , typename P
      , typename C
      >
    struct get_types_of_binary //get attribute for not B parser, P
      : types_of_binary_init
        < typename traits::attribute_of<P, C>::type
        > 
      {};
      template 
      < template <typename, typename> class B
      , typename L
      , typename R
      , typename C
      >
    struct get_types_of_binary<B, B<L, R>, C> //get attribute for B<L,R> by
      : get_types_of_binary<B, L, C> //getting type_sequence for left parser, L
        ::template extend //then extending it
          < get_types_of_binary<B, R, C> //with type_sequence for right parser, R
          > 
      {};

      template 
      < template <typename...> class A //attribute template
      , typename T //type_sequence<...>
      >
    struct type_sequence_to_attribute 
      { using type = typename T::template transfer_to<A>; 
      };

      template 
      < template <typename...> class A //template of the calculated attribute
      , template <typename, typename> class B //binary parser template
      , typename L
      , typename R
      , typename C
      >
    using attribute_of_binary = //the attribute of B<L, R>
      type_sequence_to_attribute
      < A
      , typename get_types_of_binary
        < B
        , B<L, R> //the actual parser whose attribute is being calculated.
        , C
        >::type //==type_sequence<T...> for some T...
      >;
}}}}
#endif
