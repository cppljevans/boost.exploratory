//Purpose:
//  see why is_variant<boost::variant<T...>>::value is false
//  in is_same_enough.hpp:166
//============
#include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
using var=boost::variant<int,double>;
using is_var=boost::spirit::x3::traits::is_variant<var>;
using is_var_type=typename is_var::type;
#include <iostream>
int main()
{ 
    boost::iostreams::indent_scoped_ostreambuf<char>
  indent_outbuf(std::cout,2);
  std::cout<<":is_var="<<demangle_fmt_type<is_var>()<<";\n";
  std::cout<<":is_var_type="<<demangle_fmt_type<is_var_type>()<<";\n";
  std::cout<<":is_var::value="<<is_var::value<<";\n";
  return 0;
}
    
  
