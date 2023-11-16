//Purpose:
//  show that demangle_fmt_type works.
#include <gram_algebra/templ_expr/demangle_fmt_type.hpp>
#include <vector>
struct A
{ struct B
  { struct C
    {
      using D=std::vector<int>;
    };
  };
};
  int
main()
  {
  ; boost::iostreams::indent_scoped_ostreambuf<char>
  indent_outbuf(std::cout,2)
  ; auto result=demangle_fmt_type<A::B::C::D>()
  ; std::cout<<"A::B::C::D=\n"<<result<<";\n";
  ; return 0
  ;}
