//Purpose:
//  format template expressions read from std::cin.
#include <boost/iostreams/utility/templ_expr/fmt.hpp>

namespace rm_hash_comments
{ using namespace boost::xpressive;
    std::string
  doit
    ( std::string txt
    )
    { sregex rex='#' >> *~_n >> eol
    ; std::string fmt=""
    ; return regex_replace(txt,rex,fmt)
    ;}
}
#include <iterator>
#include <iostream>
  int
main()
  {
  ; boost::iostreams::indent_scoped_ostreambuf<char>
  indent_outbuf(std::cout,2)
  ; std::istreambuf_iterator<char> beg{std::cin}, end;
  ; std::string input_v(beg,end);
  //; std::cout<<__func__<<':'<<__LINE__<<":input_v=\n"<<input_v<<";\n"
  ; input_v=rm_hash_comments::doit(input_v)
  //; std::cout<<__func__<<':'<<__LINE__<<":input_v=\n"<<input_v<<";\n"
  ; std::string fmt=boost::spirit::x3::templ_expr::fmt(input_v);
  ; std::cout<<__func__<<':'<<__LINE__<<":fmt=\n"<<fmt<<";\n"
  ; return 0
  ;}
