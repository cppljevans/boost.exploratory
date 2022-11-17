//OriginalSource:
//  copy&pasted selected parts of test/x3/alternative.cpp on 2022-01-08
//Additions:
//  
/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#include <boost/utility/trace_scope.hpp>
#endif

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
//#include <boost/fusion/adapted/variant/detail/is_variant_impl.hpp>
//#include <boost/fusion/adapted/variant/detail/size_impl.hpp>
#include <boost/fusion/include/at.hpp>

#include <string>
#include <iostream>
#include <vector>
//{test.hpp
//OriginalSource:
//  copy&pasted selected parts of test/x3/test.hpp on 2022-01-08 
#include <boost/spirit/home/x3/core/parse.hpp>
#pragma push_macro("FILE_SHORT")
#define FILE_SHORT "alternative.cpp"
namespace x3 =  boost::spirit::x3;
template<typename Component>
using attribute_of_default=x3::traits::attribute_of<Component,x3::unused_type>;

namespace spirit_test
{
    template <typename Char, typename Parser, typename Attr>
    bool test_attr
      ( Char const* in
      , Parser const& p
      , Attr& attr
      , bool full_match = true
      )
    {
        Char const* last = in;
        while (*last)
            last++;
        return x3::parse(in, last, p, attr)
            && (!full_match || (in == last));
    }
}
//}test.hpp
namespace issue707
{
    struct A{}; A a;
    struct B{}; B b;
    struct C{}; C c;
    struct D{}; D d;
    struct uda_s1
      { 
        C c;
        A a;
        B b;
      }; 
    struct uda_s2
      { 
        A a;
        B b;
      }; 
    using uda_var=
      x3::variant
      < uda_s1
      , uda_s2
      >
      ; 
    struct uda_data
      {
        D d;
        uda_var v;
      };
}
    BOOST_FUSION_ADAPT_STRUCT(issue707::uda_s1, c, a, b);
    BOOST_FUSION_ADAPT_STRUCT(issue707::uda_s2, a, b);
    BOOST_FUSION_ADAPT_STRUCT(issue707::uda_data, d, v);
int
main()
{
  #ifdef USE_TRACE_SCOPE
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif//USE_TRACE_SCOPE
    using spirit_test::test_attr;

    using boost::spirit::x3::attr;
    bool result;
    { // conversion between alternatives
    #if 1
        struct X {};
        struct Y {};
        struct Z {};
        boost::variant<X, Y, Z> v;
        boost::variant<Y, X> x{X{}};
        v = x; // boost::variant that supports conversion
     
        auto const p = 'x' >> attr(x) | 'z' >> attr(Z{});
        result=test_attr("z", p, v);
        std::cout<<__LINE__<<":result="<<result<<";\n";
        if(result)
        {   result=(boost::get<Z>(&v) != nullptr);
            std::cout<<__LINE__<<":result="<<result<<";\n";
        }
        result=test_attr("x", p, v);
        std::cout<<__LINE__<<":result="<<result<<";\n";
        if(result)
        {   result=(boost::get<X>(&v) != nullptr);
            std::cout<<__LINE__<<":result="<<result<<";\n";
        }
    #endif
    }
    { // regression test for #679
    #if 1
      #if 1
        //this fails if #defined(USE_IS_SUBSTITUTE_IMPL_VAR2)
        using Qaz = std::vector<boost::variant<int>>;
        using Foo = std::vector<boost::variant<Qaz, int>>;
      #else
        //this passes even if #defined(USE_IS_SUBSTITUTE_IMPL_VAR2)
        std::cout<<":!defined(USE_IS_SUBSTITUTE_IMPL_VAR2)\n";
        using Qaz = boost::variant<int>;
        using Foo = boost::variant<Qaz, int>;
      #endif
        using Bar = std::vector<boost::variant<Foo, int>>;
        Bar bar;
        result=test_attr("abaabb", +('a' >> attr(Foo{}) | 'b' >> attr(int{})), bar);
        std::cout<<__LINE__<<":result="<<result<<";\n";
    #endif
    }
    { //regression test for issue 722:
      //  https://github.com/boostorg/spirit/issues/722
        { //as shown (approximately) here:
          //  https://github.com/boostorg/spirit/issues/722
        #if 0
            using namespace x3;
        
            char const* inp = "";
        
            struct A{}; A a;
            struct B{}; B b;
            struct C{}; C c;
            using X = boost::variant<A,B>;
            boost::variant< std::pair<A, X>, C> target;
            auto parser=
              ( ( ( attr(a) 
                  | attr(a)
                  )//should be just A, but
                   //instead its variant< A, A>
                >> attr(unused)  
                >>( attr(a) 
                  | attr(b) 
                  )//is variant< A, B>
                )//should be pair< A, variant< A, B>>
              | attr(c)
              ) //should be variant< pair< A, variant< A, B>>, C>
              ;
            using parser_type=decltype(parser);
            std::cout<<"parser_type=\n"<<demangle_fmt_type<parser_type>()<<";\n";
            using aof=attribute_of_default<parser_type>;
            std::cout << "attribute_of<parser_type>\n"<<demangle_fmt_type<typename aof::type>()<<";\n";
            aof::trace_tmpl();
          #if 0
            bool parsed = test_attr( inp, parser, target);
          #else
            typename aof::type aof_attr;
            bool parsed = test_attr( inp, parser, aof_attr);
          #endif
            std::cout << "parsed?" << parsed << ";\n";
        #endif
        }
        { //simplified test showning just the lack of collapsing A|A -> A
        #if 0
            using namespace x3;
            
            struct A{}; A a;
            auto parser=
              ( attr(a) 
              | attr(a)
              | attr(a)
              )
              ;
            using parser_type=decltype(parser);
            std::cout<<"parser_type=\n"<<demangle_fmt_type<parser_type>()<<";\n";
            using aof=attribute_of_default<parser_type>;
            std::cout << "attribute_of<parser_type>\n"<<demangle_fmt_type<typename aof::type>()<<";\n";
          #if 0
            std::cout<< "aof::trace_tmpl():\n";
            aof::trace_tmpl();
          #endif
        #define DO_PARSE
        #ifdef DO_PARSE
          #if 0
            boost::variant<A,A,A> attr;
          #else
            typename aof::type attr;
          #endif
            char const* inp = "";
            bool parsed = test_attr( inp, parser, attr);
            std::cout << "parsed?" << parsed << ";\n";
        #endif//DO_PARSE
        #endif
        }
    }
    return 0;
}
#pragma pop_macro("FILE_SHORT")
