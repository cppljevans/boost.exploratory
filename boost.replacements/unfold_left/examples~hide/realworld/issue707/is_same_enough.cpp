#include <boost/spirit/home/x3/support/traits/and_or_values.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/spirit/home/x3/support/ast/get_put_index_variant.hpp>
#include <iostream>
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    // Find out if S and T are similar enough to be considered same.
    ///////////////////////////////////////////////////////////////////////////
    template <typename S, typename T>
    struct is_same_enough
    ;
    template < typename T>
    struct is_variant
    : mpl::false_
    {};
    template < typename... T>
    struct is_variant
      < boost::variant<T...>
      >
    : mpl::true_
    {};
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
    : and_values
      < is_same_enough
        < S
        , T
        >...
      >
    {
      static void trace_tmpl()
      { std::cout<<__LINE__<<":is_same_enough_composite<U<S...>,V<T...>>;\n"
      ;}
    };
      template 
      < typename S
      , typename T
      , typename I
      >
    struct is_same_enough_adapted
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
      : and_values
        < is_same_enough
          < typename fusion::result_of::at_c<S,I>::type
          , typename fusion::result_of::at_c<T,I>::type
          >...
        >
      {
        static void trace_tmpl()
        { std::cout<<__LINE__<<":is_same_enough_adapted;\n"
        ; std::cout<<"sizeof...(I)="<<sizeof...(I)<<";\n"
        ; auto same_enough_lambda=[]<std::size_t Ndx>()
          { std::cout<<":Ndx="<<Ndx<<";\n"
          ; using Si=typename fusion::result_of::at_c<S,Ndx>::type         
          ; std::cout<<":S<Ndx>="<<demangle_type<Si>()<<";\n"
          ; using Ti=typename fusion::result_of::at_c<T,Ndx>::type         
          ; std::cout<<":T<Ndx>="<<demangle_type<Ti>()<<";\n"
          ; auto constexpr is_same_v=is_same_enough<Si,Ti>::type::value
          ; std::cout<<":is_same_v="<<is_same_v<<";\n"
          ; return is_same_v
          ;}
        ; int is_sames[]={same_enough_lambda.template operator()<I>()...}
        ;}
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
        < fusion::result_of::size<T>::type::value
        >
      >
    {
      static void trace_tmpl()
      { std::cout<<__LINE__<<":is_same_enough_composite<U<S...>,T>;\n"
      ; using test=
        is_same_enough_adapted
        < U<S...>
        , T
        , std::make_index_sequence
          < fusion::result_of::size<T>::type::value
          >
        >
      ; std::cout<<__LINE__<<":test="<<test::type::value<<";\n"
      ; test::trace_tmpl()
      ;}
    };
    ///////////////////////////////////////////////////////////////////////////
    // Find out if S and T are similar enough to be considered same.
    ///////////////////////////////////////////////////////////////////////////
    template <typename S, typename T>
    struct is_same_enough
    : or_values
      < is_same<S,T>
      , and_values
        < is_variant<S>
        , is_variant<T>
        , is_same_enough_composite<S,T>
        >
      , and_values
        < fusion::traits::is_sequence<S>
        , fusion::traits::is_sequence<T>
        , is_same_enough_composite<S,T>
        >
      >
    {
      static void trace_tmpl()
      { std::cout<<__LINE__<<":is_same_enough<S,T>;\n"
      ; auto constexpr same_S_T=is_same<S,T>::type::value
      ; std::cout<<":is_same<S,T>="<<same_S_T<<";\n"
      ; if constexpr(same_S_T)
        {}
        else
        { auto constexpr is_var_S=is_variant<S>::type::value
        ; std::cout<<":is_var_S="<<is_var_S<<";\n"
        ; auto constexpr is_var_T=is_variant<T>::type::value
        ; std::cout<<":is_var_T="<<is_var_T<<";\n"
        ; auto constexpr is_var_S_T=is_var_S&&is_var_T
        ; if constexpr(is_var_S_T)
          {}
          else
          { auto constexpr is_seq_S=fusion::traits::is_sequence<S>::type::value
          ; std::cout<<":is_seq_S="<<is_seq_S<<";\n"
          ; auto constexpr is_seq_T=fusion::traits::is_sequence<T>::type::value
          ; std::cout<<":is_seq_T="<<is_seq_T<<";\n"
          ; auto constexpr is_seq_S_T=is_seq_S&&is_seq_T
          ; if constexpr(is_seq_S_T)
            { using test=is_same_enough_composite<S,T>
            ; std::cout<<__LINE__<<":is_same_enough_composite<S,T>="<<test::type::value<<";\n" 
            ; test::trace_tmpl()
            ;}
          ;}
        ;}
      ;}
    };
    
}}}}
  template
  < unsigned I
  >
  struct
type
  {};
using namespace boost::spirit::x3;
using namespace boost::fusion;
  template< typename S, typename T>
  using 
is_same_enough_composite=
  boost::spirit::x3::traits::is_same_enough_composite<S,T>
  ;
  template< typename S, typename T>
  using 
is_same_enough=
  boost::spirit::x3::traits::is_same_enough<S,T>
  ;
#include <boost/fusion/include/deque.hpp>  
#define USE_ADAPT_STRUCT
#ifdef USE_ADAPT_STRUCT
struct uda1//User Defined Attribute 1
{
    type<0> f0;
    type<1> f1;
};
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(uda1, f0, f1);

namespace boost{namespace fusion{
struct struct_tag{};//just so demangle_type used below works.
}}
#endif//USE_ADAPT_STRUCT

  int
main()
  {
#if 0  
  ; {
    ; using test=is_same_enough_composite
      < std_variant
        < type<0>
        , type<1>
        >
      , variant
        < type<0>
        , type<1>
        >
      >;
    ; std::cout<<__LINE__<<':'<<test::value<<";\n";
    ;}
#endif//0
#if 0  
  ; {
    ; using test=is_same_enough_composite
      < deque
        < type<0>
        , type<1>
        >
      , deque
        < type<0>
        , type<1>
        >
      >;
    ; std::cout<<__LINE__<<':'<<test::value<<";\n";
    ;}
#endif//0
#ifdef USE_ADAPT_STRUCT
  ; {
    ; using aof1=
        deque
        < type<0>
        , type<1>
        >
        ;
  #if 1
    ; using test=is_same_enough
      < aof1
      , uda1
      >;
    ; std::cout<<__LINE__<<':'<<test::value<<";\n";
    ; std::cout<<":test=\n"<<demangle_fmt_type<test>()<<";\n"
    ; if(!test::value)
      { test::trace_tmpl()
      ;}
  #endif
    ;}
#endif//USE_ADAPT_STRUCT
  ; return 0
  ;}      
