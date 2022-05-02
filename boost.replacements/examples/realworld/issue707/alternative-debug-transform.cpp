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
#include <boost/spirit/home/x3/support/templ_expr/demangle_fmt_type.hpp>
#include <boost/utility/trace_scope.hpp>
#endif

#include <boost/fusion/include/at.hpp>
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
          template<unsigned I>
        struct Udt
          {
                friend
              std::ostream& 
            operator<<
              ( std::ostream&sout
              , Udt
              )
              { return sout<<"Udt<"<<I<<"> value";
              }           
          };
     //}"conversion between alternatives");
        
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
namespace ns_679
{
        using type0 = std::vector<boost::variant<int>>;
        using type1 = std::vector<boost::variant<type0, int>>;
        using type2 = std::vector<boost::variant<type1, int>>;
        auto p=+('a' >> x3::attr(type1{}) | 'b' >> x3::attr(int{99}));
        using p_type=decltype(p);
        using p_aof=attribute_of<p_type>;
}//ns_679 namespace

#define USE_679_TRANSFORM_PARSER
#ifdef USE_679_TRANSFORM_PARSER
#include <boost/spirit/home/x3/core/make_transform_parser.hpp>
namespace boost{namespace spirit{namespace x3{namespace traits{

  template
  <
  >
  struct
transform_attribute
  < ns_679::type2
  , ns_679::p_aof
  , transform_parser_id
  >
  {
      using to_attribute_type=ns_679::type2;
      using from_attribute_type=ns_679::p_aof;
      static auto pre(to_attribute_type& to_attr) 
      { return from_attribute_type(); 
      }
  
      static void post(to_attribute_type& to_attr, from_attribute_type& from_attr)
      {
      #define TRACE_679_POST
      #ifdef TRACE_679_POST
        std::cout<<":LINE="<<__LINE__<<":type(to_attr)=\n"<<demangle_fmt_type(to_attr)<<":type(from_attr)=\n"<<demangle_fmt_type(from_attr)<<";\n";
      #endif
        for(unsigned i=0; i<from_attr.size(); ++i)
        { auto from_elem=from_attr[i]
        ; std::cout<<":type(from_attr["<<i<<"])="<<demangle_fmt_type(from_elem)<<";\n"
        ; std::cout<<":which="<<from_elem.which()<<";\n";
        ; boost::apply_visitor
          ( [](auto val)
            { std::cout<<":type(val)="<<demangle_fmt_type(val)<<";\n"
            ;}
          , from_elem
          )
        ; break
        ;} 
        //traits::move_to(from_attr, to_attr);
        //to_attr=at_c<1>(from_attr);
      }
  }; 
}}}}
namespace ns_679
{

        auto p_xform
            = x3::make_transform_parser<ns_679::type2>(p)
            ;
}//ns_679 namespace
#endif//USE_679_TRANSFORM_PARSER

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
        boost::variant<Udt<1>, Udt<0>> x{Udt<0>{}};
        Udt<2> z;
        auto p = 'x' >> attr(x) | 'z' >> attr(z);
      //#define USE_CONVERT_TWEEN_ALTS_AOF
      #ifdef USE_CONVERT_TWEEN_ALTS_AOF
        using p_t=decltype(p);
        using attr=attribute_of<p_t>;
        attr v;
      #else
        boost::variant<Udt<0>, Udt<1>, Udt<2> > v;
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
        using namespace ns_679;
        boost::trace_scope ts("regression test for #679");
      //#define USE_679_AOF
        std::cout<<"defined(USE_679_AOF)=";
      #ifdef USE_679_AOF
        std::cout<<"yes;\n";
        p_aof x;
      #else
        std::cout<<"no;\n";
        type2 x;
      #endif
        std::cout<<":LINE="<<__LINE__<<":type2=\n"<<demangle_fmt_type<type2>()<<";\n";
        std::cout<<":LINE="<<__LINE__<<":p_aof=\n"<<demangle_fmt_type<p_aof>()<<";\n";
      #define DO_679_RUN
      #ifdef DO_679_RUN
        result=test_attr
          ( "abaabb"
        #ifdef USE_679_TRANSFORM_PARSER
          , p_xform
        #else
          , p
        #endif//USE_679_TRANSFORM_PARSER
          , x
          );
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
    #if 1
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
