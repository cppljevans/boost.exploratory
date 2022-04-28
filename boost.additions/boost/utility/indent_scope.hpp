#ifndef BOOST_UTILITY_INDENT_SCOPE_HPP
#define BOOST_UTILITY_INDENT_SCOPE_HPP
#include <iostream>
#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
#include <string>
namespace boost
{
  template
  < typename Enter=std::string
  , typename Exit=Enter
  >
  class
indent_scope
  //Purpose:
  //  Indicate on a std::ostream when a scope entered and exited.
  {
  public:
    indent_scope
      ( bool doit
      , Enter a_enter=""
      , Exit a_exit=""
      , std::ostream& a_strm=std::cout
      )
      : my_doit(doit)
      , my_exit(a_exit)
      , my_strm(a_strm)
      { if(my_doit)
        { my_strm<<a_enter
        ; my_strm<<indent_buf_in
        ; my_strm.flush()
        ;}
      ;}
    ~indent_scope(void)
      { if(my_doit)
        { my_strm<<indent_buf_out
        ; my_strm<<my_exit
        ; my_strm.flush()
        ;}
      ;}
  private:
      bool
    my_doit
      ;
      Exit
    my_exit
      ;
      std::ostream&
    my_strm
      ;
  };//end indent_scope class
}//exit boost namespace
#endif
