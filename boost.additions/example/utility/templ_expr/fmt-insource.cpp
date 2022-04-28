//Purpose:
//  Demonstrate fmt function in the following #include
//  using insource test cases.
//==========
#include <boost/iostreams/utility/templ_expr/fmt.hpp>
  int 
main()
  {
  ; boost::iostreams::indent_scoped_ostreambuf<char>
  indent_outbuf(std::cout,templ_expr::fmt_width)
  //the input:
  ; std::string txt
  #if 0
  ; { txt="xxx"
    ; unsigned nesting_depth=5
    ; for(unsigned i=0; i<nesting_depth; ++i)
      { txt="std::vector<"+txt+",std::allocator<"+txt+">>"
      ;}
    ; std::cout<<__LINE__<<":depth="<<nesting_depth<<":txt=\n"<<txt<<";\n"
    ;}
  #endif
  #if 0
  ; { std::string ptr="&"
    ; txt="xxx "+ptr
    ; unsigned nesting_depth=5
    ; for(unsigned i=0; i<nesting_depth; ++i)
      { txt="std::vector<"+txt+",std::allocator<"+txt+">>"+ptr
      ;}
    ; std::cout<<__LINE__<<":depth="<<nesting_depth<<":txt=\n"<<txt<<";\n"
    ;}
  #endif
  #if 0
  ; { txt="std::vector<boost::spirit::x3::std_variant<std::vector<boost::spirit::x3::std_variant<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, boost::fusion::deque<char, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, char> >, std::allocator<boost::spirit::x3::std_variant<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, boost::fusion::deque<char, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, char> > > >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<boost::spirit::x3::std_variant<std::vector<boost::spirit::x3::std_variant<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, boost::fusion::deque<char, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, char> >, std::allocator<boost::spirit::x3::std_variant<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, boost::fusion::deque<char, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, char> > > >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >"
    //the above from Edit_2021-05-30.rule_attr_rhs.xform2rhs.cpp:270:start::post;
    //demangle_type<Exposed>()
    ;}
  #endif
  #if 0
  ; { txt="www<xxx<yyy>&>::value"
    ;}
  #endif
  #if 0
  ; { txt="is_constructible<boost::variant<std::vector<boost::variant<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, boost::fusion::deque<char, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, char>>, std::allocator<boost::variant<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, boost::fusion::deque<char, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, char>>>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, boost::variant<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, boost::fusion::deque<char, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, char>> &>::value"
    ;}
  #endif
  ; { //the formatting:
    ; std::cout<<":fmt=\n"<<templ_expr::fmt(txt)<<";\n"
    ;}
  ; return 0
  ;}
