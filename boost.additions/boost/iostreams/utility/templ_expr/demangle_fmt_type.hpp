#pragma once
//Format the result of demangle_type.
//===========
#include <boost/utility/demangle_type.hpp>
#include <boost/iostreams/utility/templ_expr/fmt.hpp>
namespace boost 
{ 
namespace iostreams
{
namespace utility
{
namespace templ_expr
{
template<typename Type>
std::string demangle_fmt_type()
{ std::string unfmt=demangle_type<Type>()
#if 0 || defined(DEMANGLE_FMT_TYPE_BUG_2023_01_06_1543_RESOLVED) || !defined(USE_FMT_XPRESSIVE)
//; std::cout<<__func__<<":unfmt="<<unfmt<<";\n"
; return templ_expr::fmt(unfmt)
#else
; return unfmt
#endif
;}
template<typename Type>
std::string demangle_fmt_type(Type)
{
  return demangle_fmt_type<Type>();
}
}//templ_expr namespace
}//utility namespace
}//iostreams namespace
}//boost namespace
using boost::iostreams::utility::templ_expr::demangle_fmt_type;
