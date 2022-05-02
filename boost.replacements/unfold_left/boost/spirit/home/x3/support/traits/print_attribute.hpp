/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
================================================_==============================*/
#if !defined(BOOST_SPIRIT_X3_PRINT_ATTRIBUTE_JANUARY_20_2013_0814AM)
#define BOOST_SPIRIT_X3_PRINT_ATTRIBUTE_JANUARY_20_2013_0814AM

#include <boost/variant.hpp>
#ifdef USE_UNFOLD_LEFT
#include <boost/spirit/home/x3/support/ast/std_variant.hpp>
#endif//USE_UNFOLD_LEFT
#include <boost/optional/optional.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_category.hpp>
#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
#ifdef BOOST_SPIRIT_X3_UNICODE
# include <boost/spirit/home/support/char_encoding/unicode.hpp>
#endif

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename Out, typename T>
    void print_attribute(Out& out, T const& val);

    template <typename Out>
    inline void print_attribute(Out& out, unused_type const& val) 
    { out<<"**unused**";}

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename Out>
        struct print_fusion_sequence
        {
            print_fusion_sequence(Out& out)
              : out(out), count(0) {}

            typedef void result_type;

            template <typename T>
            void operator()(T const& val) const
            {
                if (count>0)
                    out << ", ";
                //out<<"count="<<count<<":demange_type(val)="<<demangle_type(val)<<":val=";
                x3::traits::print_attribute(out, val);
                ++count;
            }

            Out& out;
            mutable unsigned count;
        };

        // print elements in a variant
        template <typename Out>
        struct print_visitor : static_visitor<>
        {
            print_visitor(Out& out) : out(out) {}

            template <typename T>
            void operator()(T const& val) const
            {
                x3::traits::print_attribute(out, val);
            }

            Out& out;
        };
    }

    template <typename Out, typename T, typename Enable = void>
    struct print_attribute_debug
    {
        // for unused_type
        static void call(Out& out, unused_type uu, unused_attribute)
        {
            out << unused_type::as_string();
        }

        // for plain data types
        template <typename T_>
        static void call(Out& out, T_ const& val, plain_attribute)
        {
          #if 0
            out<<__func__<<":"<<demangle_type<T_>()<<":";
          #endif
            out << val;
        }
      #ifdef USE_UNFOLD_LEFT
        template <typename ...T_>
        static void call(Out& out, std_variant<T_...> const& val, plain_attribute)
        {
          #if 1
            out<<__func__<<"std_variant:";
          #endif
            out << val;
        }
      #endif//USE_UNFOLD_LEFT
#ifdef BOOST_SPIRIT_X3_UNICODE
        static void call(Out& out, char_encoding::unicode::char_type val, plain_attribute)
        {
            out<<"char_type:";
            if (val >= 0 && val < 127)
            {
              if (iscntrl(val))
                out << "\\" << std::oct << int(val) << std::dec;
              else if (isprint(val))
                out << char(val);
              else
                out << "\\x" << std::hex << int(val) << std::dec;
            }
            else
              out << "\\x" << std::hex << int(val) << std::dec;
        }

        static void call(Out& out, char val, plain_attribute tag)
        {
            //out<<__FILE__<<":"<<__LINE__<<":"<<__func__<<";\n";
            call(out, static_cast<char_encoding::unicode::char_type>(val), tag);
        }
#else        
        static void call(Out& out, char val, plain_attribute)
        {
            out<<"char:"<<val;
        }
#endif

        // for fusion data types
        template <typename T_>
        static void call(Out& out, T_ const& val, tuple_attribute)
        {
          #if 0
            out<<__FILE__<<":"<<__LINE__<<":"<<__func__<<":demangle_type(val)="<<demangle_type(val)<<";\n";
          #endif
            out << "tuple{";
            fusion::for_each(val, detail::print_fusion_sequence<Out>(out));
            out << '}';
        }

        // stl container
        static void call(Out& out, std::string const& val, container_attribute)
        {
            out << "string["<<val<<"]";
        }
        template <typename T_>
        static void call(Out& out, T_ const& val, container_attribute)
        {
            out
          #if 0
            #ifdef USE_DEMANGLE_FMT
              << demangle_fmt_type<T_>()
            #else
              << demangle_type<T_>()
            #endif
              << ":"
          #endif
              <<"container[";
            if (!traits::is_empty(val))
            {
                bool first = true;
                typename container_iterator<T_ const>::type iend = traits::end(val);
                for (typename container_iterator<T_ const>::type i = traits::begin(val);
                     !traits::compare(i, iend); traits::next(i))
                {
                    if (!first)
                        out << ", ";
                    first = false;
                    x3::traits::print_attribute(out, traits::deref(i));
                }
            }
            out << ']';
        }

        // for variant types
        template <typename T_>
        static void call(Out& out, T_ const& val, variant_attribute)
        {
            //out<<__FILE__<<":"<<__LINE__<<":"<<__func__<<":variant_attribute;\n";
            out<<"variant( "<<val.which()<<", ";
            apply_visitor(detail::print_visitor<Out>(out), val);
            out<<")";
        }

        // for optional types
        template <typename T_>
        static void call(Out& out, T_ const& val, optional_attribute)
        {
            //out<<__FILE__<<":"<<__LINE__<<":"<<__func__<<";\n";
            if (val)
                x3::traits::print_attribute(out, *val);
            else
                out << "[empty]";
        }

        // main entry point
        static void call(Out& out, T const& val)
        { 
          #if 0
            out
            #ifdef USE_DEMANGLE_FMT
              << demangle_fmt_type<T>()
            #else
              << demangle_type<T_>()
            #endif
              <<"\n"<<__FILE__<<":main entry point\n;";
          #endif
            call(out, val, typename attribute_category<T>::type());
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Out, typename T>
    inline void print_attribute(Out& out, T const& val)
    {
        print_attribute_debug<Out, T>::call(out, val);
    }
}}}}

#endif
