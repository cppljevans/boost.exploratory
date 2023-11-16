#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#include <boost/utility/trace_scope.hpp>
#endif
#include "is_same_aof_uda.hpp"

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>
namespace x3 =  boost::spirit::x3;
  template<typename Parser>
using attribute_of_default=
  typename x3::traits::attribute_of<Parser,x3::unused_type>::type
  ;
struct S1 {
    double   f0;
    uint32_t f1;
    int64_t  f2;
};
BOOST_FUSION_ADAPT_STRUCT(S1, f0, f1, f2);

auto S1_parser_expr
  =  x3::double_ //S1.f0
  >> x3::uint32  //S1.f1
  >> x3::int64   //S1.f2
  ;
using S1_parser_type=decltype(S1_parser_expr);
using S1_aof=attribute_of_default<S1_parser_type>;

struct S2 {
    uint32_t f0;
    int64_t  f1;
};
BOOST_FUSION_ADAPT_STRUCT(S2, f0, f1);

auto S2_parser_expr
  =  x3::uint32  //S2.f0
  >> x3::int64   //S2.f1
  ;
  
using var_S1_S2=boost::variant<S1, S2>;

auto var_S1_S2_parser_expr=
     S1_parser_expr
   | S2_parser_expr
   ;
using var_S1_S2_parser_type=decltype(var_S1_S2_parser_expr);
using var_S1_S2_aof=attribute_of_default<var_S1_S2_parser_type>;

struct LogData {
    int16_t   f0;
    var_S1_S2 f1;
};
BOOST_FUSION_ADAPT_STRUCT(LogData, f0, f1);

auto LogData_parser_expr=
   x3::int16  //LogData.f0
>> var_S1_S2_parser_expr //LogData.f1
   ;
using LogData_parser_type=decltype(LogData_parser_expr);
using LogData_aof=attribute_of_default<LogData_parser_type>;
   
  template
  < unsigned I
  >
struct typeI
  { unsigned v;
    typeI(unsigned a=I):v(a){}
    friend std::ostream&
      operator<<
      ( std::ostream&sout
      , typeI const& x
      )
      { return sout<<"typeI"<<I<<">:"<<x.v
      ;}
  };
  template
  < typename Parser
  , typename Attribute
  >
  void
test_parse
  ( std::string const&inp
  , Parser&parser
  , Attribute&attr
  )
  { std::cout<<":inp="<<inp<<";\n"
  ; auto first=inp.begin()
  ; auto last=inp.end()
  ; bool result=x3::phrase_parse(first,last,parser,x3::blank,attr)
  ; std::cout<<":result="<<result<<";\n"
  ; std::string remains(first,last)
  ; std::cout<<":remains="<<remains<<";\n"
  ; std::cout<<":attr="
  ; x3::traits::print_attribute(std::cout,attr)
  ; std::cout<<";\n"
  ;}  
  int
main()
  {
  #ifdef USE_TRACE_SCOPE
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif
#if 0  
  ; {
    ; boost::trace_scope ts(stringify(__LINE__))
    ; auto parser=x3::attr(x3::unused)
    ; std::cout<<"parser=\n"<<demangle_fmt_type(parser)<<";\n"
    ; using aof=attribute_of_default<decltype(parser)>
    ; std::cout<<"aof\n="<<demangle_fmt_type<aof>()<<";\n"
    ;}
#endif
#if 0 
  ; {
    ; boost::trace_scope ts(stringify(__LINE__))
    ; auto parser=x3::attr(x3::unused) >> x3::attr(x3::unused)
    ; std::cout<<"parser=\n"<<demangle_fmt_type(parser)<<";\n"
    ; using aof=attribute_of_default<decltype(parser)>
    ; std::cout<<"aof="<<demangle_fmt_type<aof>()<<";\n"
    ;}
#endif
#if 0  
  ; {
    ; boost::trace_scope ts(stringify(__LINE__))
    ; auto parser='a' >> x3::int_ >> 'b'
    ; std::cout<<"parser=\n"<<demangle_fmt_type(parser)<<";\n"
    ; using aof=attribute_of_default<decltype(parser)>
    ; std::cout<<"aof="<<demangle_fmt_type<aof>()<<";\n"
    ; std::string inp="a 123 b"
    ; aof attr;
    ; test_parse(inp,parser,attr)
    ;}
#endif
#if 0
  ; {
    ; boost::trace_scope ts(stringify(__LINE__))
    ; auto parser= 
        x3::int16
          >> ( (  x3::double_
               >> x3::uint32
               >> x3::int64
               )
             | (  x3::uint32
               >> x3::int64
               )
             )
    ; std::cout<<"parser=\n"<<demangle_fmt_type(parser)<<";\n"
    ; using aof=attribute_of_default<decltype(parser)>
    ; std::cout<<"aof="<<demangle_fmt_type<aof>()<<";\n"
    ; std::string inp="1 1.1 11 111"
    ; aof attr;
    ; test_parse(inp,parser,attr)
    ; inp="2 22 222"
    ; test_parse(inp,parser,attr)
    ;}
#endif
#if 0
  ; {
    ; boost::trace_scope ts(stringify(__LINE__))
  #if 0
    ; var_S1_S2 var
    ; S1_aof s1{1.1, 10, 1000}
    ; var=s1
    ; std::cout<<":var="
    ; x3::traits::print_attribute(std::cout,var)
    ; std::cout<<";\n"
  #endif
  #if 1
    ; using var_find_sub_S1=x3::traits::variant_find_substitute<var_S1_S2,S1_aof>
    ; std::cout<<":var_sub_S1="<<demangle_fmt_type<typename var_find_sub_S1::type>()<<";\n"
    #ifdef USE_DEBUG
    ; var_find_sub_S1::trace_tmpl()
    #endif
  #endif
    ;}
#endif
#if 0
  ; {
    ; boost::trace_scope ts(stringify(__LINE__))
    ; using aof=LogData_aof
    ; using uda=LogData
    ; std::cout<<"uda="<<demangle_fmt_type<uda>()<<";\n"
    ; using is_sub_aof_uda=x3::traits::is_substitute<aof,uda>
    ; is_sub_aof_uda::trace_tmpl()
    ; std::cout<<"is_sub_aof_uda="<<is_sub_aof_uda::value<<";\n"
  #if 0  
    ; std::string inp="1 1.1 11 111"
    ; uda attr;
    ; test_parse(inp,parser,attr)
    ; inp="2 22 222"
    ; test_parse(inp,parser,attr)
  #endif
    ;}
#endif
#if 1
  ; {
    ; boost::trace_scope ts(stringify(__LINE__))
    ; using aof=var_S1_S2_aof
    ; using uda=var_S1_S2
    ; std::cout<<"aof="<<demangle_fmt_type<aof>()<<";\n"
    ; std::cout<<"uda="<<demangle_fmt_type<uda>()<<";\n"
    ; using is_sub_aof_uda=x3::traits::is_substitute<aof,uda>
    ; is_sub_aof_uda::trace_tmpl()
    ; std::cout<<"is_sub_aof_uda="<<is_sub_aof_uda::value<<";\n"
    ;}
#endif
#if 0    
  ; {
    ; boost::trace_scope ts(stringify(__LINE__))
    ; auto parser=x3::attr(typeI<0>{})
    ; std::cout<<"parser=\n"<<demangle_fmt_type(parser)<<";\n"
    ; using aof=attribute_of_default<decltype(parser)>
    ; std::cout<<"aof\n="<<demangle_fmt_type<aof>()<<";\n"
    ;}
  ; {
    ; boost::trace_scope ts(stringify(__LINE__))
    ; auto parser=x3::attr(typeI<0>{}) >> x3::attr(typeI<1>{})
    ; std::cout<<"parser=\n"<<demangle_fmt_type(parser)<<";\n"
    ; using aof=attribute_of_default<decltype(parser)>
    ; std::cout<<"aof="<<demangle_fmt_type<aof>()<<";\n"
    ;}
#endif    
  ; return 0
  ;}  
