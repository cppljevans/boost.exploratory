/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_MOVE_TO_JAN_17_2013_0859PM)
#define BOOST_SPIRIT_X3_MOVE_TO_JAN_17_2013_0859PM

#include <boost/spirit/home/x3/support/traits/attribute_category.hpp>
#include <boost/spirit/home/x3/support/traits/tuple_traits.hpp>
#include <boost/spirit/home/x3/support/traits/variant_has_substitute.hpp>

#include <boost/assert.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/move.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <utility>

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename Source, typename Dest>
    inline void move_to(Source&& src, Dest& dest);

    template <typename T>
    inline void move_to(T& src, T& dest);

    template <typename T>
    inline void move_to(T const& src, T& dest);

    template <typename T>
    inline void move_to(T&& src, T& dest);

    template <typename Iterator, typename Dest>
    inline void move_to(Iterator first, Iterator last, Dest& dest);

    template <typename Dest>
    inline void move_to(unused_type, Dest&) {}

    template <typename Source>
    inline void move_to(Source&, unused_type) {}

    inline void move_to(unused_type, unused_type) {}

    template <typename Iterator>
    inline void
    move_to(Iterator, Iterator, unused_type) {}

    namespace detail
    {
        template <typename Source, typename Dest>
        inline void
        move_to(Source&, Dest&, unused_attribute) {}
        
        template <typename Source, typename Dest>
        inline void
        move_to_plain(Source& src, Dest& dest, mpl::false_) // src is not a single-element tuple
        {
            boost::trace_scope ts(stringify(__func__,"(Source&,Dest&,false_):LINE=",__LINE__));
            dest = std::move(src);
        }
        
        template <typename Source, typename Dest>
        inline void
        move_to_plain(Source& src, Dest& dest, mpl::true_) // src is a single-element tuple
        {
            boost::trace_scope ts(stringify(__func__,"(Source,&Dest&,true_):LINE=",__LINE__));
            dest = std::move(fusion::front(src));
        }

        template <typename Source, typename Dest>
        inline void
        move_to(Source& src, Dest& dest, plain_attribute)
        {
            typename mpl::and_<
                fusion::traits::is_sequence<Source>,
                is_size_one_sequence<Source> >
            is_single_element_sequence;
        
            move_to_plain(src, dest, is_single_element_sequence);
        }

        template <typename Source, typename Dest>
        inline typename enable_if<is_container<Source>>::type
        move_to(Source& src, Dest& dest, container_attribute)
        {
            boost::trace_scope ts(stringify(__func__,"(Source&,Dest&,container_attribute):LINE=",__LINE__));
            traits::move_to(src.begin(), src.end(), dest);
        }

        template <typename Source, typename Dest>
        inline typename enable_if<
            mpl::and_<
                is_same_size_sequence<Dest, Source>,
                mpl::not_<is_size_one_sequence<Dest> > >
        >::type
        move_to(Source& src, Dest& dest, tuple_attribute)
        {
            boost::trace_scope ts(stringify(__func__,":LINE=",__LINE__));
            fusion::move(std::move(src), dest);
        }

        template <typename Source, typename Dest>
        inline typename enable_if<
            is_size_one_sequence<Dest>
        >::type
        move_to(Source& src, Dest& dest, tuple_attribute)
        {
            boost::trace_scope ts(stringify(__func__,"(Source&,Dest&,tuple_attribute):LINE=",__LINE__));
            traits::move_to(src, fusion::front(dest));
        }

        template <typename Source, typename Dest>
        inline void
        move_to(Source& src, Dest& dest, variant_attribute, mpl::false_)
        {
            boost::trace_scope ts(stringify(__func__,"(Source&,Dest&,variant_attribute,false_):LINE=",__LINE__));
            std::cout<<"type(src)="<<demangle_fmt_type(src)<<";\n";
            std::cout<<"type(dest)="<<demangle_fmt_type(src)<<";\n";
            dest = std::move(src);
        }
        
        template <typename Source, typename Dest>
        inline void
        move_to_variant_from_single_element_sequence(Source& src, Dest& dest, mpl::false_)
        {
            // dest is a variant, src is a single element fusion sequence that the variant
            // cannot directly hold. We'll try to unwrap the single element fusion sequence.
            
            // Make sure that the Dest variant can really hold Source
            static_assert(variant_has_substitute<Dest, typename fusion::result_of::front<Source>::type>::value,
                "Error! The destination variant (Dest) cannot hold the source type (Source)");
            
            boost::trace_scope ts(stringify(__func__,"(Source&,Dest&,false_):LINE=",__LINE__));
            dest = std::move(fusion::front(src));
        }
        
        template <typename Source, typename Dest>
        inline void
        move_to_variant_from_single_element_sequence(Source& src, Dest& dest, mpl::true_)
        {
            // dest is a variant, src is a single element fusion sequence that the variant
            // *can* directly hold.
            boost::trace_scope ts(stringify(__func__,"(Source&,Dest&,true_):LINE=",__LINE__));
            dest = std::move(src);
        }

        template <typename Source, typename Dest>
        inline void
        move_to(Source& src, Dest& dest, variant_attribute, mpl::true_)
        {
            boost::trace_scope ts(stringify(__func__,"(Source&,Dest&,variant_attribute,true_):LINE=",__LINE__));
            move_to_variant_from_single_element_sequence(src, dest, variant_has_substitute<Dest, Source>());
        }

        template <typename Source, typename Dest>
        inline void
        move_to(Source& src, Dest& dest, variant_attribute tag)
        {
            move_to(src, dest, tag, is_size_one_sequence<Source>());
        }

        template <typename Source, typename Dest>
        inline void
        move_to(Source& src, Dest& dest, optional_attribute)
        {
            boost::trace_scope ts(stringify(__func__,"(Source&,Dest&,optional_attribute):LINE=",__LINE__));
            dest = std::move(src);
        }

        template <typename Iterator>
        inline void
        move_to(Iterator, Iterator, unused_type, unused_attribute) {}

        template <typename Iterator, typename Dest>
        inline void
        move_to(Iterator first, Iterator last, Dest& dest, container_attribute)
        {
            boost::trace_scope ts(stringify(__func__,"(first,last,dest,container_attribute):LINE=",__LINE__));
            if (is_empty(dest))
                dest = Dest(first, last);
            else
                append(dest, first, last);
        }

        template <typename Iterator, typename Dest>
        inline typename enable_if<
            is_size_one_sequence<Dest>
        >::type
        move_to(Iterator first, Iterator last, Dest& dest, tuple_attribute)
        {
            boost::trace_scope ts(stringify(__func__,"(first,last,dest,tuple_attribute):LINE=",__LINE__));
            traits::move_to(first, last, fusion::front(dest));
        }
        
        template <typename Iterator>
        inline void
        move_to(Iterator first, Iterator last, boost::iterator_range<Iterator>& rng, range_attribute)
        {
            boost::trace_scope ts(stringify(__func__,"(first,last,rng,range_attribute):LINE=",__LINE__));
            rng = {first, last};
        }
    }

    template <typename Source, typename Dest>
    inline void move_to(Source&& src, Dest& dest)
    {
        detail::move_to(src, dest, typename attribute_category<Dest>::type());
    }

    template <typename T>
    inline void move_to(T& src, T& dest)
    {
        boost::trace_scope ts(stringify(__func__,"(T&,T&):LINE=",__LINE__));
        std::cout<<":T="<<demangle_fmt_type<T>()<<":src="<<src<<":dest="<<dest<<";\n";
        BOOST_ASSERT(boost::addressof(src) != boost::addressof(dest));
        dest = std::move(src);
    }

    template <typename T>
    inline void move_to(T const& src, T& dest)
    {
        boost::trace_scope ts(stringify(__func__,"(T const&,T&):LINE=",__LINE__));
        std::cout<<":T="<<demangle_fmt_type<T>();
      #if 0
        std::cout<<":src=";
        std::cout<<src;
        std::cout<<":dest=";
        std::cout<<dest;
        std::cout<<";\n";
      #endif
        BOOST_ASSERT(boost::addressof(src) != boost::addressof(dest));
        dest = src;
    }

    template <typename T>
    inline void move_to(T&& src, T& dest)
    {
        boost::trace_scope ts(stringify(__func__,"(T&&,T&):LINE=",__LINE__));
        BOOST_ASSERT(boost::addressof(src) != boost::addressof(dest));
        dest = std::move(src);
        std::cout<<":T="<<demangle_fmt_type<T>()<<":dest="<<dest<<";\n";
    }

    template <typename Iterator, typename Dest>
    inline void move_to(Iterator first, Iterator last, Dest& dest)
    {
        // $$$ Use std::move_iterator when iterator is not a const-iterator $$$
        detail::move_to(first, last, dest, typename attribute_category<Dest>::type());
    }
}}}}

#endif
