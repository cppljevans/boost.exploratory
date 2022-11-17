/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_VARIANT_FIND_SUBSTITUTE_APR_18_2014_930AM)
#define BOOST_SPIRIT_X3_VARIANT_FIND_SUBSTITUTE_APR_18_2014_930AM

#pragma push_macro("FILE_SHORT")
#define FILE_SHORT "traits/variant_find_substitute.hpp"

#include <boost/spirit/home/x3/support/traits/is_substitute.hpp>
#include <boost/mpl/find.hpp>

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename Variant, typename T>
    struct variant_find_substitute
    {
        // Get the type from the Variant that can be a substitute for T.
        // T is result of attribute_of on some parser.
        // If none is found, just return T

        typedef Variant variant_type;
        typedef typename variant_type::types types;
        typedef typename mpl::end<types>::type end;

      //#define VARIANT_FIND_SUBSTITUTE_USE_IS_SUB_IMPL
      #ifdef VARIANT_FIND_SUBSTITUTE_USE_IS_SUB_IMPL
          template<typename U, typename V>
        using is_subs=
          traits::detail::is_substitute_impl<U, V>
          ;
          typedef typename mpl::find<types, T>::type 
        iter_same //find if T is same as some S in Variant::types.
          ;
          typedef is_same<iter_same, end>
        iter_same_end
          //T not same as some S in Variant::types?
          ;
        typedef typename
            mpl::eval_if
            < iter_same_end
            , mpl::find_if<types, is_subs<T,mpl::_1>> //THEN find_if T can be substituted for some S in Variant::types.
            , mpl::identity<iter_same> //ELSE return iterator to the T found in Variant::types.
            >::type
        iter;

        typedef typename
            mpl::eval_if
            < is_same<iter, end> //IF T is neither same nor substitutable for one of types:
            , mpl::identity<T> //THEN return T
            , mpl::deref<iter> //ELSE return the found member of types.
            >::type
        type;
        
        static void trace_tmpl()
        { boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',__func__));
          std::cout<<":Variant<S...>=\n"<<demangle_fmt_type<Variant>()<<";\n";
          std::cout<<":T="<<demangle_fmt_type<T>()<<";\n";
          auto constexpr iter_same_at_end=iter_same_end::value;
          std::cout<<":iter_same_at_end="<<iter_same_at_end<<";\n";
          bool result=false;
          { boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":trace find same."));
            auto find_same=[]<typename...S>(boost::variant<S...> var)
              { int count=-1;
                bool result=
                  (  ( ++count
                     , std::cout<<":is_same<T,S["<<count<<"]>="<<is_same<T,S>::value<<";\n"
                     , bool(is_same<T,S>::value)
                     )||...
                  )
                  ;
                std::cout<<":result="<<result<<";\n";
                return result;
              };
            Variant var;
            result=find_same(var);
            std::cout<<":"<<(result?" ":"!")<<"is_same<T,S> for some S in Variant::types;\n";
          }
          if (!result)
          { boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":!is_same<T,S> for some S in Variant::types"));
            using iter_sub=mpl::find_if<types, is_subs<T,mpl::_1>>;
            auto constexpr iter_at_end=is_same<iter,end>::value;
            std::cout<<":iter_at_end="<<iter_at_end<<";\n";
            { boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":trace find sub."));
              auto find_sub=[]<typename...S>(boost::variant<S...> var)
                { int count=-1;
                  bool result=
                    (  ( ++count
                       , is_subs<T, S>::trace_tmpl()
                       , std::cout<<":is_subs<T,S["<<count<<"]>::value="<<is_subs<T,S>::value<<";\n"
                       , bool(is_subs<T,S>::value)
                       )||...
                    )
                    ;
                  std::cout<<":result="<<result<<";\n";
                  return result;
                };
              Variant var;
              result=find_sub(var);
              std::cout<<":"<<(result?" ":"!")<<"is_subs<T,S> for some S in Variant::types;\n";
            }
          }
        }//(!result)
  #else
          template<typename U, typename V>
        using is_subs=
          traits::is_substitute<U, V>
          ;
          typedef typename mpl::find_if<types, is_subs<T,mpl::_1>>::type 
        iter //find if T is substitutable for some S in Variant::types.
          ;
        typedef typename
            mpl::eval_if
            < is_same<iter, end> //IF T is not substitutable for one of types:
            , mpl::identity<T> //THEN return T
            , mpl::deref<iter> //ELSE return the found member of types.
            >::type
        type;
        
        static void trace_tmpl()
        { boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',__func__));
          std::cout<<":Variant<S...>=\n"<<demangle_fmt_type<Variant>()<<";\n";
          std::cout<<":T="<<demangle_fmt_type<T>()<<";\n";
          std::cout<<":iter="<<demangle_fmt_type<iter>()<<";\n";
          auto constexpr iter_at_end=is_same<iter,end>::value;
          std::cout<<":iter_at_end="<<iter_at_end<<";\n";
          bool result=false;
          { boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":trace find sub."));
            auto find_sub=[]<template<typename...>typename V, typename...S>(V<S...> var)
              { int count=-1;
                bool result=
                  (  ( ++count
                     , is_subs<T, S>::trace_tmpl()
                     , std::cout<<":is_subs<T,S["<<count<<"]>::value="<<is_subs<T,S>::value<<";\n"
                     , bool(is_subs<T,S>::value)
                     )||...
                  )
                  ;
                std::cout<<":result="<<result<<";\n";
                return result;
              };
            Variant var;
            result=find_sub(var);
            std::cout<<":"<<(result?" ":"!")<<"is_subs<T,S> for some S in Variant::types;\n";
          }
        }
  #endif//VARIANT_FIND_SUBSTITUTE_USE_IS_SUB_IMPL
    };
    
    template <typename Variant>
    struct variant_find_substitute<Variant, Variant>
        : mpl::identity<Variant> {};

}}}}

#pragma pop_macro("FILE_SHORT")
#endif
