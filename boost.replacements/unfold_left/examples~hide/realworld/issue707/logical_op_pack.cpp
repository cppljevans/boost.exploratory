#include <boost/mpl/bool.hpp>  
    template< bool (*Op)(bool,bool), bool A, bool B>
    struct apply_values
    : boost::mpl::bool_
      < Op(A,B)
      >
    {};
#include <iostream>    
int main()
  {
  ; using v=apply_values<||,false,false>
  ; std::cout<<__LINE__<<":v="<<v::value<<";\n"
  ; return 0
  ;}
