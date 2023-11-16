//Purpose:
//  show simplified example of:
/*
https://github.com/boostorg/spirit/issues/707
 */
//===================
#include <iostream>
#include <string>
#include <vector>
#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#include <boost/utility/trace_scope.hpp>
#endif

#include <boost/variant.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>

namespace x3 = boost::spirit::x3;
struct struct_0_0
{
    double   f0;
    uint32_t f1;
    int64_t  f2;
};
BOOST_FUSION_ADAPT_STRUCT(struct_0_0, f0, f1, f2);
struct struct_0_1
{
    uint32_t f0;
    int64_t  f1;
};    
BOOST_FUSION_ADAPT_STRUCT(struct_0_1, f0, f1);
struct struct_0 
{
    struct_0_0 f0;
    struct_0_1 f1;
};
//#define USE_NESTED_STRUCT
#ifdef USE_NESTED_STRUCT
BOOST_FUSION_ADAPT_STRUCT(struct_0, f0, f1);
#else
BOOST_FUSION_ADAPT_STRUCT(struct_0, f0.f0, f0.f1, f0.f2, f1.f0, f1.f1);
#endif// USE_NESTED_STRUCT

  std::ostream&
operator<<
  ( std::ostream& sout
  , struct_0 const& x
  )
  { sout<<"struct_0=\n"
  ; x3::traits::print_attribute(sout,x);
  ; return sout
  ;}

auto parser_0_0
  =  x3::double_
  >> x3::uint32
  >> x3::int64
  ;
auto parser_0_1
  =  x3::uint32
  >> x3::int64
  ;
auto parser_0
  =  parser_0_0
  >> parser_0_1
  ;
int main() {
  #ifdef USE_TRACE_SCOPE
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif
    std::cout<<std::boolalpha;
    auto s = std::string
    {
R"(  10 11 12 13 14
     20 21 22 23 24)"
    };
    using parser_0_type=decltype(parser_0);
    std::cout<<__func__<<":parser_0_type="<<demangle_fmt_type<parser_0_type>()<<";\n";
  #if 1
    using parser_0_aof=typename x3::traits::attribute_of<parser_0_type,x3::unused_type>::type;
    std::cout<<__func__<<":parser_0_aof="<<demangle_fmt_type<parser_0_aof>()<<";\n";
    using same_aof_struct_0=x3::traits::is_same_enough<parser_0_aof,struct_0>;
    std::cout<<__func__<<":same_aof_struct_0="<<same_aof_struct_0::value<<";\n";
    auto Container = std::vector<struct_0>{};
    if 
    ( x3::phrase_parse
      ( s.begin(), s.end()
      , parser_0 % x3::eol
      , x3::blank
      , Container
      )
    )
    {
        std::cout<<"Parsing success\n";
        for(unsigned i=0; i<Container.size(); ++i)
        {
          std::cout<<"Container["<<i<<"]=\n";
          x3::traits::print_attribute(std::cout,Container[i]);
          std::cout<<";\n";
        }
    }
    else
    {
        std::cout<<"Parsing failed\n";
    }
  #endif
    return 0;
}
