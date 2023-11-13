#pragma once
//Dependencies:
//  Following code uses lexertl library mentioned here:
/*
https://www.boost.org/doc/libs/1_83_0/libs/spirit/doc/html/spirit/lex/tutorials/lexer_tutorials.html
http://www.benhanson.net/lexertl.html
 */
//Purpose:
//
//  Parse a text stream with confix syntax...
/*
https://github.com/avnomad/dop-parser
 */
//  The terminology used there is translated to terms used here as follows:
//    operand -> operand
//    initiator -> open
//    terminator -> close
//    separator -> split
//  It's assumed each open delimiter is matched with
//  the corresponding close delimiter.
//
//  ...and while parsing, output a formatted version of input
//  which highlights the structure of the input 
//  by using indentation and newlines.
//
//CodeDerivedFrom:
/*
https://github.com/BenHanson/lexertl14/blob/master/examples/wc/main.cpp
 */
//====================
//#include <lexertl/debug.hpp>
#include <lexertl/generator.hpp>
#include <lexertl/lookup.hpp>
namespace//unnamed namespace
{
    enum class fmt_state{eoi, operand, open, close, split, ws, newline};
    lexertl::rules rules_;
    lexertl::state_machine sm_;

    std::string delims_open ="<([";
    std::string delims_close=">)\\]";
    std::string delims_split=",";
    std::string delims_all=delims_open+delims_close+delims_split;
    struct build_rules
    {
      build_rules()
      {
        rules_.insert_macro("ws", "[ \t]");
        rules_.insert_macro("nonws", "[^ \t\n"+delims_all+"]");
        rules_.push
        ( "{nonws}+"
        , (const unsigned short)fmt_state::operand
        );
        rules_.push
        ( "["+delims_open+"]"
        , (const unsigned short)fmt_state::open
        );
        rules_.push
        ( "["+delims_close+"]"
        , (const unsigned short)fmt_state::close
        );
        rules_.push
        ( "["+delims_split+"]"
        , (const unsigned short)fmt_state::split
        );
        rules_.push
        ( "{ws}+"
        , (const unsigned short)fmt_state::ws
        );
        rules_.push
        ( "\n"
        , (const unsigned short)fmt_state::newline
        );
        lexertl::generator::build(rules_, sm_);
        //lexertl::debug::dump(sm_, std::cout);
      }
    } build_the_rules;
}//unamed namespace
#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
#include <sstream>
#include <string>
//#include <iostream>
namespace boost 
{ 
namespace iostreams
{
namespace utility
{
namespace templ_expr
{
  std::string
fmt
  ( std::string const&inp
  )
  {
  ; const char* inp_beg=inp.c_str()
  ; const char* inp_end=inp_beg+inp.size()
  ; lexertl::cmatch results_(inp_beg,inp_end)
  ; std::ostringstream tmp_out
  ; boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf
    ( tmp_out
    , 2
      //indentation amount.
      //This allows space for 1 char delimiter + 1 space=2
    )
  ; do
    {
        lexertl::lookup(sm_, results_);
        switch (results_.id)
        {
        case (int)fmt_state::eoi:
            break;
        case (int)fmt_state::operand:
            tmp_out
              //<<":operand="
              <<results_.str()
              <<' '
              ;
            break;
        case (int)fmt_state::open:
            tmp_out
              <<"\n"
              //<<":open="
              <<results_.str()<<" "
              <<indent_buf_in
              ;
            break;
        case (int)fmt_state::close:
            tmp_out
              <<indent_buf_out
              <<"\n"
              //<<":close="
              <<results_.str()
              ;
            break;
        case (int)fmt_state::split:
            tmp_out
              <<indent_buf_out
              <<"\n"
              //<<"split="
              <<results_.str()<<" "
              <<indent_buf_in
              ;
            break;
        case (int)fmt_state::ws:
            break;
        case (int)fmt_state::newline:
            break;
        default:
            assert(0);
            break;
        }
    } while (results_.id != (int)fmt_state::eoi)
  ; tmp_out<<std::flush
  ; return std::string(tmp_out.str())
  ;}
}//templ_expr namespace
}//utility namespace
}//iostreams namespace
}//boost namespace
