#include <iostream>
  template<unsigned Index>
  struct
type_index
  {
    constexpr auto index(){ return Index;}
        friend
      std::ostream&
    operator<<
      ( std::ostream&sout
      , type_index const& x
      )
      {  return sout<<"type_index<"<<Index<<">"
      ;}
  };
#include <variant>
int main()
{
; type_index<1> e
; using variant_t=
  std::variant
  < type_index<0>
  , type_index<1>
  , type_index<2>
  >
; variant_t v
; v=e
; constexpr auto index=e.index()
; std::cout<<":index="<<index<<";\n"
; std::cout<<":e="<<v.index()<<";\n"
; constexpr auto var_size=std::variant_size_v<decltype(v)>
; std::cout<<":var_size="<<var_size<<";\n"
; return 0
;}
