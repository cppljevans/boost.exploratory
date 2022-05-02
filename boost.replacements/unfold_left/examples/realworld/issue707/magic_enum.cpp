#include <magic_enum_io.hpp>
enum o_enum
{ o0
, o1
, o2
};
namespace inner
{
  enum i_enum
  { i0
  , i1
  , i2
  };
}
int main()
{
  std::cout<<"outer:0="<<o0<<":1="<<o1<<";\n";
  std::cout<<"inner:0="<<inner::i0<<":1="<<inner::i1<<";\n";
  return 0;
}
  
