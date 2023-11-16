#include <vector>
#include <boost/fusion/support/is_sequence.hpp>
#include <iostream>
int main()
{ using vec=std::vector<int>
; using is_vec_seq=boost::fusion::traits::is_sequence<vec>
; std::cout<<"is_vec_seq="<<is_vec_seq::value<<";\n"
; return 0
;} 
