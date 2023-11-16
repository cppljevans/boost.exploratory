/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2011 Jan Frederick Eick

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_EXTRACT_INT_APRIL_17_2006_0830AM)
#define BOOST_SPIRIT_X3_EXTRACT_INT_APRIL_17_2006_0830AM

#include <boost/spirit/home/x3/support/traits/move_to.hpp>
#include <boost/spirit/home/x3/support/numeric_utils/detail/extract_int.hpp>
#include <boost/assert.hpp>

namespace boost { namespace spirit { namespace x3
{
    ///////////////////////////////////////////////////////////////////////////
    //  Extract the prefix sign (- or +), return true if a '-' was found
    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator>
    inline bool
    extract_sign(Iterator& first, Iterator const& last)
    {
        (void)last;                  // silence unused warnings
        BOOST_ASSERT(first != last); // precondition

        // Extract the sign
        bool neg = *first == '-';
        if (neg || (*first == '+'))
        {
            ++first;
            return neg;
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Low level unsigned integer parser
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, unsigned Radix, unsigned MinDigits, int MaxDigits
      , bool Accumulate = false>
    struct extract_uint
    {
        // check template parameter 'Radix' for validity
        static_assert(
            (Radix >= 2 && Radix <= 36),
            "Error Unsupported Radix");

        template <typename Iterator>
        inline static bool call(Iterator& first, Iterator const& last, T& attr)
        {
            if (first == last)
                return false;

            typedef detail::extract_int<
                T
              , Radix
              , MinDigits
              , MaxDigits
              , detail::positive_accumulator<Radix>
              , Accumulate>
            extract_type;

            Iterator save = first;
            if (!extract_type::parse(first, last, attr))
            {
                first = save;
                return false;
            }
            return true;
        }

        template <typename Iterator, typename Attribute>
        inline static bool call(Iterator& first, Iterator const& last, Attribute& attr_)
        {
            // this case is called when Attribute is not T
            T attr;
            if (call(first, last, attr))
            {
                traits::move_to(attr, attr_);
                return true;
            }
            return false;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    // Low level signed integer parser
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, unsigned Radix, unsigned MinDigits, int MaxDigits>
    struct extract_int
    {
        // check template parameter 'Radix' for validity
        static_assert(
            (Radix == 2 || Radix == 8 || Radix == 10 || Radix == 16),
            "Error Unsupported Radix");

        template <typename Iterator>
        inline static bool call(Iterator& first, Iterator const& last, T& attr)
        {
          #ifdef TRACE_EXTRACT_INT_HPP
            boost::trace_scope ts(stringify("extract_int(Attribute *is* T)",__func__));
            std::cout<<":T="<<demangle_fmt_type<T>()<<";\n";
          #endif//TRACE_EXTRACT_INT_HPP
            if (first == last)
                return false;

            typedef detail::extract_int<
                T, Radix, MinDigits, MaxDigits>
            extract_pos_type;

            typedef detail::extract_int<
                T, Radix, MinDigits, MaxDigits, detail::negative_accumulator<Radix> >
            extract_neg_type;

            Iterator save = first;
            bool hit = extract_sign(first, last);
            if (hit)
                hit = extract_neg_type::parse(first, last, attr);
            else
                hit = extract_pos_type::parse(first, last, attr);

            if (!hit)
            {
                first = save;
                return false;
            }
            return true;
        }

        template <typename Iterator, typename Attribute>
        inline static bool call(Iterator& first, Iterator const& last, Attribute& attr_)
        {
          #ifdef TRACE_EXTRACT_INT_HPP
            boost::trace_scope ts(stringify("extract_int(Attribute *not* T)",__func__));
            std::cout<<":T="<<demangle_fmt_type<T>()<<";\n";
            std::cout<<":Attribute="<<demangle_fmt_type<Attribute>()<<";\n";
          #endif//TRACE_EXTRACT_INT_HPP
            // this case is called when Attribute is not T
            T attr;
            if (call(first, last, attr))
            {
                traits::move_to(attr, attr_);
                return true;
            }
            return false;
        }
    };
}}}

#endif
