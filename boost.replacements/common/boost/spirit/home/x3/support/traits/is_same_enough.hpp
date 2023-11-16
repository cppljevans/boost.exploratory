#pragma once
#pragma push_macro("FILE_SHORT")
#define FILE_SHORT "traits/is_same_enough.hpp"

#ifndef USE_IS_SAME_ENOUGH
#include <boost/type_traits/is_same.hpp>
namespace boost { namespace spirit { namespace x3 { namespace traits
{
  template<typename Aof, typename Uda>
  struct is_same_enough
  : boost::is_same<Aof,Uda>
  {
      static void trace_tmpl()
      {  
         boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":boost::is_same<Aof,Uda>"));
         std::cout<<":result="<<is_same_enough::value<<";\n";
      }
  };
  
}}}}
#else
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
#include <boost/spirit/home/x3/support/ast/std_variant.hpp>
#define IS_SAME_ENOUGH_TRACE_IS_SAME_ENOUGH_ADAPTED
#ifdef IS_SAME_ENOUGH_TRACE_IS_SAME_ENOUGH_ADAPTED
  #include <boost/fusion/adapted/variant/detail/size_impl.hpp>
#endif//IS_SAME_ENOUGH_TRACE_IS_SAME_ENOUGH_ADAPTED
#include <boost/fusion/include/size.hpp>
#include <boost/mpl/less_equal.hpp>
namespace boost { namespace spirit { namespace x3 { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    // Find out if S and T are similar enough to be considered same.
    ///////////////////////////////////////////////////////////////////////////
    template <typename S, typename T>
    struct is_same_enough
    ;
    template < typename... T>
    struct is_variant
      < std_variant<T...>
      >
    : mpl::true_
    {};
      template 
      < typename S
      , typename T
      >
    struct is_same_enough_composite
    ;
      template 
      < typename... S
      , typename... T
      , template<typename...>typename U
      , template<typename...>typename V
      >
    struct is_same_enough_composite
      < U<S...>
      , V<T...>
      >
    : mpl::and_
      < is_same_enough
        < S
        , T
        >...
      >
    {
        static void trace_tmpl()
        {
           boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":is_same_enough_composite"));
           std::cout<<":U<S...>=\n"<<demangle_fmt_type<U<S...>>()<<";\n"; 
           std::cout<<":V<T...>=\n"<<demangle_fmt_type<V<T...>>()<<";\n"; 
           bool values[]=
             { ( is_same_enough
                 < S
                 , T
                 >::trace_tmpl()
               , is_same_enough
                 < S
                 , T
                 >::value
               )...
             }
             ;
            bool result=true;
            for(unsigned i=0; i<sizeof...(S); ++i)
            { std::cout<<":i="<<i<<":value="<<values[i]<<";\n";
              result=result&&values[i];
            }
            std::cout<<":result="<<result<<";\n";
        }
    };
      template 
      < typename S
      , typename T
      , typename I
      >
    struct is_same_enough_adapted
      ;
    template< typename U, std::size_t J>
    using adapted_c=typename std::remove_reference<typename fusion::result_of::at_c<U,J>::type>::type
      ;
      template 
      < typename S
      , typename T
      , std::size_t... I
      >
    struct is_same_enough_adapted
      < S
      , T
      , std::integer_sequence<std::size_t, I...>
      >
      : mpl::and_
        < is_same_enough
          < adapted_c<S,I>
          , adapted_c<T,I>
          >...
        >
      {
        static void trace_tmpl()
      #ifndef IS_SAME_ENOUGH_TRACE_IS_SAME_ENOUGH_ADAPTED
        {
        }
      #else
        {
          boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":is_same_enough_adapted<S,T>")); 
          bool values[]=
            { ( is_same_enough
                < adapted_c<S,I>
                , adapted_c<T,I>
                >::trace_tmpl()
              , is_same_enough
                < adapted_c<S,I>
                , adapted_c<T,I>
                >::value
              )...
            }
            ;
          bool result=true;
          for(unsigned i=0; i<sizeof...(I); ++i)
          { std::cout<<":i="<<i<<":value="<<values[i]<<";\n";
            result=result&&values[i];
          }
          std::cout<<":result="<<result<<";\n";
        }
      #endif//IS_SAME_ENOUGH_TRACE_IS_SAME_ENOUGH_ADAPTED
      };
      template 
      < typename... S
      , typename T
      , template<typename...>typename U
      >
    struct is_same_enough_composite
      < U<S...>
      , T
      >
    : is_same_enough_adapted
      < U<S...>
      , T
      , std::make_index_sequence
        < sizeof...(S)
        >
      >
    {
        static void trace_tmpl()
        {
           boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":is_same_enough_composite<U<S...>,T>"));
           using super=
             is_same_enough_adapted
              < U<S...>
              , T
              , std::make_index_sequence
                < sizeof...(S)
                >
              >;
           super::trace_tmpl(); 
        }
    };
    template <typename S, typename T>
    struct is_same_enough
    : mpl::or_
      < is_same<S,T>
      , mpl::and_
        < mpl::or_
          < mpl::and_
            < is_variant<S>
            , is_variant<T>
            >
          , mpl::and_
            < fusion::traits::is_sequence<S>
            , fusion::traits::is_sequence<T>
            , mpl::less_equal
              < fusion::result_of::size<S>
              , fusion::result_of::size<T>
              >
            >
          >
        , is_same_enough_composite<S,T>
        >
      >
    {
        static auto constexpr class_label="is_same_enough<S,T>";
        static void trace_tmpl()
        {  
           boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,FILE_SHORT,':',class_label));
           std::cout<<":S="<<demangle_type<S>()<<";\n";
           std::cout<<":T="<<demangle_type<T>()<<";\n";
           auto constexpr is_same_S_T=is_same<S,T>::value;
           std::cout<<":is_same_S_T="<<is_same_S_T<<";\n";
           if constexpr (!is_same_S_T)
           { boost::trace_scope ts("!is_same_S_T");
             auto constexpr is_less_equal_size=[]
               < typename T0=S
               , typename T1=T
               > 
               //"lazify" function by requiring template parameters.
               //Otherwise, could just replace T0->S and T1->T, in following
               //code.
               ()
             { 
               using size_0=fusion::result_of::size<T0>;
               using size_1=fusion::result_of::size<T1>;
               auto constexpr less_equal_v=mpl::less_equal<size_0,size_1>::value
                 //without "lazification", and using replacements T0->S and T1->T, 
                 //this might result in compile error when S or T is
                 //for example, double or int, or some type, X, where
                 //fusion::result_of::size<X> is not defined.
                 ;
               return less_equal_v;
             };
             auto constexpr is_var_S=is_variant<S>::value;
             std::cout<<":is_var_S="<<is_var_S<<";\n";
             if constexpr (is_var_S)
             { boost::trace_scope ts("is_var_S");
               auto constexpr is_var_T=is_variant<T>::value;
               std::cout<<":is_var_T="<<is_var_T<<";\n";
               if constexpr (is_var_T)
               { boost::trace_scope ts("is_var_T");
                 bool constexpr less_equal_size_var=is_less_equal_size();
                 std::cout<<":less_equal_size_var="<<less_equal_size_var<<";\n";
                 if constexpr (less_equal_size_var)
                 { boost::trace_scope ts("less_equal_size_var");
                   using is_same_enough_composite_var=is_same_enough_composite<S,T>;
                   is_same_enough_composite_var::trace_tmpl();
                   auto constexpr is_same_enough_composite_var_v=is_same_enough_composite_var::value;
                   std::cout<<":is_same_enough_composite_var_v="<<is_same_enough_composite_var_v<<";\n";
                 }
               }
             }
             else
             { boost::trace_scope ts("!is_var_S");
               auto constexpr is_seq_S=fusion::traits::is_sequence<S>::value;
               std::cout<<":is_seq_S="<<is_seq_S<<";\n";
               if constexpr (is_seq_S)
               { boost::trace_scope ts("is_seq_S");
                 auto constexpr is_seq_T=fusion::traits::is_sequence<T>::value;
                 std::cout<<":is_seq_T="<<is_seq_T<<";\n";
                 if constexpr (is_seq_T)
                 { boost::trace_scope ts("is_seq_T");
                   bool constexpr less_equal_size_seq=is_less_equal_size();
                   std::cout<<":less_equal_size_seq="<<less_equal_size_seq<<";\n";
                   if constexpr (less_equal_size_seq)
                   { boost::trace_scope ts("less_equal_size_seq");
                     using is_same_enough_composite_seq=is_same_enough_composite<S,T>;
                     is_same_enough_composite_seq::trace_tmpl();
                     auto constexpr is_same_enough_composite_seq_v=is_same_enough_composite_seq::value;
                     std::cout<<":is_same_enough_composite_seq_v="<<is_same_enough_composite_seq_v<<";\n";
                   }
                 }
               }
             }
           }
           std::cout<<"is_same_enough<S,T>:result="<<is_same_enough<S,T>::value<<";\n";
        }
    
    };

}}}}
#endif//USE_IS_SAME_ENOUGH
#pragma pop_macro("FILE_SHORT")
