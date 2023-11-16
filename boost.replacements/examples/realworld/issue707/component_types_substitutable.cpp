#define USE_COMPONENT_TYPES_SUBSTITUTABLE_TRACE_TMPL
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#include <boost/utility/trace_scope.hpp>
#include <boost/spirit/home/x3/support/traits/is_substitute.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/container/deque.hpp>
#include <iostream>
template<unsigned I>
struct type
{};
struct s0
{ type<0> f0;
  type<1> f1;
};
BOOST_FUSION_ADAPT_STRUCT(s0, f0, f1);
struct s1
{ type<1> f0;
  type<0> f1;
};
BOOST_FUSION_ADAPT_STRUCT(s1, f0, f1);
struct s2
{ type<0> f0;
  type<1> f1;
};
BOOST_FUSION_ADAPT_STRUCT(s2, f0, f1);

namespace x3=boost::spirit::x3;
namespace fus=boost::fusion;


template<typename T0, typename T1>
auto test(auto line)
{
    using t=x3::traits::detail::component_types_substitutable<T0,T1>;
  #ifdef USE_COMPONENT_TYPES_SUBSTITUTABLE_TRACE_TMPL
    t::trace_tmpl();
  #endif
    using r=typename t::type;
    std::cout<<line<<':'<<r::value<<";\n";
}
int main()
{
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
    std::cout<<std::boolalpha;
  #if 1
    {  
        using t0=fus::deque<type<0>,type<1>>;
        using t1=fus::deque<type<1>,type<0>>;
        test<t0,t0>(__LINE__);
        test<t0,t1>(__LINE__);
    }
  #endif
  #if 1
    {  
        using t0=boost::variant<type<0>,type<1>>;
        using t1=boost::variant<type<1>,type<0>>;
        test<t0,t0>(__LINE__);
        test<t0,t1>(__LINE__);
    }
  #endif
  #if 1
    {  
        test<s0,s0>(__LINE__);
        test<s0,s1>(__LINE__);
        test<s0,s2>(__LINE__);
        using t0=fus::deque<type<0>,type<1>>;
        test<s0,t0>(__LINE__);
    }
  #endif
    return 0;
}    
