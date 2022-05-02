#include <boost/spirit/home/x3/support/ast/put_variant.hpp>
template<unsigned I>
struct type
{
};
#include <iostream>
  template<unsigned I>
  std::ostream&
operator<<
  ( std::ostream&sout
  , type<I>
  )
  {  return sout<<"type<"<<I<<">";
  }
int main()
  {
    boost::variant<type<0>,type<1>,type<2>> x;
    put<1>(x);
    std::cout<<get<type<1>>(x)<<";\n";
    put<2>(x);
    std::cout<<get<type<2>>(x)<<";\n";
    return 0;
  }  
