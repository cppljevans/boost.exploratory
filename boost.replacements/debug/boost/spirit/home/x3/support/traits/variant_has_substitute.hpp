/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_VARIANT_HAS_SUBSTITUTE_APR_18_2014_925AM)
#define BOOST_SPIRIT_X3_VARIANT_HAS_SUBSTITUTE_APR_18_2014_925AM

#include <boost/spirit/home/x3/support/traits/is_substitute.hpp>
#include <boost/mpl/find.hpp>
#pragma push_macro("FILE_SHORT")
#define FILE_SHORT "traits/variant_has_substitute.hpp"

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename Variant, typename T>
    struct variant_has_substitute_impl
    {
        // Find a type from the Variant that can be a substitute for T.
        // return true_ if one is found, else false_

        typedef Variant variant_type;
        typedef typename variant_type::types types;
        typedef typename mpl::end<types>::type end;

        typedef typename mpl::find<types, T>::type iter_1;

        typedef typename
            mpl::eval_if<
                is_same<iter_1, end>,
              #ifdef USE_VARIANT_SUB_1_78_0
                mpl::find_if<types, traits::is_substitute<T, mpl::_1>>,
              #else
                mpl::find_if<types, traits::is_substitute<mpl::_1, T>>,
              #endif//USE_VARIANT_SUB_1_78_0
                mpl::identity<iter_1>
            >::type
        iter;

        typedef mpl::not_<is_same<iter, end>> type;
        
        static void trace_tmpl()
        { auto constexpr class_label="variant_has_substitute_impl<Variant, T>";
          boost::trace_scope ts(stringify(FILE_SHORT,':',class_label));
          std::cout<<":Variant=\n"<<demangle_fmt_type<Variant>()<<";\n";
          std::cout<<":T=\n"<<demangle_fmt_type<T>()<<";\n";
          std::cout<<":types=\n"<<demangle_fmt_type<types>()<<";\n";
          using same_iter_1_end=is_same<iter_1,end>;
          std::cout<<":is_same<iter_1,end>::value="<<same_iter_1_end::value<<";\n";
          if constexpr (same_iter_1_end::value)
          { using find_if_types_T=mpl::find_if<types, traits::is_substitute<T, mpl::_1>>;
            std::cout<<":find_if_types_T=\n"<<demangle_fmt_type<find_if_types_T>()<<";\n";
            std::cout<<":find_if_types_T::type=\n"<<demangle_fmt_type<typename find_if_types_T::type>()<<";\n";
          }
          std::cout<<":result="<<type::value<<";\n";
        }
    };

    template <typename Variant, typename T>
    struct variant_has_substitute
        : variant_has_substitute_impl<Variant, T>::type 
    {
        static void trace_tmpl()
        { variant_has_substitute_impl<Variant, T>::trace_tmpl();
        }
    };

    template <typename T>
    struct variant_has_substitute<unused_type, T> : mpl::true_ 
    {
        static void trace_tmpl()
        { auto constexpr class_label="variant_has_substitute<unused_type, T>";
          std::cout<<stringify(FILE_SHORT,':',class_label,":mpl::true_");
        }
    };

    template <typename T>
    struct variant_has_substitute<unused_type const, T> : mpl::true_ 
    {
        static void trace_tmpl()
        { auto constexpr class_label="variant_has_substitute<unused_type const, T>";
          std::cout<<stringify(FILE_SHORT,':',class_label,":mpl::true_");
        }
    };

}}}}

#pragma pop_macro("FILE_SHORT")
#endif
