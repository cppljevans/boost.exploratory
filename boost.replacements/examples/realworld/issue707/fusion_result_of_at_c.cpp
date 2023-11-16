#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
namespace x3 = boost::spirit::x3;
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

struct LogData {
    int16_t                f0;
    boost::variant<S1, S2> f1;
};
BOOST_FUSION_ADAPT_STRUCT(LogData, f0, f1);

#include <boost/fusion/include/at.hpp>
namespace fusion=boost::fusion;

  template
  < typename U
  , std::size_t I
  >
struct result_of_at_c
  : std::remove_reference
    < typename fusion::result_of::at_c
      < U
      , I
      >::type
    >
  {};

  int
main()
  { using type=typename result_of_at_c<LogData,1>::type
  ; std::cout<<":type=\n"<<demangle_fmt_type<type>()<<";\n"
  ; return 0
  ;}  
