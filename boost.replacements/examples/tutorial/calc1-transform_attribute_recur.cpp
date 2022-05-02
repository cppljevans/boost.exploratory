/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2022 Larry Joe Evans

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  This example is based on that here:
//
//    https://github.com/boostorg/spirit/blob/develop/example/x3/calc/calc1.cpp
//    
//  However, instead of using the macro BOOST_SPIRIT_DEFINE to connect the rule
//  to it's RHS, is uses var_{enum,attr,def,expr} entities to connect
//  var_expr<ENUM> to var_def<ENUM> and out-of-line transform_attribute's
//  to transform the var_def<ENUM> attributes to var_expr<ENUM> attributes (as done in 
//  current spirit using it's transform_attribute template).
//
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <type_traits>

namespace boost{namespace spirit{namespace x3
{
  template<typename Parser>
using attribute_of_default=typename
  traits::attribute_of
  < std::remove_const_t<Parser>
  , x3::unused_type
  >::type
  ;
}}}

#include <boost/fusion/include/adapt_struct.hpp>

#include <iostream>
#include <string>

namespace x3 = boost::spirit::x3;

namespace client
{
    namespace calculator_grammar
    {
        using x3::uint_;

          enum 
        var_enum
          { enum_expression
          , enum_term
          , enum_factor
          };
          template
          < var_enum Varble
          >
          struct
        var_attr
          {
            int val;
            var_attr():val(-10000)//bogus value
            {}
            
              friend std::ostream& 
            operator<<
              ( std::ostream&sout
              , var_attr const&x
              )
              { return sout<<x.val<<' '
              ;}
          };
          template
          < var_enum Varble
          >
          struct
        var_def
          ;
          template
          < var_enum Varble
          >
          struct
        var_expr
          ;
      //#define SPECIALIZE_RECURRENCE
      #ifdef SPECIALIZE_RECURRENCE
          template
          <
          >
          struct
        var_expr
          < enum_expression
          >
          ;
      #endif//SPECIALIZE_RECURRENCE
          template
          < var_enum Varble
          >
          struct
        var_expr
          : x3::parser
            < var_expr
              < Varble
              >
            >
          , var_attr< Varble>  
          {
              using
            to_attribute=
              var_attr< Varble>
              ;
              using
            attribute_type=to_attribute
              ;
            #define USE_VAR_INLINE_PARSE
            #ifndef USE_VAR_INLINE_PARSE
            #else
              //This produces a compile-time error,
              //but **ONLY** for Varble==enum_expression,
              //which suggests that maybe if, somehow, a specialization
              //were made **JUST** for enum_expression, then
              //the error could be eliminated.
              struct 
            transform_attribute
              { 
                  using
                from_parser=
                  typename var_def< Varble>::type
                  ;
                  using
                from_attribute=x3::attribute_of_default<from_parser>
                  ;
                  from_attribute
                pre(to_attribute&)
                  ;
                  void
                post(to_attribute&, from_attribute const&)
                  ;
              };
            #endif//USE_VAR_INLINE_PARSE
              template<typename It, typename Ctx, typename RCtx>
              bool 
            parse(It& f, It l, Ctx&, RCtx&, to_attribute& to_attr) const
            #ifndef USE_VAR_INLINE_PARSE
              ;
            #else
              {
                  auto
                from_attr=transform_attribute::pre(to_attr);
                  using
                from_parser=
                  typename transform_attribute::from_parser
                  ;
                  from_parser 
                from_p;
                bool result=
                  from_p.parse
                  ( f
                  , l
                  , x3::blank
                  , from_attr
                  );
                if(result)
                {
                    transform_attribute::post(to_attr,from_attr);
                }
                return result;
              }
            #endif//USE_VAR_INLINE_PARSE
          };
          
        var_expr<enum_expression> const expression;
        var_expr<enum_term> const term;
        var_expr<enum_factor> const factor;

        auto const expression_def =
            term
            >> *(   ('+' >> term)
                |   ('-' >> term)
                )
            ;

        auto const term_def =
            factor
            >> *(   ('*' >> factor)
                |   ('/' >> factor)
                )
            ;

        auto const factor_def =
                uint_
            |   '(' >> expression >> ')'
            |   ('-' >> factor)
            |   ('+' >> factor)
            ;

        auto calculator = expression;
        
      #ifndef USE_VAR_INLINE_PARSE
          template
          <
          >
          struct
        var_def
          < enum_expression
          >
          {
            using type=decltype(expression_def);
            static constexpr type*const value=boost::addressof(expression_def);
          };
          template
          <
          >
          struct
        var_def
          < enum_term
          >
          {
            using type=decltype(term_def);
            static constexpr type*const value=boost::addressof(term_def);
          };
          template
          <
          >
          struct
        var_def
          < enum_factor
          >
          {
            using type=decltype(factor_def);
            static constexpr type*const value=boost::addressof(factor_def);
          };
          template
          < var_enum Varble
          >
              template<typename It, typename Ctx, typename RCtx>
              bool 
        var_expr
          < Varble
          >::
            parse(It& f, It l, Ctx&ctx, RCtx&rctx, to_attribute& to_attr) const
              { std::string now(f,l);
                std::cout<<":now="<<now<<";\n";
                using from_parser_t=typename var_def<Varble>::type;
                from_parser_t*from_parser_v=var_def<Varble>::value;
                using from_attr_t=x3::attribute_of_default<from_parser_t>;
                from_attr_t from_attr_v;
                bool result=
                  from_parser_v->parse
                  ( f
                  , l
                  , ctx
                  , rctx
                  , from_attr_v
                  );
                return result;
              }
      #endif//USE_VAR_INLINE_PARSE
    }

    using calculator_grammar::calculator;

}

///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int
main()
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "Expression parser...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";

    typedef std::string::const_iterator iterator_type;

    std::string strs[]=
    { "1"
    , "1+2"
    , "1+2*3"
    };
    for( std::string str: strs)
    {
        auto& calc_gram = client::calculator;    // Our grammar

        iterator_type iter = str.begin();
        iterator_type end = str.end();
        boost::spirit::x3::ascii::space_type space;
          client::calculator_grammar::var_attr
          < client::calculator_grammar::enum_expression
          >
        calc_attr
          ;
        bool r = phrase_parse(iter, end, calc_gram, space, calc_attr);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            std::cout << "-------------------------\n";
            std::cout << "calc_attr=" << calc_attr <<"\n";
        }
        else
        {
            std::string rest(iter, end);
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "stopped at: \"" << rest << "\"\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}
