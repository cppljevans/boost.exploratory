#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/utility/trace_scope.hpp>
#endif//USE_TRACE_SCOPE
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#define BOOST_SPIRIT_X3_DEBUG // Provide some meaningful reassuring output.
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>

namespace x3 = boost::spirit::x3;
using x3::unused_type;
using x3::unused;
using x3::space;
using x3::attr;
using x3::char_;
using x3::string;
using x3::omit;
using x3::lit;
using x3::int_;
using x3::float_;
using x3::no_case;
using x3::rule;
using x3::alnum;
using x3::traits::attribute_of;

template<unsigned I>
struct type_i
  { int val1;
    int val2;
  };
  template<unsigned I>
  std::ostream&
operator<<
  ( std::ostream& sout
  , type_i<I>const& x
  )
  {  return std::cout<<"type_i<"<<I<<">{ "<<x.val1<<", "<<x.val2<<"}";
  }
BOOST_FUSION_ADAPT_STRUCT(type_i<1>, val1, val2)	  

namespace parser{
  static auto r1_def=attr(unused)>>int_>>attr(unused)>>int_;
  using rhs_gram_t=decltype(parser::r1_def);
  using rhs_attr_t=attribute_of<rhs_gram_t,unused_type>::type;
  rule<struct r1,rhs_attr_t> r1="r1";
  BOOST_SPIRIT_DEFINE(r1);
}

int main()
{
    std::cout<<std::boolalpha;
    std::cout<<"USE_UNFOLD_LEFT:";
  #ifdef USE_UNFOLD_LEFT
    std::cout<<"defined";
  #else
    std::cout<<"undefined";
  #endif
    std::cout<<";\n";
  #if 0
    {
        std::cout<<"operator>>:\n";
        using parser_type = 
          decltype
          (  attr(unused) 
          >> attr(type_i<1>()) 
          >> attr(unused)
          >> attr(type_i<2>()) 
          );
        using attrib_aof = typename
          attribute_of
          < parser_type
          , unused_type//context type
          >::type
          ;
        using attrib_expected=
             boost::fusion::deque//as in sequence.hpp:attribute_of
         #ifdef USE_UNFOLD_LEFT
             < x3::unused_type
             , type_i<1>
             , x3::unused_type
             , type_i<2>
             >
         #else
             < type_i<1>
             , type_i<2>
             >
         #endif//USE_UNFOLD_LEFT
             ;
        bool constexpr is_expected = 
           boost::is_same
           < attrib_aof
           , attrib_expected
           >::value
           ;
        std::cout<<__LINE__
           <<":is_expected="<<is_expected
           <<":sizeof_aof="<<sizeof(attrib_aof)
           <<":sizeof_expected="<<sizeof(attrib_expected)
           <<";\n";
        std::cout<<":demangle_type<attrib_aof>()=\n"<<demangle_type<attrib_aof>()<<";\n";
        std::cout<<":demangle_type<attrib_expected>()=\n"<<demangle_type<attrib_expected>()<<";\n";
    }
  #endif
  #if 0
    {
        std::cout<<"operator|:\n";
        using parser_type = 
          decltype
          (  attr(unused) 
          |  attr(type_i<1>()) 
          |  attr(unused)
          |  attr(type_i<2>()) 
          );
        using attrib_aof = typename
          attribute_of
          < parser_type
          , unused_type//context type
          >::type
          ;
        using attrib_expected=
             boost::variant//as in alternative.hpp:attribute_of
         #ifdef USE_UNFOLD_LEFT
             < x3::unused_type
             , type_i<1>
             , x3::unused_type
             , type_i<2>
             >
         #else
             < type_i<1>
             , type_i<2>
             >
         #endif//USE_UNFOLD_LEFT
             ;
        bool constexpr is_expected = 
           boost::is_same
           < attrib_aof
           , attrib_expected
           >::value
           ;
        std::cout<<__LINE__
           <<":is_expected="<<is_expected
           <<":sizeof_aof="<<sizeof(attrib_aof)
           <<":sizeof_expected="<<sizeof(attrib_expected)
           <<";\n";
        std::cout<<":demangle_type<attrib_aof>()=\n"<<demangle_type<attrib_aof>()<<";\n";
        std::cout<<":demangle_type<attrib_expected>()=\n"<<demangle_type<attrib_expected>()<<";\n";
    }
  #endif
  #if 1
    {
        char const * iter= "123 456", * const end= iter + std::strlen(iter);
        parser::rhs_attr_t rhs_attr_v;
        bool result=x3::phrase_parse(iter, end, parser::r1, space, rhs_attr_v);
        std::cout<<__LINE__
          <<"\nresult="<<result
          <<"\nat_end="<<(iter==end)
          <<"\nrhs_attr_v=";
        x3::traits::print_attribute(std::cout, rhs_attr_v);
        std::cout
          <<"\ndemangle_type(rhs_attr_v)="<<demangle_type(rhs_attr_v)
          <<"\ndemangle_type<rhs_attr_t>()="<<demangle_type<parser::rhs_attr_t>()
          <<";\n";
    }
  #endif
    return 0;
}
