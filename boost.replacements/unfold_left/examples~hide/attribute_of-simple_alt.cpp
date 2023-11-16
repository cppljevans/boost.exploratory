#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/utility/trace_scope.hpp>
#endif
#include <functional>
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#include <boost/spirit/home/x3/support/ast/get_put_index_variant.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>
namespace x3 = boost::spirit::x3;
using x3::unused_type;
using x3::unused;
using x3::lit;
using x3::space;
using x3::rule;
using x3::traits::attribute_of;
using x3::traits::print_attribute;
auto alt_rule_def
  = lit('a')  //unused attribute
  >> x3::int32
  | lit('b')  //unused attribute
  >> x3::int32
  | lit('c')  //unused attribute
  >> x3::int32
  ;
using rhs_gram_t=decltype(alt_rule_def);
using rhs_attr_t=attribute_of<rhs_gram_t,unused_type>::type;
using alt_attr_t=
//#define RULE_ATTR_IS_RHS_ATTR
#ifndef RULE_ATTR_IS_RHS_ATTR
  #ifdef USE_UNFOLD_LEFT
    //#define UNFOLD_ATTR_IS_RHS_ATTR
  #endif
#endif//RULE_ATTR_IS_RHS_ATTR
#ifdef UNFOLD_ATTR_IS_RHS_ATTR
  rhs_attr_t
#else
  x3::std_variant
  < int32_t
  , int32_t
  , int32_t
  >
#endif
  ;
rule<struct alt_rule,alt_attr_t> alt_rule="alt_rule";
BOOST_SPIRIT_DEFINE(alt_rule);
auto alt_assign=alt_rule=alt_rule_def;
namespace boost { namespace spirit { namespace x3 { namespace traits
{
  #ifndef UNFOLD_ATTR_IS_RHS_ATTR
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
            std::cout<<"Exposed="<<demangle_fmt_type<Exposed>()<<";\n";
            std::cout<<"Transformed="<<demangle_fmt_type<Transformed>()<<";\n";
            std::cout<<"print_attribute(transformed):";print_attribute(std::cout,transformed);std::cout<<";\n";
            auto get_put_l=[&]<std::size_t Ndx>()
              {
                std::cout<<"get_put_l<"<<Ndx<<">()\n";
                boost::put<Ndx>
                  ( val
                  , fusion::at_c<1>//get x3::int32 part
                    ( boost::get<Ndx>(transformed)
                    )
                  );
              };
          //#define get_put_l_SPECIALIZE
          #ifdef get_put_l_SPECIALIZE
            //fails compile with:
            //  error: redefinition of 'get_put_l'
            auto get_put_l=[&]<0>()
              {
                std::cout<<"get_put_l<0specialized>()\n";
              };
          #endif//get_put_l_SPECIALIZE
            using get_put_t=std::function<void(void)>;
            std::size_t constexpr size_v=
              std::variant_size_v<Exposed::variant_type>;
            auto make_array_l=[&]<std::size_t... Ndx>(std::integer_sequence<std::size_t,Ndx...>)
              { 
              ; std::array<get_put_t,size_v> v
                { [get_put_l](){get_put_l.operator()<Ndx>();}...
                };
              ; return v
              ;};
            auto constexpr ndx_seq=std::make_index_sequence<size_v>();
            auto ndx=boost::index(transformed);
            std::cout<<"make_array_l["<<ndx<<"];\n";
            make_array_l(ndx_seq)[ndx]();
        }
    };
  #endif//RULE_ATTR_IS_RHS_ATTR
}}}}
char const * input=
  "b 456"
  ;
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
  std::cout<<":demangle_fmt_type(gram)=\n"<<demangle_fmt_type(gram)<<";\n";
  std::cout<<":demangle_fmt_type(ast)=\n"<<demangle_fmt_type(ast)<<";\n";
  bool result=x3::phrase_parse(iter, end, gram, space, ast);
  std::cout<<":result="<<result<<":at_end="<<(iter==end)<<";\n";
  std::cout<<":index(ast)="<<boost::index(ast)<<":ast=";print_attribute(std::cout,ast);std::cout<<";\n";
}
int main()
{
  #ifdef USE_TRACE_SCOPE
    boost::iostreams::indent_scoped_ostreambuf<char>
  indent_outbuf(std::cout,2);
  #endif
  test<alt_attr_t>("***alt_rule***",alt_rule);
  //test<rhs_attr_t>("***alt_rule_def***",alt_rule_def);
  //test<alt_attr_t>("***alt_assign:alt_attr_t***",alt_assign);
  //test<rhs_attr_t>("***alt_assign:rhs_attr_t***",alt_assign);
  return 0;
}
