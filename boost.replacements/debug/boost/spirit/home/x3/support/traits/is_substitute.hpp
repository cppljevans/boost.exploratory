/*=============================================================================
    Copyright (c) 2022 Larry Joe Evans
    Copyright (c) 2001-2014 Joel de Guzman
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_IS_SUBSTITUTE_OCT_1_2022_604PM)
#define BOOST_SPIRIT_X3_IS_SUBSTITUTE_OCT_1_2022_604PM

#include <boost/spirit/home/x3/support/traits/container_traits.hpp>
#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
#include <boost/spirit/home/x3/support/traits/tuple_traits.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/value_at_key.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/support/is_iterator.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/optional/optional.hpp>
#include <boost/spirit/home/x3/support/traits/is_same_enough.hpp>

#pragma push_macro("FILE_SHORT")
#define FILE_SHORT "debug/*/traits/is_substitute.hpp"

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    // Find out if T can be a (strong) substitute for Attribute
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Attribute, typename Enable = void>
    struct is_substitute;

    template <typename Variant, typename Attribute>
    struct variant_has_substitute;

    namespace detail
    {
        template <typename T, typename Attribute>
        void is_substitute_impl_enablers_trace_tmpl()
          //forward delcare because static member calls static members of 
          //is_substitute_impl_enabler templates defined below.
        ;
        template <typename T, typename Attribute, typename Enable = void>
        struct is_substitute_impl 
        : mpl::false_ 
        {
            static bool trace_tmpl()
            { auto constexpr this_type_name="is_substitute_impl<T ,Attribute, Enable = void>";
              std::cout<<FILE_SHORT<<':'<<__LINE__<<':'<<this_type_name<<"::"<<__func__<<":result=mpl::false_;\n";
              is_substitute_impl_enablers_trace_tmpl<T,Attribute>();
              return false;
            }
        };

        template <typename T, typename Attribute, typename Tag>
        struct is_substitute_impl_enabler
        /**@brief
         *  The Enable arg to some is_substitute_impl< T, Attribute, Enable>.
         *  Also provides a static trace_tmpl which provides trace of that Enable.
         *  The Tag argument determines which is_substitute_impl is the
         *  candidate for trace of the Enable arg.
         *
         *  Making the enabler separate from the is_substitute_impl allows the maintainer 
         *  to more easily see why one is_substitute_impl was chosen by the compiler over another
         *  by simply invoking the enabler::trace_tmpl.  For example, see the
         *  is_substitute_impl_enablers_trace_tmpl definition below.
         */
          ;
        
          template
          < typename U
          , typename Enable=void
          >
        struct size_composite
          ;
          template
          < typename U
          >
        struct size_composite
          < U
          , typename enable_if
            < fusion::traits::is_sequence<U>
            >::type
          >
          : fusion::result_of::size<U>
          {
          };
          template
          < typename ...U
          , template<typename...>typename Composite
          >
        struct size_composite
          < Composite<U...>
          , typename enable_if
            < is_variant<Composite<U...>>
            >::type
          >
          : mpl::integral_c<std::size_t,sizeof...(U)>
          {
          };
          template
          < typename U
          , std::size_t I
          , typename Enable=void
          >
        struct type_at_c
          ;
          template
          < typename U
          , std::size_t I
          >
        struct type_at_c
          < U
          , I
          , typename enable_if
            < fusion::traits::is_sequence<U>
            >::type
          >
          {
            using type=typename
              std::remove_reference
              < typename std::remove_reference
                < typename fusion::result_of::at_c
                  < U
                  , I
                  >::type
                >::type
              >::type;
          };
          template
          < typename U
          , std::size_t I
          >
        struct type_at_c_variadic
          ;
          template
          < typename ...U
          , template<typename...>typename Composite
          , std::size_t I
          >
        struct type_at_c_variadic
          < Composite<U...>
          , I
          >
          {
            using type=typename mpl::at_c<mpl::vector<U...>,I>::type;
          };
          template
          < typename U
          , std::size_t I
          >
        struct type_at_c
          < U
          , I
          , typename enable_if
            < is_variant<U>
            >::type
          >
          : type_at_c_variadic<U,I>
          {
          };

          template
          < typename Aof
          , typename Uda
          , typename Indices
          >
        struct component_types_substitutable_at
          ;
          template
          < typename Aof
          , typename Uda
          , std::size_t... Indices
          >
        struct component_types_substitutable_at
          < Aof
          , Uda
          , std::index_sequence<Indices...>
          >
        #ifdef __cpp_fold_expressions
          : mpl::bool_
            (  ... 
            && bool
               ( is_substitute
                 < typename type_at_c<Aof,Indices>::type
                 , typename type_at_c<Uda,Indices>::type
                 >::value
               )
            )
        #else
          #   warning "Use -std=c++17 or -std=gnu++17 compiler flag to enable C++17 mode"
        #endif//__cpp_fold_expressions
          {
              static bool trace_tmpl()
              {   
                  auto index=[]<std::size_t I>()->bool
                    { //std::cout<<":I="<<I<<";\n";
                    #if 1
                      using Aof_I=typename type_at_c<Aof,I>::type;
                      using Uda_I=typename type_at_c<Uda,I>::type;
                      std::cout<<":Aof["<<I<<"]=\n"<<demangle_fmt_type<Aof_I>()<<";\n";
                      std::cout<<":Uda["<<I<<"]=\n"<<demangle_fmt_type<Uda_I>()<<";\n";
                      using is_sub_Aof_Uda=is_substitute<Aof_I,Uda_I>;
                      is_sub_Aof_Uda::trace_tmpl();
                      std::cout<<FILE_SHORT<<':'<<__LINE__<<":is_sub_Aof_Uda["<<I<<"]="<<is_sub_Aof_Uda::value<<";\n";
                      return is_sub_Aof_Uda::value;
                    #else
                      return true;
                    #endif
                    };
                  bool results[]
                  { index.template operator()<Indices>()...
                  };
                  bool result=true;
                  for(unsigned i=0; i<sizeof...(Indices); ++i) result=result&&results[i];
                  std::cout<<FILE_SHORT<<':'<<__LINE__<<":value="<<value<<":result="<<result<<";\n";
                  return value;
              }
          };
          template
          < typename Aof//attribute_of<Parser> for some Parser
          , typename Uda//user defined attribute.
          >
        struct component_types_substitutable
          {
              static std::size_t constexpr size_Aof=
                size_composite<Aof>::value
                ;
              static std::size_t constexpr size_Uda=
                size_composite<Uda>::value
                ;
              static bool constexpr big_enough_Uda=
                size_Aof<=size_Uda
                ;
              using type=
                mpl::and_
                < mpl::bool_<big_enough_Uda>
                , component_types_substitutable_at
                  < Aof
                  , Uda
                  , std::make_index_sequence<size_Aof>
                  >
                >;
              static bool trace_tmpl()
              {   if constexpr(big_enough_Uda)
                  {
                      using at_t=
                        component_types_substitutable_at
                        < Aof
                        , Uda
                        , std::make_index_sequence
                          < size_composite<Aof>::value
                          >
                        >;
                      at_t::trace_tmpl();
                      return at_t::value;
                  }
                  else
                  {   std::cout<<__FILE__<<':'<<__LINE__<<':'<<__func__<<":result="<<false<<";\n";
                      return false;
                  }
              }
          };
          
        struct enable_are_seq
          {};
        
        template <typename T, typename Attribute>
        struct is_substitute_impl_enabler<T, Attribute, enable_are_seq>
          : mpl::and_
            < fusion::traits::is_sequence<T>
            , fusion::traits::is_sequence<Attribute>
            , component_types_substitutable<T, Attribute>
            >
        {
            static void trace_tmpl()
            { boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":is_substitute_impl_enabler<T,Attribute,enable_are_seq>"));
              bool constexpr is_T=fusion::traits::is_sequence<T>::value;
              bool constexpr is_Attribute=fusion::traits::is_sequence<Attribute>::value;
              std::cout<<"is_sequence<T>="<<is_T<<";\n";
              std::cout<<"is_sequence<Attribute>="<<is_Attribute<<";\n";
              bool constexpr pre_enabler=is_T && is_Attribute;
              if constexpr (pre_enabler)
              {
                  using post_enabler=component_types_substitutable<T, Attribute>;
                  post_enabler::trace_tmpl();
                  std::cout<<"component_types_substitutable="<<post_enabler::type::value<<";\n";
              }
            }
        };
        
        template <typename T, typename Attribute>
        struct is_substitute_impl
          < T
          , Attribute
          , typename enable_if
            < is_substitute_impl_enabler<T, Attribute, enable_are_seq>
            >::type
          >
          : mpl::true_ 
        {
            static void trace_tmpl()
            { is_substitute_impl_enabler<T, Attribute, enable_are_seq>::trace_tmpl();
            }
        };
        
      #ifdef USE_IS_SUBSTITUTE_IMPL_VAR2
        struct enable_are_var
          {};
        
        template <typename T, typename Attribute>
        struct is_substitute_impl_enabler<T, Attribute, enable_are_var>
          : mpl::and_
            < is_variant<T>
            , is_variant<Attribute>
            , component_types_substitutable<T, Attribute>
            >
        {
            static void trace_tmpl()
            { 
              boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":is_substitute_impl_enabler<T,Attribute,enable_are_var>"));
              bool constexpr is_T=is_variant<T>::value;
              bool constexpr is_Attribute=is_variant<Attribute>::value;
              std::cout<<"is_variant<T>="<<is_T<<";\n";
              std::cout<<"is_variant<Attribute>="<<is_Attribute<<";\n";
              bool constexpr pre_enabler=is_T && is_Attribute;
              if constexpr (pre_enabler)
              {
                  using post_enabler=component_types_substitutable<T, Attribute>;
                  post_enabler::trace_tmpl();
                  std::cout<<"component_types_substitutable="<<post_enabler::type::value<<";\n";
              }
              std::cout<<":result="<<is_substitute_impl_enabler::value<<";\n";
            }
        };
        
        template <typename T, typename Attribute>
        struct is_substitute_impl
          < T
          , Attribute
          , typename enable_if
            < is_substitute_impl_enabler<T, Attribute, enable_are_var>
            >::type
          >
          : mpl::true_ 
        {
            static void trace_tmpl()
            { is_substitute_impl_enabler<T, Attribute, enable_are_var>::trace_tmpl();
            }
        };
      #endif//USE_IS_SUBSTITUTE_IMPL_VAR2
        
      #define USE_IS_SUBSTITUTE_IS_VARIANT_ATTRIBUTE
      #ifdef USE_IS_SUBSTITUTE_IS_VARIANT_ATTRIBUTE
        struct enable_is_variant_Attribute
          {};
        
        template <typename T, typename Attribute>
        struct is_substitute_impl_enabler<T, Attribute, enable_is_variant_Attribute>:
        #ifdef USE_IS_SUBSTITUTE_IMPL_VAR2
          mpl::and_
          < mpl::not_<is_variant<T>>
          , is_variant<Attribute>
          , variant_has_substitute<Attribute, T>
          >
        #else
          mpl::and_
          < is_variant<Attribute>
          , variant_has_substitute<Attribute, T>
          >
        #endif//USE_IS_SUBSTITUTE_IMPL_VAR2 
        {
            static void trace_tmpl()
            { 
              boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":is_substitute_impl_enabler<T,Attribute,enable_is_variant_Attribute>"));
              bool constexpr is_variant_Attribute=is_variant<Attribute>::value;
            #ifdef USE_IS_SUBSTITUTE_IMPL_VAR2 
              bool constexpr not_variant_T=mpl::not_<is_variant<T>>::value;
              std::cout<<":not_variant_T="<<not_variant_T<<";\n";
              bool constexpr pre_enabler=not_variant_T && is_variant_Attribute;
            #else
              bool constexpr pre_enabler=is_variant_Attribute;
            #endif
              std::cout<<":is_variant_Attribute="<<is_variant_Attribute<<";\n";
              if constexpr(pre_enabler)
              { using post_enabler=variant_has_substitute<Attribute, T>;
                post_enabler::trace_tmpl();
                std::cout<<"variant_has_substitute="<<post_enabler::type::value<<";\n";
              }
            }
        };
        
        template <typename T, typename Attribute>
        struct is_substitute_impl
          < T
          , Attribute
          , typename enable_if
            < is_substitute_impl_enabler<T, Attribute, enable_is_variant_Attribute>
            >::type
          >
          : variant_has_substitute
            < Attribute
            , T
            >
        {
            static void trace_tmpl()
            {  
              is_substitute_impl_enabler<T, Attribute, enable_is_variant_Attribute>::trace_tmpl();
            }
        };
      #endif//USE_IS_SUBSTITUTE_IS_VARIANT_ATTRIBUTE
        
        template <typename T, typename Attribute>
        struct value_type_is_substitute
        /**@brief
         *  This is needed to delay evaluation of
         *  the following container_value<X>::type calls
         *  if those calls were used directly in 
         *  the is_substitute_enabler<,,are_container> template defined below.
         *  Otherwise, may get compiler error if either 
         *  !is_container<T> or !is_container<Attribute> and
         *  the container_value<X>::type calls are used directly in
         *  is_substitute_enabler<,,are_container>.
         **@pre T and Attribute are containers:
         *  is_container<T>::value && is_container<Attribute>::value
         */
          : is_substitute
            < typename container_value<T>::type
            , typename container_value<Attribute>::type
            >
        {
            static bool trace_tmpl()
            {
              auto constexpr this_type_name="detail::value_type_is_substitute<T,Attribute>";
              boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',this_type_name));
              using container_value_T=typename container_value<T>::type;
              using container_value_Attribute=typename container_value<Attribute>::type;
              using is_sub_values=is_substitute<container_value_T,container_value_Attribute>;
              std::cout<<":container_value_T="<<demangle_fmt_type<container_value_T>()<<";\n";
              std::cout<<":container_value_Attribute="<<demangle_fmt_type<container_value_Attribute>()<<";\n";
              is_sub_values::trace_tmpl();
              bool result=is_sub_values::value;
              std::cout<<":result="<<result<<";\n";
              return result;
            }
        };
        
        struct enable_are_container
          {};

        template <typename T, typename Attribute>
        struct is_substitute_impl_enabler<T, Attribute, enable_are_container>
          : mpl::and_
            < is_container<T>
            , is_container<Attribute>
            , detail::value_type_is_substitute<T, Attribute>
            >
          {
            static auto constexpr this_type_name="is_substitute_impl_enabler<T,Attribute,enable_are_container>";
            static bool trace_tmpl()
            { boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',this_type_name));
              bool constexpr is_container_T=is_container<T>::value;
              bool constexpr is_container_Attribute=is_container<Attribute>::value;
              std::cout<<":is_container_T="<<is_container_T<<";\n";
              std::cout<<":is_container_Attribute="<<is_container_Attribute<<";\n";
              bool constexpr pre_enabler=is_container_T && is_container_Attribute;
              std::cout<<":pre_enabler="<<pre_enabler<<";\n";
              if constexpr (pre_enabler)
              {
                using post_enabler=detail::value_type_is_substitute<T, Attribute>;
                post_enabler::trace_tmpl();
              }
              bool constexpr result=is_substitute_impl_enabler::value;
              std::cout<<":result="<<result<<";\n";
              return result;
            }
          };
  
        template <typename T, typename Attribute>
        struct is_substitute_impl
          < T
          , Attribute
          , typename enable_if
            < is_substitute_impl_enabler<T, Attribute, enable_are_container>
            >::type
          >
          : mpl::true_
        {
            static bool trace_tmpl()
            { return is_substitute_impl_enabler<T, Attribute, enable_are_container>::trace_tmpl()
            ;}
        };
      
        template <typename T, typename Attribute>
        void is_substitute_impl_enablers_trace_tmpl()
        {
        #define USE_IS_SUBSTITUTE_IMPL_ENABLERS_TRACE_TMPL
        #ifdef USE_IS_SUBSTITUTE_IMPL_ENABLERS_TRACE_TMPL
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',__func__,"<T,Attribute>()"));
            std::cout<<":T=\n"<<demangle_fmt_type<T>()<<";\n";
            std::cout<<":Attribute=\n"<<demangle_fmt_type<Attribute>()<<";\n";
            detail::is_substitute_impl_enabler<T, Attribute, detail::enable_are_seq>::trace_tmpl();
          #ifdef USE_IS_SUBSTITUTE_IMPL_VAR2
            detail::is_substitute_impl_enabler<T, Attribute, detail::enable_are_var>::trace_tmpl();
          #endif
          #ifdef USE_IS_SUBSTITUTE_IS_VARIANT_ATTRIBUTE
            detail::is_substitute_impl_enabler<T, Attribute, detail::enable_is_variant_Attribute>::trace_tmpl();
          #endif//USE_IS_SUBSTITUTE_IS_VARIANT_ATTRIBUTE
            detail::is_substitute_impl_enabler<T, Attribute, detail::enable_are_container>::trace_tmpl();
        #endif//USE_IS_SUBSTITUTE_IMPL_ENABLERS_TRACE_TMPL
        }
    
    }//detail namespace

      template 
      < typename T
      , typename Attribute
      , typename Enable /*= void*/
      >
    struct is_substitute
      : mpl::or_
        < is_same_enough<T, Attribute>
        , detail::is_substitute_impl<T, Attribute>
        >
    {
        static bool trace_tmpl()
        {  
           auto constexpr this_type_name="is_substitute<T,Attribute,Enable=void>";
           boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',this_type_name,"::",__func__));
           std::cout<<"T="<<demangle_fmt_type<T>()<<";\n";
           std::cout<<"Attribute="<<demangle_fmt_type<Attribute>()<<";\n";
           std::cout<<"Enable="<<demangle_fmt_type<Enable>()<<";\n";
           using is_same_enough_T_Attribute=is_same_enough<T, Attribute>;
           bool result=is_same_enough_T_Attribute::value;
           is_same_enough_T_Attribute::trace_tmpl();
           std::cout<<"is_same_enough<T,Attribute>="<<is_same_enough_T_Attribute()<<";\n";
           if constexpr (!is_same_enough_T_Attribute())
           { //so the is_same_enough<T, Attriute> in this's superclass is false;
             //so check the alternative, detail::is_substitute_impl<T, Attribute>:
             boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',this_type_name,":!is_same_enough_T_Attribute"));
             using is_substitute_impl_T_Attribute=detail::is_substitute_impl<T, Attribute>;
             result=is_substitute_impl_T_Attribute::value;
             is_substitute_impl_T_Attribute::trace_tmpl();
             std::cout<<":is_substitute_impl<T,Attribute>="<<result<<";\n";
           }
           return result;
        }
    };

    template <typename T, typename Attribute, typename Enable>
    struct is_substitute_trace
        : is_substitute<T, Attribute, Enable>
    {
        static bool trace_tmpl()
        { std::cout<<FILE_SHORT<<':'<<__LINE__<<":is_substitute_trace<T,Attribute,Enable>;\n";
          return is_substitute<T,Attribute,Enable>::trace_tmpl();
        }
    };
    // for reference T
    template <typename T, typename Attribute, typename Enable>
    struct is_substitute<T&, Attribute, Enable>
        : is_substitute_trace<T, Attribute, Enable> 
    {
    };

    // for reference Attribute
    template <typename T, typename Attribute, typename Enable>
    struct is_substitute<T, Attribute&, Enable>
        : is_substitute_trace<T, Attribute, Enable> 
    {
    };

    // for reference T,Attribute
    template <typename T, typename Attribute, typename Enable>
    struct is_substitute<T&, Attribute&, Enable>
        : is_substitute_trace<T, Attribute, Enable> 
    {
    };

    // 2 element mpl tuple is compatible with fusion::map if:
    // - it's first element type is existing key in map
    // - it second element type is compatible to type stored at the key in map
    template <typename T, typename Attribute>
    struct is_substitute<T, Attribute
	, typename enable_if<
	      typename mpl::eval_if<
		  mpl::and_<fusion::traits::is_sequence<T>
			    , fusion::traits::is_sequence<Attribute>>
		  , mpl::and_<traits::has_size<T, 2>
			   , fusion::traits::is_associative<Attribute>>
		  , mpl::false_>::type>::type>

    {
        // checking that "p_key >> p_value" parser can
        // store it's result in fusion::map attribute
        typedef typename mpl::at_c<T, 0>::type p_key;
        typedef typename mpl::at_c<T, 1>::type p_value;

        // for simple p_key type we just check that
        // such key can be found in attr and that value under that key
        // matches p_value
        template <typename Key, typename Value, typename Map>
        struct has_kv_in_map
            : mpl::eval_if<
                fusion::result_of::has_key<Map, Key>
              , mpl::apply<
                    is_substitute<
                        fusion::result_of::value_at_key<mpl::_1, Key>
                      , Value>
                      , Map>
              , mpl::false_>
        {};

        // if p_key is variant over multiple types (as a result of
        // "(key1|key2|key3) >> p_value" parser) check that all
        // keys are found in fusion::map attribute and that values
        // under these keys match p_value
        template <typename Variant>
        struct variant_kv
            : mpl::equal_to<
                mpl::size< typename Variant::types>
              , mpl::size< mpl::filter_view<typename Variant::types
              , has_kv_in_map<mpl::_1, p_value, Attribute>>>
            >
        {};

        typedef typename
            mpl::eval_if<
                is_variant<p_key>
              , variant_kv<p_key>
              , has_kv_in_map<p_key, p_value, Attribute>
            >::type
        type;
    };

    template <typename T, typename Attribute>
    struct is_substitute<optional<T>, optional<Attribute>>
      : is_substitute<T, Attribute> {};
}}}}

#pragma pop_macro("FILE_SHORT")
#endif
