//Purpose:
//  See if fold expressions:
//    https://en.cppreference.com/w/cpp/language/fold
//  are valid.
//Result:
//  when COMPILE.clangxx.flags.std=-std=c++17, prints yes.
//  when COMPILE.clangxx.flags.std=-std=c++11, prints not.
//=======
#include <iostream>
int main()
{
; std::cout<<"fold_expr_valid="
#ifdef __cpp_fold_expressions
; std::cout<<"yes;\n"
#else
; std::cout<<"not;\n"
#endif
; return 0
;}
