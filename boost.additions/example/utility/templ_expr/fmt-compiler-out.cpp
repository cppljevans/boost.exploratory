//Purpose:
//  format clang++ compiler error messages.
//==============================
#include <boost/iostreams/utility/templ_expr/fmt.hpp>
using namespace boost::xpressive;

#include <string>
#include <iterator>
#include <iostream>
  sregex msg_header=bol >> "C:/msys64/home/evansl/prog_dev/"
; sregex msg_end=bol >> +_d >> _s >> "errors generated."
; sregex msg_rex
  = (s1=
      (  msg_header
      >> -+_
      >> before
         ( msg_header
         | msg_end
         )
      )
    )
; sregex single_quote
  = as_xpr("'")
; sregex quote_rex
  =  single_quote
  >> -+_
  >> single_quote
;
#include <boost/iostreams/utility/templ_expr/fmt.hpp>
  int
main()
  {
  ; boost::iostreams::indent_scoped_ostreambuf<char>
  indent_outbuf(std::cout,2)
  ; std::istreambuf_iterator<char> cin_beg{std::cin}, cin_end;
  ; std::string input(cin_beg,cin_end);
  //; std::cout<<__LINE__<<":input=\n"<<input<<";\n"
  ; sregex_token_iterator tok_end
  ; sregex_token_iterator msg_tok(input.begin(), input.end(), msg_rex, 1)
  ; for(unsigned msg_num=0; msg_tok != tok_end; ++msg_num,++msg_tok)
    { std::string msg_str=*msg_tok
    //; std::cout<<__LINE__<<":msg_str="<<msg_str<<";\n"
  #if 1
    ; auto msg_now=msg_str.begin()
    ; auto msg_end=msg_str.end()
    ; smatch quote_what
    ; bool quote_found=
      regex_search
      ( msg_now
      , msg_end
      , quote_what
      , quote_rex
      , regex_constants::match_default
      )
    ; while(quote_found)
      { std::cout<<quote_what.prefix()
      ; std::cout<<indent_buf_in
      ; std::string texpr=quote_what.str()
      ; { auto iter=texpr.begin()
        ; texpr.erase(iter)//rm beginning single quote
        ; iter=texpr.end()-1
        ; texpr.erase(iter)//rm ending single quote
        ;}
      ; std::string fmt_texpr=
        boost::spirit::x3::templ_expr::fmt(texpr)
      ; std::cout<<"'\n"<<fmt_texpr<<"\n'\n"
      ; std::cout<<indent_buf_out
      ; msg_str=std::string(quote_what.suffix().str())
      //; std::cout<<__LINE__<<":msg_str="<<msg_str<<";\n"
      ; msg_now=msg_str.begin()
      ; msg_end=msg_str.end()
      ; quote_found=
        regex_search
        ( msg_now
        , msg_end
        , quote_what
        , quote_rex
        , regex_constants::match_default
        )
      //; std::cout<<__LINE__<<":quote_found="<<quote_found<<";\n"
      ;}
    ; std::cout<<quote_what.suffix()<<"\n"
  #endif
    ;}
  //; std::cout<<__LINE__<<":return;\n"
  ; return 0
  ;}
