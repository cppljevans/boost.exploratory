#pragma once
//Format the result of demangle_type.
//===========
#include <boost/utility/demangle_type.hpp>
#include <boost/iostreams/utility/templ_expr/fmt.hpp>
namespace boost { namespace spirit { namespace x3
{
namespace templ_expr
{
template<typename Type>
std::string demangle_fmt_type()
{ std::string unfmt=demangle_type<Type>()
//; std::cout<<__func__<<":unfmt="<<unfmt<<";\n"
; return templ_expr::fmt(unfmt)
;}
template<typename Type>
std::string demangle_fmt_type(Type)
{
  return demangle_fmt_type<Type>();
}
}//templ_expr namespace
}}}
using boost::spirit::x3::templ_expr::demangle_fmt_type;
