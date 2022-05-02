//Purpose:
//  Figure out why 1.78breakage-simplified2.cpp shows is_substitute on boost::variant<S1,S2> and
//  correponding attribute_of doesn't pass the is_substitute test.
//===============================
#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/spirit/home/x3/support/templ_expr/demangle_fmt_type.hpp>
#include <boost/utility/trace_scope.hpp>
#endif

#include <boost/variant.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>

namespace x3 = boost::spirit::x3;
namespace fusion = boost::fusion;

struct S1 {
    double   f0;
    uint32_t f1;
    int64_t  f2;
};
BOOST_FUSION_ADAPT_STRUCT(S1, f0, f1, f2);

struct S2 {
    uint32_t f0;
    int64_t  f1;
};
BOOST_FUSION_ADAPT_STRUCT(S2, f0, f1);

using attribute=
            boost::variant
            < S1
            , S2
            >;
using attr_of=
            boost::variant
            < fusion::deque
              < double
              , uint32_t
              , int64_t
              >//S1
            , fusion::deque
              < uint32_t
              , int64_t
              >//S2
            >;
template<typename T0, typename T1>
void test_is_substitute(std::string const&label)
{
      boost::trace_scope ts(__func__+label);
      using is_sub=x3::traits::is_substitute< T0, T1>;
      std::cout<<":is_sub::value="<<is_sub::value<<";\n";
    #ifdef USE_DEBUG
      std::cout<<"trace_tmpl=\n";
      is_sub::trace_tmpl();
    #endif//USE_DEBUG
}
int main()
{
  std::cout<<std::boolalpha;
  #ifdef USE_TRACE_SCOPE
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif
    test_is_substitute
    < attr_of
    , attribute
    >("< attr_of, attribute>");
    test_is_substitute
    < attribute
    , attr_of
    >("< attribute, attr_of>");
    return 0;
}                    
