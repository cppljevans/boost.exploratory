/** \file playerk.separate_tu.main.cpp
 */
//=============
#include "playerk.separate_tu.my_rule.hpp"
#include <boost/fusion/include/adapt_struct.hpp>
#include <iostream>
struct Foo
  {
    int a;
    int b;
    constexpr auto operator<=>(Foo const&) const = default; 
  };
BOOST_FUSION_ADAPT_STRUCT(Foo, a, b);
  std::ostream&
operator<<
  ( std::ostream&os
  , Foo const&x
  )
  { os<<"Foo{a="<<x.a<<",b="<<x.b<<"}"
  ; return os
  ;}
struct Bar
  {
    int c;
    int d;
    int e;
    Bar():c(-1),d(-2),e(-3)
      {}
    Bar(int c_,int d_,int e_):c(c_),d(d_),e(e_)
      {}
    constexpr auto operator<=>(Bar const&) const = default; 
  };
BOOST_FUSION_ADAPT_STRUCT(Bar, c, d, e);
  std::ostream&
operator<<
  ( std::ostream&os
  , Bar const&x
  )
  { os<<"Bar{c="<<x.c<<",d="<<x.d<<",e="<<x.e<<"}"
  ; return os
  ;}
struct Baz
  {
    int f;
    Foo foo;
    constexpr auto operator<=>(Baz const&) const = default; 
  };
BOOST_FUSION_ADAPT_STRUCT(Baz, f, foo);
  std::ostream&
operator<<
  ( std::ostream&os
  , Baz const&x
  )
  { os<<"Baz{f="<<x.f<<",foo="<<x.foo<<"}"
  ; return os
  ;}
const auto bar_rule_def =
  x3::byte_ >> my_rule
  ;
class Bar_rule_tag{}
  ;  
#ifdef USE_GEN_TRANSFORM_PARSER
  #include <boost/spirit/home/x3/core/gen_transform_parser.hpp>
namespace boost
{
namespace spirit
{
namespace x3
{
namespace traits
{
    template
    < typename Transformed
    >
struct transform_attribute_tagged
    < Bar
    , Transformed
    , Bar_rule_tag
    >
    {
      using Exposed=Bar
        ;
      using type=Transformed
        ;
          static
        Transformed
      pre(Exposed&e)
        { Transformed t{}
        ; std::cout<<__LINE__<<':'<<__func__
          <<":exposed="<<e
          <<":transformed="<<t
          <<";\n"
        ; return t
        ;}
          static
        auto
      post(Exposed&e, Transformed&t)
        { 
        ; e.c=fusion::at_c<0>(t)
        ; auto const&at_1=fusion::at_c<1>(t)
        ; e.d=fusion::at_c<0>(at_1)
        ; e.e=fusion::at_c<1>(at_1)
        ; std::cout<<__LINE__<<':'<<__func__
          <<":exposed="<<e
          <<":transformed="<<t
          <<";\n"
        ; return true
        ;}
    };
}//traits
}//x3
}//spirit
}//boost
#endif//USE_GEN_TRANSFORM_PARSER
const auto foo_rule = 
  x3::rule<class Foo_rule_tag, Foo>{ "foo-rule" } %=
  my_rule;
const auto bar_rule = 
  x3::rule<Bar_rule_tag, Bar>{ "bar-rule" } %=
#ifdef USE_GEN_TRANSFORM_PARSER
  x3::traits::as_attr<Bar,Bar_rule_tag>[bar_rule_def]
#else
  bar_rule_def
  //This causes "Size of the passed attribute is bigger than expected"
  //static assert failure in detail/sequence.hpp
#endif//USE_GEN_TRANSFORM_PARSER
  ;
const auto baz_rule = 
  x3::rule<class Bar_rule_tag, Baz>{ "baz-rule" } %=
  x3::byte_ >> my_rule
  ;
  template <typename T>
  bool 
parse(auto const& frame, auto rule, T expected) 
  {
      std::cout<<":expected="<<expected<<";\n";
      T actual;
      bool parse_ok=parse(begin(frame), end(frame), rule, actual);
      std::cout<<":parse_ok="<<parse_ok<<";\n";
      std::cout<<":actual="<<actual<<";\n";
      if(parse_ok)
      {
        bool attr_ok=expected==actual;
        std::cout<<":attr_ok="<<attr_ok<<";\n";
        return attr_ok;
      }
      else
      {
        return parse_ok;
      }
  }
  int
main()
  {
    std::cout<<std::boolalpha;
    stream frame{0x03, 0x06, 0x09};
    bool all_ok=true;
    std::cout<<__LINE__<<":all_ok="<<all_ok<<";\n";
  #if 1
    all_ok=all_ok && parse<Foo>(frame, foo_rule, Foo{ 3, 6 });
    std::cout<<__LINE__<<":all_ok="<<all_ok<<";\n";
  #endif
  #if 1 && defined(USE_GEN_TRANSFORM_PARSER)
    all_ok=all_ok && parse<Bar>(frame, bar_rule, Bar{ 3, 6, 9 });
    std::cout<<__LINE__<<":all_ok="<<all_ok<<";\n";
  #endif
  #if 1
    all_ok=all_ok && parse<Baz>(frame, baz_rule, Baz{ 3, {6, 9} });
    std::cout<<__LINE__<<":all_ok="<<all_ok<<";\n";
  #endif
    return 0;
  }
