#define TRANSFORM_ATTRIBUTE_RULE2RHS
#include <iostream>
#define USE_INDENT_OSTREAM
#ifdef USE_INDENT_OSTREAM
  #include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
  boost::iostreams::indent_scoped_ostreambuf ind_ostream(std::cout);
#endif//USE_INDENT_OSTREAM
#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/utility/trace_scope.hpp>
#include <boost/utility/stringify.hpp>
#endif
#include <boost/utility/demangle_type.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>

namespace x3 = boost::spirit::x3;
using x3::space;
using x3::unused_type;
using x3::unused;
using x3::rule;
using x3::traits::attribute_of;
using x3::traits::print_attribute;

auto rule_val_gram=
  (+x3::digit)
  ;
using rule_val_attr=
  attribute_of<decltype(rule_val_gram),x3::unused_type>::type
  ;
  template
  < typename Exposed
  >
  void
transform_attribute_post
  ( Exposed&val
  , rule_val_attr const&attr
  )
  {
    std::cout<<__FILE__<<":LINE="<<__LINE__<<':'<<__func__<<":no_op;\n";
    std::cout<<"demangle_type<Exposed>="<<demangle_type<Exposed>()<<";\n";
    std::cout<<"attr="<<attr<<";\n";
  }
using rule_attr_default=int;
  void
transform_attribute_post
  ( rule_attr_default&val
  , rule_val_attr const&attr
  )
  {
    std::istringstream istrm(attr);
    istrm>>val;
    val=-val;
    std::cout<<__FILE__<<":LINE="<<__LINE__<<':'<<__func__<<":rule_val_attr->"<<demangle_type(val)<<":val(result)="<<val<<";\n";
  }
  void
transform_attribute_post
  ( rule_val_attr&val
  , rule_val_attr const&attr
  )
  {
    val=attr+":rule_val_attr";
    std::cout<<__FILE__<<":LINE="<<__LINE__<<':'<<__func__<<":rule_val_attr->"<<demangle_type(val)<<":val(result)="<<val<<";\n";
  }
//#define USE_ACTION
#ifdef USE_ACTION
struct  action_attr_to_val
{
    template <typename Context>
    void operator()(Context const& ctx) const
    {
        boost::trace_scope ts("action_attr_to_val");
        auto& val=x3::_val(ctx);
        auto attr=x3::_attr(ctx);
        std::cout<<":demangle_type(val)="<<demangle_type(val)<<";\n";
        std::cout<<":demangle_type(attr)="<<demangle_type(attr)<<";\n";
        transform_attribute_post(val,attr);
        std::cout<<":val(after)="<<val<<";\n";
    }
};
#endif
auto rule_val_def=
  rule_val_gram
#ifdef USE_ACTION
  [ action_attr_to_val()]
#endif
  ;
using rule_attr_t=
//#define RULE_ATTR_IS_RHS_ATTR
#ifdef RULE_ATTR_IS_RHS_ATTR
  rule_val_attr
#else
  rule_attr_default
#endif//RULE_ATTR_IS_RHS_ATTR
  ;
rule<struct rule_val,rule_attr_t,true> rule_val="rule_val";
#include <sstream>
using rhs_gram_t=decltype(rule_val_gram);
using rhs_attr_t=attribute_of<rhs_gram_t,unused_type>::type;
BOOST_SPIRIT_DEFINE(rule_val);
#define USE_TRANSFORM_ATTRIBUTE_RULE2RHS_SPECIALIZATION
#ifdef USE_TRANSFORM_ATTRIBUTE_RULE2RHS_SPECIALIZATION
namespace boost { namespace spirit { namespace x3 { namespace traits
{
  #if RULE_ATTR_IS_RHS_ATTR
    template <>
    struct transform_attribute
      < rule_val_attr_t //Exposed
      , rule_val_attr_t //Transformed
      , x3::parser_id
      >
    {
        using Exposed=rule_val_attr_t;
        using Transformed=rule_val_attr_t&;
        typedef Transformed type;

        static Transformed pre(Exposed& val) 
        { 
            return val;
        }

        static void post(Exposed& val, Transformed&& attribute)
        {
            std::cout<<__FILE__<<':'<<__LINE__<<':'<<__func__<<"defined(RULE_ATTR_IS_RHS_ATTR);\n";
            transform_attribute_post(val,std::forward<Transformed>(attribute));
        }
    };
  #else  
    template <>
    struct transform_attribute
      < rule_attr_t //Exposed
      , rhs_attr_t //Transformed
      , x3::parser_id
      >
    {
        using Exposed=rule_attr_t;
        using Transformed=rhs_attr_t;
        typedef Transformed type;

        static Transformed pre(Exposed& val) 
        { 
            return Transformed(); 
        }

        static void post(Exposed& val, Transformed&& attribute)
        {
            std::cout<<__FILE__<<':'<<__LINE__<<':'<<__func__<<"!defined(RULE_ATTR_IS_RHS_ATTR);\n";
            transform_attribute_post(val,std::forward<Transformed>(attribute));
        }
    };
  #endif//RULE_ATTR_IS_RHS_ATTR
}}}}
#endif//USE_TRANSFORM_ATTRIBUTE_RULE2RHS_SPECIALIZATION

int main()
{
    std::cout<<"USE_ATTRIBUTE_OF_NO_COLLAPSE:";
  #ifdef USE_ATTRIBUTE_OF_NO_COLLAPSE
    std::cout<<"defined";
  #else
    std::cout<<"undefined";
  #endif
    std::cout<<";\n";
    char const * input="123";
    char const * end= input + std::strlen(input);
  #if 0
    {
      std::cout<<"***val_def***;\n";
      char const * iter=input;
      rhs_attr_t ast;
      std::cout<<":demangle_type(ast)="<<demangle_type(ast)<<";\n";
      bool result=x3::phrase_parse(iter, end, rule_val_def, space, ast);
      bool at_end=iter==end;
      std::cout<<":result="<<result<<":at_end="<<at_end<<";\n";
      std::cout<<":ast=";print_attribute(std::cout,ast);std::cout<<";\n";
    }
  #endif
  #if 1
    {
      std::cout<<"***rule_val***;\n";
      char const * iter=input;
      rule_attr_t ast;
      std::cout<<":demangle_type(ast)="<<demangle_type(ast)<<";\n";
      bool result=x3::phrase_parse(iter, end, rule_val, space, ast);
      bool at_end=iter==end;
      std::cout<<":result="<<result<<":at_end="<<at_end<<";\n";
      std::cout<<":ast=";print_attribute(std::cout,ast);std::cout<<";\n";
    }
  #endif
    return 0;
}
