/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_IS_SUBSTITUTE_JAN_9_2012_1049PM)
#define BOOST_SPIRIT_X3_IS_SUBSTITUTE_JAN_9_2012_1049PM

//#include <magic_enum_io.hpp>
#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
#include <boost/spirit/home/x3/support/traits/tuple_traits.hpp>
#include <boost/spirit/home/x3/support/traits/container_traits.hpp>
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
#include <boost/mpl/logical.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/optional/optional.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    // Find out if T can be a (strong) substitute for Attribute
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Attribute, typename Enable = void>
    struct is_substitute;

    template <typename Variant, typename Attribute>
    struct variant_has_substitute;

    template <typename Variant, typename Attribute>
    struct variant_has_substitute_impl;

    namespace detail
    {
        enum enum_tag
        { tag_value_type_is_substitute
        , tag_impl_default
        , tag_seq_seq_equal
        , tag_containers
        , tag_impl_variant
        , tag_sub_same_impl
        };
        template<enum_tag Tag>
        struct trace_tag{};
        
        template<typename T, typename Attribute>
        void trace_fun_tmpl(trace_tag<tag_value_type_is_substitute>)
        { std::cout<<__func__<<"<tag_value_type_is_substitute>:LINE="<<__LINE__<<";\n";
          using container_value_T=typename container_value<T>::type;
          using container_value_A=typename container_value<Attribute>::type;
          using super_t=is_substitute<container_value_T,container_value_A>;
          std::cout<<"super_t::trace_tmpl"; super_t::trace_tmpl();
        }
          
        template <typename T, typename Attribute>
        struct value_type_is_substitute
          : is_substitute<
                typename container_value<T>::type
              , typename container_value<Attribute>::type>
        {
            static auto constexpr struct_tag=tag_value_type_is_substitute;
            
            static void trace_tmpl()
            {  trace_tmpl<T,Attribute>(trace_tag<struct_tag>{});
            }
        };
        
        template<typename T, typename Attribute>
        void trace_fun_tmpl(trace_tag<tag_impl_default>)
        { std::cout<<__func__<<":tag_impl_default:LINE="<<__LINE__<<";\n";
          std::cout<<"result=false;\n";
        }
          
        template <typename T, typename Attribute, typename Enable = void>
        struct is_substitute_impl : mpl::false_ 
        {
            static auto constexpr struct_tag=tag_impl_default;
            static auto constexpr class_label="<T ,Attribute, Enable>";
            static void trace_tmpl()
            { std::cout<<"is_substitute_impl"<<class_label<<"::"<<__func__<<":LINE="<<__LINE__<<"==false_;\n";
            }
        };

        template<typename T, typename Attribute>
        void trace_fun_tmpl(trace_tag<tag_seq_seq_equal>)
        { std::cout<<__func__<<"<tag_seq_seq_equal>LINE="<<__LINE__<<";\n";
          using is_seq_T=fusion::traits::is_sequence<T>;
          std::cout<<"is_seq_T="<<is_seq_T::value<<";\n";
          using is_seq_Attribute=fusion::traits::is_sequence<Attribute>;
          std::cout<<"is_seq_Attribute="<<is_seq_Attribute::value<<";\n";
          if constexpr (is_seq_T::value && is_seq_Attribute::value)
          {
            using mpl_equal=typename
              mpl::equal
              < T
              , Attribute
              , is_substitute
              #ifdef USE_IS_SUBSTITUTE_1_78_0
                < mpl::_1
                , mpl::_2
              #else
                < mpl::_2
                , mpl::_1
              #endif
                > 
              >::type;
            std::cout<<"mpl_equal="<<mpl_equal::value<<";\n";
          }
        }
          
        template <typename T, typename Attribute>
        struct is_substitute_impl<T, Attribute,
            typename enable_if<
                mpl::and_<
                    fusion::traits::is_sequence<T>,
                    fusion::traits::is_sequence<Attribute>,
                    mpl::equal<T, Attribute, is_substitute<mpl::_1, mpl::_2>>
                >
              >::type>
          : mpl::true_ 
        {
            static auto constexpr struct_tag=tag_seq_seq_equal;
            static auto constexpr class_label="<is_sequence<T>,is_sequence<Attribute>,mpl_equal>";
            static void trace_tmpl()
            { std::cout<<"is_substitute_impl"<<class_label<<"::"<<__func__<<":LINE="<<__LINE__<<"==true_;\n";
              trace_fun_tmpl<T,Attribute>(detail::trace_tag<struct_tag>{});
            }
        };

        template<typename T, typename Attribute>
        void trace_fun_tmpl(trace_tag<tag_containers>)
        { std::cout<<__func__<<"<tag_containers>LINE="<<__LINE__<<";\n";
          bool constexpr is_container_T=is_container<T>::value;
          bool constexpr is_container_Attribute=is_container<Attribute>::value;
          std::cout<<":is_container<T>="<<is_container_T<<";\n";
          std::cout<<":is_container<Attribute>="<<is_container_Attribute<<";\n";
          using val_is_sub=value_type_is_substitute<T, Attribute>;
          auto constexpr struct_tag=val_is_sub::struct_tag;
          trace_fun_tmpl<T, Attribute>(trace_tag<struct_tag>{});
        }
        
        template <typename T, typename Attribute>
        struct is_substitute_impl<T, Attribute,
            typename enable_if<
                mpl::and_<
                    is_container<T>,
                    is_container<Attribute>,
                    value_type_is_substitute<T, Attribute>
                >
            >::type>
          : mpl::true_ 
        {
            static auto constexpr struct_tag=tag_containers;
            static auto constexpr class_label="<is_container<T>,is_container<Attribute>,value_type_is_substitute>";
            static void trace_tmpl()
            {  std::cout<<"is_substitute_impl"<<class_label<<":LINE="<<__LINE__<<"==true_;\n";
            }
        };
      #ifdef USE_ISSUE707_VAR2
        template <typename T, typename Attribute, typename IsVarT>
        struct is_substitute_impl_varT
        ;
        template <typename T, typename Attribute>
        struct is_substitute_impl_varT
        < T
        , Attribute
        , mpl::true_//T is variant
        >
        : mpl::true_//should check further.
        {
            static auto constexpr class_label="is_substitute_impl_varT<,,true_>";
            static void trace_tmpl()
            { boost::trace_scope ts(stringify(class_label,":LINE=",__LINE__));
              std::cout<<"T="<<demange_type_fmt<T>()<<";\n"; 
              std::cout<<"Attribute="<<demange_type_fmt<Attribute>()<<";\n"; 
            }
        };
        
        template <typename T, typename Attribute>
        struct is_substitute_impl_varT
        < T
        , Attribute
        , mpl::false_//T not variant
        >
          : variant_has_substitute
            < Attribute
            , T
            >
        {
            static auto constexpr class_label="is_substitute_impl_varT<,,false_>";
            static void trace_tmpl()
            { boost::trace_scope ts(stringify(class_label,":LINE=",__LINE__));
              std::cout<<"T="<<demange_type_fmt<T>()<<";\n"; 
              std::cout<<"Attribute="<<demange_type_fmt<Attribute>()<<";\n"; 
            }
        };
      #endif
      
        template <typename T, typename Attribute>
        struct is_substitute_impl<T, Attribute,
            typename enable_if<
                is_variant
                < Attribute
                >
            >::type>
        #ifdef USE_ISSUE707_VAR2
          : is_substitute_impl_varT<T, Attribute, typename is_variant<T>::type >
        #else
          : variant_has_substitute
            < Attribute
            , T
            >
        #endif
        {
            static auto constexpr class_label="is_substitute_impl:variant_has_substitute";
            static void trace_tmpl()
            {  boost::trace_scope ts(stringify(class_label,":LINE=",__LINE__));
               using var_has_sub=variant_has_substitute_impl<Attribute, T>;
               std::cout<<"var_has_sub="<<demangle_fmt_type<var_has_sub>()<<";\n";
               std::cout<<"var_has_sub::value="<<var_has_sub::type::value<<";\n";
               using end=typename var_has_sub::end;
               std::cout<<"end="<<demangle_fmt_type<end>()<<";\n";
               using iter_1=typename var_has_sub::iter_1;
               std::cout<<"iter_1="<<demangle_fmt_type<iter_1>()<<";\n";
               constexpr auto same_iter_1_end=is_same<iter_1, end>::value;
               std::cout<<"same_iter_1_end="<<same_iter_1_end<<";\n";
               if constexpr(same_iter_1_end)
               { using types=typename var_has_sub::types;
                 using iter=typename mpl::find_if<types, is_substitute<T, mpl::_1>>::type;
                 constexpr auto same_iter_end=is_same<iter, end>::value;
                 std::cout<<"same_iter_end="<<same_iter_end<<";\n";
               } 
            }
        };
    }

    template<typename T, typename Attribute, typename Enable>
    void trace_fun_tmpl(detail::trace_tag<detail::tag_sub_same_impl>)
    {
        std::cout<<__func__<<"<tag_sub_same_impl>;\n";
        std::cout<<"T="<<demangle_fmt_type<T>()<<";\n";
        std::cout<<"Attribute="<<demangle_fmt_type<Attribute>()<<";\n";
        std::cout<<"Enable="<<demangle_fmt_type<Enable>()<<";\n";
        using same_T_Attribute_t=is_same<T, Attribute>;
        constexpr auto same_T_Attribute_v=bool(same_T_Attribute_t());
        std::cout<<"same_T_Attribute_v="<<same_T_Attribute_v<<";\n";
        if constexpr (!same_T_Attribute_v)
        {  //so the is_same<T, Attriute> in this's superclass is false;
           //so check the alternative, is_substitute_impl<T, Attribute>:
           using is_subs_impl=detail::is_substitute_impl<T, Attribute>;
           std::cout<<"is_substitute::"<<__func__<<":__LINE__="<<__LINE__<<":is_subs_impl::trace_tmpl();\n";
           is_subs_impl::trace_tmpl();
           //examine the conditions for calling is_substitute_impl which inherits mpl::true_.
           using is_seq_T=fusion::traits::is_sequence<T>;
           std::cout<<"is_seq_T="<<is_seq_T::value<<";\n";
           using is_seq_Attribute=fusion::traits::is_sequence<Attribute>;
           std::cout<<"is_seq_Attribute="<<is_seq_Attribute::value<<";\n";
           if constexpr (is_seq_T::value && is_seq_Attribute::value)
           {
             std::size_t constexpr size_T=fusion::result_of::size<T>::value;
             std::cout<<":size_T="<<size_T<<";\n";
             std::size_t constexpr size_Attribute=fusion::result_of::size<Attribute>::value;
             std::cout<<":size_Attribute="<<size_Attribute<<";\n";
             bool size_same=(size_T==size_Attribute);
             std::cout<<":size_same="<<size_same<<";\n";
             std::size_t constexpr min_size=std::min(size_T,size_Attribute);
             using ndx_seq=
               std::make_index_sequence
               < min_size
               >;
             auto op_seq=
               []<std::size_t... ndxs>
               ( std::integer_sequence<std::size_t, ndxs...>
               )
               { std::cout<<"ndxs...\n";
                 std::cout<<indent_buf_in;
                 ( ( std::cout
                   <<"ndx="<<ndxs<<"\n"
                   <<indent_buf_in
                   <<"  T[ndx]=\n"<<demangle_fmt_type<typename fusion::result_of::at_c<T,ndxs>::type>()<<";\n"
                   <<"  Attribute[ndx]=\n"<<demangle_fmt_type<typename fusion::result_of::at_c<Attribute,ndxs>::type>()<<";\n"
                   <<"  is_substitute<T[ndx],Attribute[ndx]>="
                     << is_substitute
                        < typename fusion::result_of::at_c<T,ndxs>::type
                        , typename fusion::result_of::at_c<Attribute,ndxs>::type
                        >::type::value<<";\n"
                   <<indent_buf_out
                   <<"  is_substitute<T[ndx],Attribute[ndx]>::trace_tmpl()=\n"
                   , is_substitute
                     < typename fusion::result_of::at_c<T,ndxs>::type
                     , typename fusion::result_of::at_c<Attribute,ndxs>::type
                     >::trace_tmpl()
                   )
                 , ...
                 );
                 std::cout<<indent_buf_out;
                 std::cout<<"...ndxs\n";
               };
             op_seq(ndx_seq());
           }
           using mpl_equal=typename
             mpl::equal
             < T
             , Attribute
             , is_substitute
             #ifdef USE_IS_SUBSTITUTE_1_78_0
               < mpl::_1
               , mpl::_2
             #else
               < mpl::_2
               , mpl::_1
             #endif
               > 
             >::type;
           std::cout<<"mpl_equal="<<mpl_equal::value<<";\n";
           //if constexpr (!mpl_equal::value)
           {
           //Trace why !mpl_equal::value:
           //copy&past from boost/mpl/equal.hpp:struct equal_impl<Sequence1,Sequence2,Predicate>
             using Sequence1 = T;
             using Sequence2 = Attribute;
             using Predicate = 
               is_substitute
             #ifdef USE_IS_SUBSTITUTE_1_78_0
               < mpl::_1
               , mpl::_2
             #else
               < mpl::_2
               , mpl::_1
             #endif
               >;
             typedef typename mpl::begin<Sequence1>::type first1_;
             typedef typename mpl::begin<Sequence2>::type first2_;
             typedef typename mpl::end<Sequence1>::type last1_;
             typedef typename mpl::end<Sequence2>::type last2_;
         
             typedef mpl::aux::iter_fold_if_impl
                 < first1_
                 , first2_
                 , mpl::next<>
                 , mpl::protect< mpl::aux::equal_pred<Predicate,last1_,last2_> >
                 , mpl::void_
                 , mpl::always<mpl::false_>
                 > fold_;

             //using typename fold_::forward_step1;
             //^fails compile.
             typedef typename fold_::iterator iter1_;
             typedef typename fold_::state iter2_;
             using same_iter_last1_=is_same<iter1_,last1_>;
             using same_iter_last2_=is_same<iter2_,last2_>;
             std::cout<<":LINE="<<__LINE__<<":same_iter_last1_::value="<<same_iter_last1_::value<<";\n";
             std::cout<<":LINE="<<__LINE__<<":same_iter_last2_::value="<<same_iter_last2_::value<<";\n";
             if constexpr(!same_iter_last1_::value)
             {
               std::cout<<":LINE="<<__LINE__<<":iter1_="<<demangle_fmt_type<iter1_>()<<";\n";
             }
             if constexpr(!same_iter_last2_::value)
             {
               std::cout<<":LINE="<<__LINE__<<":iter2_="<<demangle_fmt_type<iter2_>()<<";\n";
             }
             typedef mpl::and_
                 < same_iter_last1_
                 , same_iter_last2_
                 > result_;
             //std::cout<<":LINE="<<__LINE__<<":result_="<<demangle_fmt_type<result_>()<<";\n";
             std::cout<<":LINE="<<__LINE__<<":result_::type::value="<<result_::type::value<<";\n";
           #if 0
             using is_iter1_t=fusion::is_fusion_iterator<iter1_>;
             std::cout<<"demangle is_iter1_t="<<demangle_fmt_type<is_iter1_t>()<<";\n";
             using is_iter2_t=fusion::is_fusion_iterator<iter2_>;
             std::cout<<"demangle is_iter2_t="<<demangle_fmt_type<is_iter2_t>()<<";\n";
             bool constexpr is_iter1_v=is_iter1_t::value;
             bool constexpr is_iter2_v=is_iter2_t::value;
             std::cout<<":LINE="<<__LINE__<<":is_iter1_v="<<is_iter1_v<<":is_iter2_v="<<is_iter2_v<<";\n";
             if constexpr(is_iter1_v && is_iter2_v)
             {
               using deref1_=typename mpl::deref<iter1_>::type;
               std::cout<<":LINE="<<__LINE__<<":deref1_=\n"<<demangle_fmt_type<deref1_>()<<";\n";
               using deref2_=typename mpl::deref<iter2_>::type;
               std::cout<<":LINE="<<__LINE__<<":deref2_=\n"<<demangle_fmt_type<deref2_>()<<";\n";
               using is_sub_derefs=is_substitute<deref1_,deref2_>;
               std::cout<<":LINE="<<__LINE__<<":is_sub_derefs="<<demangle_fmt_type<is_sub_derefs>()<<";\n";
               std::cout<<":LINE="<<__LINE__<<":is_sub_derefs::type="<<demangle_fmt_type<typename is_sub_derefs::type>()<<";\n";
               std::cout<<":LINE="<<__LINE__<<":is_sub_derefs::trace_tmpl=\n";
               is_sub_derefs::trace_tmpl();
             }
           #endif
           }
        }
    }
    
    template <typename T, typename Attribute, typename Enable /*= void*/>
    struct is_substitute
        : mpl::or_<
              is_same<T, Attribute>,
              detail::is_substitute_impl<T, Attribute>> 
    {
        static auto constexpr struct_tag=detail::tag_sub_same_impl;
        static auto constexpr class_label="is_substitute:or_<is_same,is_substitute_impl>";
        static void trace_tmpl()
        {  
           boost::trace_scope ts(stringify(class_label,":LINE=",__LINE__));
           trace_fun_tmpl<T,Attribute,Enable>(detail::trace_tag<struct_tag>{});
        }
    };

    template <typename T, typename Attribute, typename Enable>
    struct is_substitute_trace
        : is_substitute<T, Attribute, Enable>
    {
        static void trace_tmpl()
        { std::cout<<"is_substitute_trace<T,Attribute,Enable>:LINE="<<__LINE__<<";\n";
          auto constexpr struct_tag=is_substitute<T,Attribute,Enable>::struct_tag;
          is_substitute<T,Attribute,Enable>::trace_tmpl();
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

#endif
