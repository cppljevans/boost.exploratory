//Purpose:
//  provide test case for issue707:
//    
/*
https://github.com/boostorg/spirit/issues/707
 */
//=============
//#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#include <boost/utility/trace_scope.hpp>
#endif

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <string>
#include <iostream>
#include <vector>
//{test.hpp
//OriginalSource:
//  copy&pasted selected parts of test/x3/test.hpp on 2022-01-08 
#include <boost/spirit/home/x3/core/parse.hpp>
#pragma push_macro("FILE_SHORT")
#define FILE_SHORT "issue707-test.cpp"

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
    { //regression test for issue 707:
      //  https://github.com/boostorg/spirit/issues/707
        #if 0
        {   //trace on this shows is_substitute not used.
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":issue707-std::variant"));
            using namespace x3;
        
            char const* inp = "";
            
            struct A{}; A a;
            struct B{}; B b;
            auto parser=
                attr(a) 
              | attr(b)
              ;
            using uda_t=
              std::variant
              < A
              , B
              >
              ; 
            uda_t uda;  
            bool parsed = test_attr( inp, parser, uda);
            std::cout<<":LINE="<<__LINE__<<":parsed="<<parsed<<";\n";
        }
        #endif
        #if 0
        {
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":issue707-std::vector<std::variant>"));
            //using namespace x3;
        
            char const* inp = "";
            
            struct A{}; A a;
            struct B{}; B b;
            auto parser_var=
                x3::attr(a) 
              | x3::attr(b)
              ;
            auto parser_vec=
              x3::repeat(2)[parser_var]
              ;
            using uda_var=
              x3::variant
              < A
              , B
              >
              ; 
            using uda_vec=
              std::vector
              < uda_var
              >
              ; 
            uda_vec uda;
            bool parsed = test_attr( inp, parser_vec, uda);
            std::cout<<":LINE="<<__LINE__<<":parsed="<<parsed<<";\n";
        }
        #endif
        #if 1
        {
          #ifdef USE_TRACE_SCOPE
            boost::trace_scope ts(stringify(FILE_SHORT,':',__LINE__,":issue707-vector<seq<vars>>"));
          #endif
            using namespace issue707;
        
            char const* inp = "";
            
            auto parser_s1=
                x3::attr(c)
              >>x3::attr(a) 
              >>x3::attr(b)
              ;
            auto parser_s2=
                x3::attr(a) 
              >>x3::attr(b)
              ;
            auto parser_var=
                parser_s1
              | parser_s2
              ;
            auto parser_data=
                x3::attr(d)
              >>parser_var
              ;
            auto parser_vec=
              x3::repeat(2)[parser_data]
              ;
            using uda_vec=
              std::vector
              < uda_data
              >
              ;
            uda_vec uda_vec_v;
            bool parsed = test_attr( inp, parser_vec, uda_vec_v);
            std::cout<<":LINE="<<__LINE__<<":parsed="<<parsed<<";\n";
        }
        #endif
    }
    return 0;
}        
#pragma pop_macro("FILE_SHORT")
