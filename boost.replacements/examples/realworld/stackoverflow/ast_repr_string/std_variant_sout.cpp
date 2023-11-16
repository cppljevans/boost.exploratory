//Purpose:
//  Explore why operator<< for std::variant fails in
//  spirit's print_attribute.hpp.
//=====================
#include <iostream>
  template
  < unsigned I
  >
  struct
type_i
  {
        friend
      std::ostream&
    operator<<
      ( std::ostream&sout
      , type_i const&x
      )
      { return sout<<"type_i<"<<I<<">"
      ;}
  };
#include <boost/spirit/home/x3/support/ast/std_variant.hpp>
#define BOOST_SPIRIT_X3_DEBUG // Provide some meaningful reassuring output.
#ifdef BOOST_SPIRIT_X3_DEBUG
  #define BOOST_SPIRIT_X3_DEBUG_OUT std::cout
  #include <boost/spirit/home/x3/nonterminal/simple_trace.hpp>
#endif
  int
main()
  {
  ; boost::spirit::x3::std_variant<type_i<0>,type_i<1>,type_i<2>> var
#ifdef BOOST_SPIRIT_X3_DEBUG
  ; {
    ; std::string input="0123456789"
    ; auto first=input.begin()
    ; auto last=input.end()
    ; std::string rule_name="rule_dummy"
    ; auto& tracer=boost::spirit::x3::detail::get_simple_trace()
    ; std::cout<<"***status=pre_parse;\n";
    ; tracer(first,last,var,boost::spirit::x3::pre_parse,rule_name)
    ; std::cout<<"***status=successful_parse;\n";
    ; tracer(first,last,var,boost::spirit::x3::successful_parse,rule_name)
    ;}
#endif//BOOST_SPIRIT_X3_DEBUG
  ; return 0
  ;}
