#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/utility/trace_scope.hpp>
#endif
#include <boost/utility/demangle_type.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>
namespace x3 = boost::spirit::x3;
using x3::unused_type;
using x3::unused;
using x3::lit;
using x3::space;
using x3::int_;
using x3::rule;
using x3::traits::attribute_of;
using x3::traits::print_attribute;

auto alt_rule_def
  = lit('a')//unused attribute
  >>int_    //int attribute
  | lit('b')//unused attribute
  >>int_    //int attribute
  ;
using rhs_gram_t=decltype(alt_rule_def);
using rhs_attr_t=attribute_of<rhs_gram_t,unused_type>::type;
using alt_attr_t=
//#define RULE_ATTR_IS_RHS_ATTR
#ifdef RULE_ATTR_IS_RHS_ATTR
  rhs_attr_t
#else
  x3::std_variant<int,int>
#endif
  ;
rule<struct alt_rule,alt_attr_t> alt_rule="alt_rule";
BOOST_SPIRIT_DEFINE(alt_rule);
auto alt_assign=alt_rule=alt_rule_def;
namespace boost { namespace spirit { namespace x3 { namespace traits
{
  #ifndef RULE_ATTR_IS_RHS_ATTR
    template <>
    struct transform_attribute
      < alt_attr_t //Exposed
      , rhs_attr_t //Transformed
      , x3::parser_id
      >
    {
        using Exposed=alt_attr_t;
        using Transformed=rhs_attr_t;
        typedef Transformed type;

        static Transformed pre(Exposed&exposed) 
        { 
          return Transformed();
        }

        static void post(Exposed& val, Transformed&& attribute)
        {
            trace_scope ts(__func__);
            Transformed transformed=std::forward<Transformed>(attribute);
            std::cout<<"Exposed="<<demangle_type<Exposed>()<<";\n";
            std::cout<<"Transformed="<<demangle_type<Transformed>()<<";\n";
            std::cout<<"print_attribute(transformed):";print_attribute(std::cout,transformed);std::cout<<";\n";
            auto index=transformed.index();
            switch (index)
            {
            case 0:
              val.template emplace<0>(fusion::at_c<1>(std::get<0>(transformed)));
              break;
            case 1:
              val.template emplace<1>(fusion::at_c<1>(std::get<1>(transformed)));
              break;
            default:
              std::cout<<":default;\n";
            }
        }
    };
  #endif//RULE_ATTR_IS_RHS_ATTR
}}}}
char const * input="b 456";
char const * end= input + std::strlen(input);
template<typename Ast, typename Gram>
void test
( std::string label
, Gram gram
)
{
  std::cout<<label<<";\n";
  char const * iter=input;
  Ast ast;
  std::cout<<":demangle_type(ast)="<<demangle_type(ast)<<";\n";
  bool result=x3::phrase_parse(iter, end, gram, space, ast);
  std::cout<<":result="<<result<<":at_end="<<(iter==end)<<";\n";
  std::cout<<":ast=";print_attribute(std::cout,ast);std::cout<<";\n";
}
int main()
{
    test<alt_attr_t>("***alt_rule***",alt_rule);
    //test<rhs_attr_t>("***alt_rule_def***",alt_rule_def);
    //test<alt_attr_t>("***alt_assign:alt_attr_t***",alt_assign);
    //test<rhs_attr_t>("***alt_assign:rhs_attr_t***",alt_assign);
    return 0;
}
