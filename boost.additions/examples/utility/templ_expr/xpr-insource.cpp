//Purpose:
//  Demonstrate fmt function in the following #include
//  using insource test cases.
//==========
#include "output_nested_results.hpp"
  int 
main()
  {
  ; unsigned constexpr fmt_width=2
  ; boost::iostreams::indent_scoped_ostreambuf<char>
  indent_outbuf(std::cout,fmt_width)
  ; std::string txt
  #if 0
  ; { txt=" xxx yyy zzz"
    ; sregex tok_rex=
      _s
    ; sregex_token_iterator tok_end
    ; sregex_token_iterator tok_iter(txt.begin(), txt.end(), tok_rex, -1)
    ; for(unsigned tok_num=0; tok_iter != tok_end; ++tok_num,++tok_iter)
      { std::string tok_str=*tok_iter
      ; std::cout<<"***tok_num:"<<tok_num<<"="<<tok_str<<";\n";
      ;}    
    ;}
  #endif
  #if 0
  ; { txt=" 'xxx' 'yyy' 'zzz'"
    ; sregex tok_rex=
      xpr::as_xpr('\'')
    ; sregex_token_iterator tok_end
    ; sregex_token_iterator tok_iter(txt.begin(), txt.end(), tok_rex, -1)
    ; for(unsigned tok_num=0; tok_iter != tok_end; ++tok_num,++tok_iter)
      { std::string tok_str=*tok_iter
      ; std::cout<<"***tok_num:"<<tok_num<<"="<<tok_str<<";\n";
      ;}    
    ;}
  #endif
  #if 1
  ; { txt=" 'xxx' 'yyy' 'zzz'"
    ; sregex single_quote=xpr::as_xpr('\'')
    ; sregex tok_rex=
      single_quote >> +alnum >> single_quote
    ; bool result
    ; smatch what
    ; result=regex_search(txt.begin(), txt.end(), what, tok_rex)
    ; std::cout<<__func__<<";"<<__LINE__<<":result="<<result<<";\n"
    ; if(result) 
      { std::cout<<":what=\n"
      ; output_nested_results_v(what)
      ;}
    ;}
  #endif
  ; return 0
  ;}
