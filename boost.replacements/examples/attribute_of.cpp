#include <boost/utility/demangle_type.hpp>
#define BOOST_SPIRIT_X3_DEBUG // Provide some meaningful reassuring output.
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace var_ns=
#ifdef USE_ATTRIBUTE_OF_NO_COLLAPSE
  std
#else
  boost
#endif//ATTRIBUTE_OF_NO_COLLAPSE
  ;
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
  {  return std::cout<<"{ "<<x.val1<<", "<<x.val2<<"}";
  }
BOOST_FUSION_ADAPT_STRUCT(type_i<1>, val1, val2)	  

namespace parser{
  rule<struct r1,type_i<1>> r1="r1";
  
  static auto r1_def=attr(unused)>>int_>>attr(unused)>>int_;
  
  BOOST_SPIRIT_DEFINE(r1);
}

int main()
{
    std::cout<<"USE_ATTRIBUTE_OF_NO_COLLAPSE:";
  #ifdef USE_ATTRIBUTE_OF_NO_COLLAPSE
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
        using attrib = typename
          attribute_of
          < parser_type
          , unused_type//context type
          >::type
          ;
        bool constexpr is_expected_attr = 
           boost::is_same
           < attrib
           , boost::fusion::deque//as in sequence.hpp:attribute_of
         #ifdef USE_ATTRIBUTE_OF_NO_COLLAPSE
             < x3::unused_type
             , type_i<1>
             , x3::unused_type
             , type_i<2>
             >
         #else
             < type_i<1>
             , type_i<2>
             >
         #endif//USE_ATTRIBUTE_OF_NO_COLLAPSE
           >::value
           ;
        std::cout<<__LINE__
          <<":is_expected_attr="<<is_expected_attr
          <<":sizeof_attr="<<sizeof(attrib)
          <<":demangle_type(attrib)="<<demangle_type<attrib>()
          <<";\n";
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
        using attrib = typename
          attribute_of
          < parser_type
          , unused_type//context type
          >::type
          ;
        bool constexpr expected_attr = 
           boost::is_same
           < attrib
           , var_ns::variant//as in alternative.hpp:attribute_of
         #ifdef USE_ATTRIBUTE_OF_NO_COLLAPSE
             < x3::unused_type
             , type_i<1>
             , x3::unused_type
             , type_i<2>
             >
         #else
             < type_i<1>
             , type_i<2>
             >
         #endif//USE_ATTRIBUTE_OF_NO_COLLAPSE
           >::value
           ;
        std::cout<<__LINE__<<":expected_attr="<<expected_attr<<":sizeof_attr="<<sizeof(attrib)<<";\n";
    }
  #endif
  #if 1
    {
        char const * iter= "123 456", * const end= iter + std::strlen(iter);
        type_i<1> ast;
        using rhs_gram_t=decltype(parser::r1_def);
        using rhs_attr_t=attribute_of<rhs_gram_t,unused_type>::type;
        bool result=x3::phrase_parse(iter, end, parser::r1, space, ast) && iter!=end;
        std::cout<<__LINE__<<":result="<<result
          <<"\nast="<<ast
          <<"\ndemangle_type(ast)="<<demangle_type(ast)
          <<"\ndemangle_type<rhs_attr_t>()="<<demangle_type<rhs_attr_t>()
          <<";\n";
    }
  #endif
    return 0;
}
