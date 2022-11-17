/*=============================================================================
    Copyright (c) 2021 Larry Joe Evans
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_PARSER_OCTOBER_16_2008_0254PM)
#define BOOST_SPIRIT_X3_PARSER_OCTOBER_16_2008_0254PM

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/declval.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/support/traits/has_attribute.hpp>
#include <boost/spirit/home/x3/support/utility/sfinae.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/assert.hpp>
#include <string>
#include <sstream>

#include <boost/fusion/container/generation/make_deque.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>

#if !defined(BOOST_SPIRIT_X3_NO_RTTI)
#include <typeinfo>
#endif

#pragma push_macro("FILE_SHORT")
#define FILE_SHORT "unfold_left/*/core/parser.hpp"

namespace boost { namespace spirit { namespace x3
{
    template <typename Subject, typename Action>
    struct action;

    template <typename Subject, typename Handler>
    struct guard;

    struct parser_base {};
    struct parser_id;

    template <typename Derived>
    struct parser : parser_base
    {
        typedef Derived derived_type;
        static bool const handles_container = false;
        static bool const is_pass_through_unary = false;
        static bool const has_action = false;

        constexpr Derived const& derived() const
        {
            return *static_cast<Derived const*>(this);
        }

        template <typename Action>
        constexpr action<Derived, Action> operator[](Action f) const
        {
            return { this->derived(), f };
        }

        template <typename Handler>
        constexpr guard<Derived, Handler> on_error(Handler f) const
        {
            return { this->derived(), f };
        }
    };

    template <typename Parser, typename Enable = void>
    struct get_info
    {
        std::ostream& operator()(std::ostream& sout, Parser const& p) const
        {
            //std::cout<<FILE_SHORT<<':'<<__LINE__<<":get_info::"<<__func__<<"(std::ostream& sout, Parser const& p);\n";
            return sout<<demangle_fmt_type<Parser>();
        }
        std::string operator()(Parser const& p) const
        {
            //std::cout<<FILE_SHORT<<':'<<__LINE__<<":get_info::"<<__func__<<"(Parser const& p);\n";
            return demangle_fmt_type<Parser>();
        }
    };
    
    template <typename Parser>
    std::string what(Parser const& p)
    {
        return get_info<Parser>()(p);
    }
    template <typename Parser>
    std::ostream& what(std::ostream& sout, Parser const& p)
    {
        return get_info<Parser>()(sout, p);
    }

    struct unary_category;
    struct binary_category;
    struct n_ary_category;

    template <typename Subject, typename Derived>
    struct unary_parser : parser<Derived>
    {
        typedef unary_category category;
        typedef Subject subject_type;
        static bool const has_action = Subject::has_action;

        constexpr unary_parser(Subject const& subject)
            : subject(subject) {}

        unary_parser const& get_unary() const { return *this; }

        Subject subject;
    };

    template <typename Left, typename Right, typename Derived>
    struct binary_parser : parser<Derived>
    {
        typedef binary_category category;
        typedef Left left_type;
        typedef Right right_type;
        static bool const has_action =
            left_type::has_action || right_type::has_action;

        constexpr binary_parser(Left const& left, Right const& right)
            : left(left), right(right) {}

        binary_parser const& get_binary() const { return *this; }

        Left left;
        Right right;
    };
    
      template
      < std::size_t... IndicesLeft
      , std::size_t... IndicesRight
      , typename NaryLeft
      , typename NaryRight
      >
    constexpr auto n_ary_make_deque
      ( std::integer_sequence<std::size_t, IndicesLeft...>
      , std::integer_sequence<std::size_t, IndicesRight...>
      , NaryLeft const& left
      , NaryRight const& right
      )
      { return boost::fusion::make_deque
        ( left.template sub_parser<IndicesLeft>()...
        , right.template sub_parser<IndicesRight>()...
        );
      }
      template
      < std::size_t... IndicesLeft
      , typename NaryLeft
      , typename UnaryRight
      >
    constexpr auto n_ary_make_deque
      ( std::integer_sequence<std::size_t, IndicesLeft...>
      , NaryLeft const& left
      , UnaryRight const& right
      )
      { return boost::fusion::make_deque
        ( left.template sub_parser<IndicesLeft>()...
        , right
        );
      }
          
      template
      < template<typename...>typename Derived
      , typename Left
      , typename Right
      >
    constexpr auto n_ary_sub_parsers
      ( Left const& left
      , Right const& right
      )
      {
        return 
          boost::fusion::make_deque
          ( left
          , right
          );
      }

      template
      < template<typename...>typename Derived
      , typename... Left
      , typename Right
      >
    constexpr auto n_ary_sub_parsers
      ( Derived< Left...> const& left
      , Right const& right
      )
      { return
          n_ary_make_deque
          ( Derived< Left...>::indices
          , left
          , right
          );
      }

      template
      < template<typename...>typename Derived
      , typename... Left
      , typename... Right
      >
    constexpr auto n_ary_sub_parsers
      ( Derived< Left...> const& left
      , Derived< Right...> const& right
      )
      { return
          n_ary_make_deque
          ( Derived<Left...>::indices
          , Derived<Right...>::indices
          , left
          , right
          );
      }
            
    template <typename>
    struct n_ary_parser ;
    
      template 
      < template<typename...>typename Derived
      , typename... SubParsers
      >
    struct n_ary_parser
      < Derived<SubParsers...>
      > 
    : parser<Derived<SubParsers...>>
    {
        using sub_parsers_t=boost::fusion::deque<SubParsers...>;
        sub_parsers_t sub_parsers;
        typedef n_ary_category category;
        static bool const has_action =
          (  false
          || ...
          || SubParsers::has_action
          );

        static bool const has_attribute = true
          //even though may not be true for any SubParsers, delare true
          //when no collapsing, because this allows emplace use on attributes in 
          //Derived.
          ;
        constexpr n_ary_parser
          ( fusion::deque<SubParsers...> const& sp
          )
          : sub_parsers(sp) 
          {
            //std::cout<<FILE_SHORT<<':'<<__LINE__<<":"<<__func__<<":sub_parsers="<<demangle_fmt_type(sub_parsers)<<";\n";
          }
         
        static auto constexpr indices=std::index_sequence_for<SubParsers...>{};
        
        
        template<std::size_t I>
        auto& sub_parser(){ return fusion::at_c<I>(sub_parsers);}
        template<std::size_t I>
        constexpr auto const& sub_parser()const{ return fusion::at_c<I>(sub_parsers);}
    };
    
      template 
      < template<typename...>typename Derived
      , typename... SubParsers
      >
    struct get_info
      < Derived<SubParsers...>
      , std::enable_if_t
        < std::is_base_of_v
          < n_ary_parser
            < Derived<SubParsers...>
            >
          , Derived<SubParsers...>
          >
        >
      >
    {
        using Parser=Derived<SubParsers...>;
        
        std::ostream& operator()(std::ostream& sout, Parser const& p) const
        {
            //std::cout<<FILE_SHORT<<':'<<__LINE__<<":get_info::"<<__func__<<"(std::ostream& sout, Parser const& p);\n";
            sout<<demangle_fmt_type<Parser>();
            sout<<"\n";
            auto parser_what=[&p,&sout]<std::size_t Index>()
              { char constexpr delim=(Index==0)?'(':','
              ; sout<<delim<<" "
              ; sout<<indent_buf_in
              ; what(sout, p.template sub_parser<Index>())<<"\n"
              ; sout<<indent_buf_out
              ; return false
              ;};
            auto parsers_what=[&parser_what]<std::size_t ...Indices>(std::integer_sequence< std::size_t, Indices...>)
              { bool ingore[]=
                { parser_what.template operator()<Indices>()...
                }
              ;};
            parsers_what(Parser::indices);
            sout<<")";
            //sout<<"\n( "<<what(p.template sub_parser<0>())<<"\n)";
            sout.flush();
            return sout;
        }
        std::string operator()(Parser const& p) const
        {
            //std::cout<<FILE_SHORT<<':'<<__LINE__<<":get_info::"<<__func__<<"(Parser const& p);\n";
            std::ostringstream oss;
              boost::iostreams::indent_scoped_ostreambuf<char>
            indent_outbuf(oss,2);
            this->operator()(oss,p);
            std::string result=oss.str();
            //std::cout<<FILE_SHORT<<':'<<__LINE__<<":result=\n"<<result<<";\n";
            return result;
        }
    };
    
      template
      < template<typename...>typename Derived
      , typename... SubParsers
      >
    auto n_ary_make_parser
      ( fusion::deque<SubParsers...>const& sub_parsers
      )
      { 
      
        using derived_parser_t=
          Derived<SubParsers...>
          ;
      //#define TRACE_N_ARY_MAKE_PARSER
      #ifdef TRACE_N_ARY_MAKE_PARSER
        boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,':',__func__));
        std::cout
          <<":derived_parser_t=\n"<<demangle_fmt_type<derived_parser_t>()<<";\n"
          ;
      #endif//TRACE_N_ARY_MAKE_PARSER
        derived_parser_t derived_parser_v(sub_parsers);
      #ifdef TRACE_N_ARY_MAKE_PARSER
        get_info<derived_parser_t> get_info_v;
        std::cout<<__LINE__<<":get_info=\n"<<get_info_v(std::cout, derived_parser_v)<<";\n";
      #endif//TRACE_N_ARY_MAKE_PARSER
        return derived_parser_v;
      }
      template
      < template<typename...>typename Derived
      , typename Left
      , typename Right
      >
    auto n_ary_make_parser
      ( Left const& left
      , Right const& right
      )
      { auto sub_parsers=
          n_ary_sub_parsers<Derived>( left, right);
      #if 0
        std::cout<<FILE_SHORT<<':'<<__LINE__<<":"<<__func__<<";\n"
          <<":type<left>=\n"<<demangle_fmt_type(left)<<";\n"
          <<":type<right>=\n"<<demangle_fmt_type(right)<<";\n"
          <<":type<sub_parsers>=\n"<<demangle_fmt_type(sub_parsers)<<";\n"
          <<":left=\n"<<what(left)<<";\n"
          <<":right=\n"<<what(right)<<";\n"
          ;
      #endif
        return n_ary_make_parser<Derived>
          ( sub_parsers
          );
      }
    
    ///////////////////////////////////////////////////////////////////////////
    // as_parser: convert a type, T, into a parser.
    ///////////////////////////////////////////////////////////////////////////
    namespace extension
    {
        namespace detail
        {
            namespace as_parser_guard
            {
                void as_spirit_parser(...);

                template<typename T, typename R =
                    decltype(as_spirit_parser(boost::declval<T const&>()))>
                struct deduce_as_parser
                {
                    typedef R type;
                    typedef typename
                        boost::remove_cv<
                            typename boost::remove_reference<R>::type
                        >::type
                    value_type;

                    static type call(T const& v)
                    {
                        return as_spirit_parser(v);
                    }
                };
                template<typename T>
                struct deduce_as_parser<T, void>
                {};
            }
            using as_parser_guard::deduce_as_parser;
        }

        template <typename T, typename Enable = void>
        struct as_parser : detail::deduce_as_parser<T> {};

        template <>
        struct as_parser<unused_type>
        {
            typedef unused_type type;
            typedef unused_type value_type;
            static constexpr type call(unused_type)
            {
                return unused;
            }
        };

        template <typename Derived>
        struct as_parser<Derived
            , typename enable_if<is_base_of<parser_base, Derived>>::type>
        {
            typedef Derived const& type;
            typedef Derived value_type;
            static constexpr type call(Derived const& p)
            {
                return p;
            }
        };

        template <typename Derived>
        struct as_parser<parser<Derived>>
        {
            typedef Derived const& type;
            typedef Derived value_type;
            static constexpr type call(parser<Derived> const& p)
            {
                return p.derived();
            }
        };
    }

    template <typename T>
    constexpr typename extension::as_parser<T>::type
    as_parser(T const& x)
    {
        return extension::as_parser<T>::call(x);
    }

    template <typename Derived>
    constexpr Derived const&
    as_parser(parser<Derived> const& p)
    {
        return p.derived();
    }

}}}

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename Subject, typename Derived, typename Context>
    struct has_attribute<x3::unary_parser<Subject, Derived>, Context>
        : has_attribute<Subject, Context> {};

    template <typename Left, typename Right, typename Derived, typename Context>
    struct has_attribute<x3::binary_parser<Left, Right, Derived>, Context>
        : mpl::bool_<has_attribute<Left, Context>::value ||
                has_attribute<Right, Context>::value> {};
}}}}
#pragma pop_macro("FILE_SHORT")
#endif
