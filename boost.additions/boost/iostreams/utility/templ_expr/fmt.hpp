#pragma once
/*! \file fmt.hpp
    \brief function fmt formats demangled typenames
       including specifics for boost/spirit/x3 typenames.
*/
//==========
#include <iostream>
#include <sstream>
#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
#include <string>
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_primitives.hpp>
#include <boost/xpressive/regex_actions.hpp>

namespace boost { namespace spirit { namespace x3
{
namespace templ_expr
{
namespace xpr=boost::xpressive;
using namespace xpr;
unsigned constexpr indent_width=2
  /**<
   * @brief indentation width
   *  
   *  Each time output indentation is increased or decreased,
   *  it's increased or decreased by this amount.
   */
  ;
  using
media_t
  =std::string
  ;
  /*!< 
   * @brief the media type on which input is read and formatted output is written.
   */
/*! \struct out_nst_res_t
    \brief provides debug output.
 */
struct out_nst_res_t
{
    template< typename BidiIterT >
    void operator ()( match_results< BidiIterT > const &what )const
    {
        // output the match
        std::cout <<":size="<<what.size()<<":position="<<what.position()<<":what[0]=\n"<< what[0] << ";\n";

        // output any nested matches
        using match_results_t=match_results< BidiIterT >;
        std::cout<<indent_buf_in;
        typename match_results_t::nested_results_type const&nst_res=what.nested_results();
        std::for_each
        ( nst_res.begin()
        , nst_res.end()
        , (*this)
        );
        std::cout<<indent_buf_out;
    }
};//out_nst_res_t struct
out_nst_res_t out_nst_res_v;
#ifdef DOXY_REF_FMT_FUN
 media_t fmt(media_t)
  //MAINTENANCE_NOTE(2022-03-22):
  //  The **ONLY** reason for this forward declaration
  //  is to allow doxygen to properly process:
  //    @ref fmt(media_t)
  //  as used below.
  ;
#endif//DOXY_REF_FMT_FUN  
/** \defgroup cond_out conditional output
 *  \brief These types, variables and functions enable conditional output to *sout.
 *  @{
 */
  std::ostream*
sout=0
 /**<@brief a "control flag" controlling whether output occurs during parsing.
  *  Also, when non-null, *sout serves as the output stream where formatted output is written.
  *
  *  The term, "control flag", is defined in the following web sites:
  *
  *  - <a href="https:*www.geeksforgeeks.org/use-of-flag-in-programming/">use-of-flag</a>
  *  - <a href="https:*press.rebus.community/programmingfundamentals/chapter/flag-concept/">flag-concept</a>
  *
  *  This flag is needed because the method used for the 
  *  @ref fmt(media_t) 
  *  function is to:
  *
  *  - disable output while doing replacements which "massages" text to make it easier to parse.
  *  - do the actual output of the massaged text while parsing.
  */
  ;
void out_ident(ssub_match const&val)
/**@brief simply output val.
 */
  { if(sout)(*sout)<<val
  ;}
void out_open(ssub_match const&val)
/**@brief output opening delimiter, val, then increase indentation.
 */
  { if(sout)(*sout)<<'\n'<<std::left<<std::setw(indent_width)<<val<<indent_buf_in
  ;}
void out_between(ssub_match const&val)
/**@brief output properly indented in-between delimiter, val
 */
  { if(sout)
    { std::string delim=val
    ; if(delim=="::") 
      { (*sout)<<delim
      ;}
      else
      { (*sout)<<indent_buf_out<<'\n'<<std::left<<std::setw(indent_width)<<delim<<indent_buf_in
      ;}
    ;}
  ;}
void out_close(ssub_match const&val)
/**@brief decrease indentation, then output closing delimiter, val.
 */
  { if(sout)(*sout)<<indent_buf_out<<'\n'<<val
  ;}
void out_noop(ssub_match const&)
/**@brief do nothing.
 */
  {
  ;}
  struct
action_out_t
  /**@brief action for "conditionally" producing output while parsing.
   */
  {
    using out_fun_t=void(*)(ssub_match const&);
    out_fun_t out_fun_v;
    action_out_t(out_fun_t f=out_ident)
    : out_fun_v(f)
    {}
    // Result type, needed for tr1::result_of
    typedef void result_type;

    void operator()(ssub_match const &val) const
    {
        out_fun_v(val);
    }  
  };
  using
action_fun=function<action_out_t>::type
  ;
 /*!<
  * @brief the type of semantic action functions named action_*.
  */
  action_fun
action_ident   = {{out_ident  }},
action_open    = {{out_open   }},
action_between = {{out_between}},
action_close   = {{out_close  }};
  /**<@brief parser semantic actions.
   */
/** @}*////cond_out defgroup
  template
  < typename Delim
  >
  sregex
list_delim
  ( sregex const& element
  , Delim const& delim
  )
  /**@brief
   *  a list of elements separated by delim
   */
  { return element >> *(delim >> element)
  ;}
  
/** \defgroup type_id_grammar xpressive grammar defining type_id 
 *  \brief These xpressive::sregex's define a grammar for 
 *   demangled template expressions.
 *  @{
 */
  sregex type_id 
  /** @brief main grammar
   *
   *   This is a forward declaration because the grammar defined by
   *   type_id is recursive, requiring a separate definition
   *   further down and use of the ref_type_id.
   */
; sregex ref_type_id = by_ref(type_id)
  /** @brief eases expressing recursion on type_id.
   */
; sregex identifier = (+_w)[action_ident(_)]
; sregex signed_opt_num=!as_xpr('-') >> +_d
; sregex ul_literal=icase((set='u','l'))
; sregex ul_literal0_3=xpr::repeat<0,3>(ul_literal)
; sregex integer_literal=skip(_s)(signed_opt_num >> ul_literal0_3)[action_ident(_)]
; sregex paren_open=skip(_s)(as_xpr('('))[action_ident(_)]
; sregex paren_close=skip(_s)(as_xpr(')'))[action_ident(_)]
; sregex angle_open=skip(_s)(as_xpr('<'))[action_open(_)] 
; sregex angle_close=skip(_s)(as_xpr('>'))[action_close(_)]
; sregex comma=skip(_s)(as_xpr(','))[action_between(_)] 
; sregex scope=skip(_s)(as_xpr("::"))[action_ident(_)]
; sregex cv_qualifier=skip(_s)(as_xpr("const")|"volatile")[action_ident(_)]
; sregex ptr_operator=skip(_s)(as_xpr('*')|'&'|"&&")[action_ident(_)]
; sregex enum_literal
  =  paren_open   
  >> ref_type_id 
  >> paren_close
  >> integer_literal
; sregex template_argument
  = integer_literal
  | enum_literal
  | ref_type_id
; sregex templ_comma_arg_list=list_delim( template_argument, comma)
; sregex templ_arg_list 
  = angle_open
  >> templ_comma_arg_list
  >> angle_close
; sregex id_opt_args 
  = skip(_s)
  (  identifier 
  >> !templ_arg_list
  )
; sregex def_type_id 
  =  skip(_s)
  (  !cv_qualifier
  >> list_delim( id_opt_args, scope)
  >> !cv_qualifier
  >> !ptr_operator
  )
; sregex std_string=
  skip(_s)
  ( as_xpr("std::__cxx11::basic_string")
  >> angle_open
  >> "char"
  >> comma
  >> "std::char_traits<char>"
  >> !( comma
      >> "std::allocator<char>"
      )
  >> angle_close
  )
; sregex std_allocator
  /**@brief std::allocator<type_id>
   */
  = skip(_s)(comma >> "std::allocator" >> angle_open >> ref_type_id >> angle_close)
;
/** @}*///type_id_grammar defgroup

; using replace_args=
  std::vector
  < std::pair
    < sregex //replace this
    , std::string //with this
    >
  >
  ;
  void
replace_map_apply
  ( media_t& txt 
  , replace_args const& replace_map
  )
  {
  ; auto it=replace_map.begin()
  ; auto end=replace_map.end()
  ; for(;it!=end;++it)
    { sregex key=it->first
    ; std::string val=it->second
    ; txt=regex_replace(txt,key,val)
    ;}
  ;}
; sregex spaces=+_s
;
  /** @var replace_map_std
   *  @brief args to regex_replace to be applied sequentially.
   *     These args are mostly tailored to types from std namespace.
   */
  replace_args
replace_map_std
  { { spaces, " "}//replace multiple whitespace with single space.
  , { std_allocator, ""}//remove std::allocator<...>
  , { std_string, "std::string"}
  , { as_xpr("const char *"), "const_char_ptr"}
  };
  ;
  /** @var replace_map_std
   *  @brief args to regex_replace to be applied sequentially.
   *     These args are mostly tailored to types from spirit::x3.
   */
  replace_args
replace_map_x3
  { { as_xpr("const boost::spirit::x3::unused_type"), "const_x3_unused_type"}
  , { as_xpr("boost::spirit::x3::unused_type"), "x3_unused"}
  , { as_xpr("boost::spirit::x3::int_parser<short, 10u, 1u, -1>"), "x3_int16"}
  , { as_xpr("boost::spirit::x3::int_parser<int, 10u, 1u, -1>"), "x3_int32"}
  , { as_xpr("boost::spirit::x3::int_parser<long long, 10u, 1u, -1>"), "x3_int64"}
  , { as_xpr("boost::spirit::x3::uint_parser<unsigned short, 10u, 1u, -1>"), "x3_uint16"}
  , { as_xpr("boost::spirit::x3::uint_parser<unsigned int, 10u, 1u, -1>"), "x3_uint32"}
  , { as_xpr("boost::spirit::x3::uint_parser<unsigned long long, 10u, 1u, -1>"), "x3_uint64"}
  , { as_xpr("boost::spirit::x3::real_parser<double, boost::spirit::x3::real_policies<double> >"), "x3_double"}
  , { as_xpr("boost::spirit::x3::literal_char<boost::spirit::char_encoding::standard, boost::spirit::x3::unused_type>"), "x3_char_unused"}
  , { as_xpr("boost::spirit::x3::sequence"), "x3_sequence"}
  , { as_xpr("boost::spirit::x3::alternative"), "x3_alternative"}
  , { as_xpr("boost::spirit::x3::list"), "x3_list"}
  , { as_xpr("boost::fusion"), "fusion"}
  , { _b>>"unsigned short">>_b, "uint16_t"}
  , { _b>>"unsigned int">>_b, "uint32_t"}
  , { _b>>"unsigned long long">>_b, "uint64_t"}
  , { _b>>"short">>_b, "int16_t"}
  , { _b>>"int">>_b, "int32_t"}
  , { _b>>"long long">>_b, "int64_t"}
  };
  media_t
fmt
  ( media_t txt
    //a template expression, such as one produced by 
    //  * a compiler diagnostic
    //  * output from demangle_type<T>()
    //    defined here ../demangle_type.hpp.
  )
  /**@brief
   *  return formated txt.
   */
  {
  ; media_t str_result=txt//preserve original input in case of error reporting.
  ; type_id=skip(_s)(def_type_id)
  ; { //do replacements:
    ; sout=0 //disable output
    //#define TRACE_TEMPL_EXPR_FMT
    #ifdef TRACE_TEMPL_EXPR_FMT
    ; media_t inp
    ; std::cout<<__func__<<':'<<__LINE__<<':'<<"txt=\n"<<txt<<";\n"
    ; inp=txt
    #endif//TRACE_TEMPL_EXPR_FMT
    ; replace_map_apply(txt,replace_map_std);
    ; replace_map_apply(txt,replace_map_x3);
    #ifdef TRACE_TEMPL_EXPR_FMT
    ; std::cout<<__func__<<':'<<__LINE__<<':'<<"txt=\n"<<txt<<";\n:same="<<(inp==txt)<<";\n"
    #endif//TRACE_TEMPL_EXPR_FMT
    ;}
  ; { //do formatting:
    ; std::ostringstream tmp_out
    ; boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(tmp_out,templ_expr::indent_width)
    ; sout=&tmp_out//enable output
    ; bool bool_result
    ; smatch what
    ; bool_result=
        regex_search
        ( txt
        , what
        , type_id
        , regex_constants::match_partial
          //|regex_constants::match_any
        )
    ; tmp_out<<std::flush
    ; if(bool_result)  
      { str_result=tmp_out.str()//formatted result
      ;}
      else
      {
    #if 0
      ; std::cout<<"failed match:what=\n"
      ; out_nst_res_v(what)
      ; std::string matched=tmp_out.str()
      ; std::cout<<":matched="<<matched<<";\n"
    #endif
      ; str_result="!!!not type_expr=\n"+str_result//error result
      ;}
    ;}
  ; return str_result
  ;}
}//templ_expr namespace
}}}
