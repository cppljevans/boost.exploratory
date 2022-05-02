//OriginalSource:
//  copy&pasted selected parts of test/x3/alternative.cpp on 2022-01-08
/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <string>
#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/utility/trace_scope.hpp>
#endif

#include <boost/fusion/include/at.hpp>
#include <boost/utility/demangle_fmt_type.hpp>
  template
  < typename T
  >
  std::ostream&
operator<<
  ( std::ostream&sout
  , std::vector<T>const&x
  )
  {
    for(unsigned i=0; i<x.size(); ++i)
    { if(i==0) sout<<"[ "; else sout<<", "; 
      sout<<i<<"="<<x[i];
    }
    return sout<<"]";
  }
#include <boost/variant.hpp>
     //{"conversion between alternatives");
        struct X 
        {
        };
          std::ostream& operator<<
            ( std::ostream&sout
            , X
            )
            { return sout<<"X value";
            } 
        struct Y 
        {
        };
          std::ostream& operator<<
            ( std::ostream&sout
            , Y
            )
            { return sout<<"Y value";
            } 
        struct Z 
        {
        };
          std::ostream& operator<<
            ( std::ostream&sout
            , Z
            )
            { return sout<<"Z value";
            } 
     //}"conversion between alternatives");
     //{"regression test for #679"
        using Qaz = std::vector<boost::variant<int>>;
        using Foo = std::vector<boost::variant<Qaz, int>>;
        using Bar = std::vector<boost::variant<Foo, int>>;
     //}"regression test for #679"
        
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>

namespace x3        = boost::spirit::x3;

template<typename Parser, typename Context=x3::unused_type>
using attribute_of=typename x3::traits::attribute_of<Parser,Context>::type;

//{test.hpp
//OriginalSource:
//  copy&pasted selected parts of test/x3/test.hpp on 2022-01-08 

namespace spirit_test
{
    template <typename Char, typename Parser, typename Attr>
    bool test_attr(Char const* in, Parser const& p
        , Attr& attr, bool full_match = true)
    {
        Char const* last = in;
        while (*last)
            last++;
        return boost::spirit::x3::parse(in, last, p, attr)
            && (!full_match || (in == last));
    }
}
//}test.hpp

int
main()
{
    std::cout<<std::boolalpha;
  #ifdef USE_TRACE_SCOPE
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif
    using spirit_test::test_attr;
    using boost::spirit::x3::attr;
    bool result;
    { // conversion between alternatives
    #if 0
        boost::trace_scope ts("conversion between alternatives");
        boost::variant<Y, X> x{X{}};
        auto p = 'x' >> attr(x) | 'z' >> attr(Z{});
      //#define USE_CONVERT_TWEEN_ALTS_AOF
      #ifdef USE_CONVERT_TWEEN_ALTS_AOF
        using p_t=decltype(p);
        using attr=attribute_of<p_t>;
        attr v;
      #else
        boost::variant<X, Y, Z> v;
        v = x; // boost::variant supports that convertion
      #endif

        result=test_attr("z", p, v);
        std::cout<<__LINE__<<":result="<<result<<";\n";
        if(result)
        {   std::cout<<__LINE__<<":v=\n";
            boost::spirit::x3::traits::print_attribute(std::cout,v);
            std::cout<<";\n";
        }
        result=test_attr("x", p, v);
        std::cout<<__LINE__<<":result="<<result<<";\n";
        if(result)
        {   std::cout<<__LINE__<<":v=\n";
            boost::spirit::x3::traits::print_attribute(std::cout,v);
            std::cout<<";\n";
        }
    #endif
    }

    { // regression test for #679
    #if 1
        boost::trace_scope ts("regression test for #679");
        auto p=+('a' >> attr(Foo{}) | 'b' >> attr(int{99}));
        using p_type=decltype(p);
        using p_attr=attribute_of<p_type>;
      //#define USE_679_AOF
        std::cout<<"defined(USE_679_AOF)=";
      #ifdef USE_679_AOF
        std::cout<<"yes;\n";
        p_attr x;
      #else
        std::cout<<"no;\n";
        Bar x;
      #endif
        std::cout<<":LINE="<<__LINE__<<":Bar=\n"<<demangle_fmt_type<Bar>()<<";\n";
        std::cout<<":LINE="<<__LINE__<<":p_attr=\n"<<demangle_fmt_type<p_attr>()<<";\n";
      #define DO_679_RUN
      #ifdef DO_679_RUN
        result=test_attr( "abaabb", p, x);
        std::cout<<":LINE="<<__LINE__<<":result="<<result<<";\n";
        if(result) 
        { std::cout<<":x=\n";
          boost::spirit::x3::traits::print_attribute(std::cout,x);
          std::cout<<";\n";
        }
      #endif//DO_679_RUN
    #endif
    }
    { // regression test for #679-simplified
    #if 0
        boost::trace_scope ts("regression test for #679-simplified");
        auto const gram='a' >> attr(char{'a'}) | 'b' >> attr(long{999});
        boost::variant<char,long> x;
        result=test_attr("a", gram, x);
        std::cout<<__LINE__<<":result="<<result<<";\n";
        if(result)
        { std::cout<<":x.which()="<<x.which()<<":x=\n";
          boost::spirit::x3::traits::print_attribute(std::cout,x);
          std::cout<<";\n";
        }
        result=test_attr("b", gram, x);
        std::cout<<__LINE__<<":result="<<result<<";\n";
        if(result) 
        { std::cout<<":x=\n";
          boost::spirit::x3::traits::print_attribute(std::cout,x);
          std::cout<<";\n";
        }
    #endif
    }
    return 0;
}
