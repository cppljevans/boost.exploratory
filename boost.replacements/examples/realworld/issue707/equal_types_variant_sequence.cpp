//Purpose:
//  see if mpl::equal works same on boost::variant and fusion::deque
//========
#include <iostream>
#include <boost/mpl/equal.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/container/deque.hpp>
int main()
  { 
#if 0
  ; { using x0=boost::variant<int,char>
    ; using x1=boost::variant<int,char>
    ; using equal=boost::mpl::equal<x0,x1>::type
    ; std::cout<<__LINE__<<':'<<equal::value<<";\n"
    ;}
  ; { using x0=boost::variant<int,char>
    ; using x1=boost::variant<char,int>
    ; using equal=boost::mpl::equal<x0,x1>::type
    ; std::cout<<__LINE__<<':'<<equal::value<<";\n"
    ;}
#endif    
  ; { using x0=boost::fusion::deque<int,char>
    ; using x1=boost::fusion::deque<int,char>
    ; using equal=boost::mpl::equal<x0,x1>::type
    ; std::cout<<__LINE__<<':'<<equal::value<<";\n"
    ;}
  ; { using x0=boost::fusion::deque<int,char>
    ; using x1=boost::fusion::deque<char,int>
    ; using equal=boost::mpl::equal<x0,x1>::type
    ; std::cout<<__LINE__<<':'<<equal::value<<";\n"
    ;}
  ; return 0
  ;}
//======
//Result:
//  Erroneously returns true when should be false on line 17:
/*
clang++ -c -O0 -gdwarf-2 -g3 -ftemplate-backtrace-limit=0  -std=c++2b -ftemplate-backtrace-limit=0 -fdiagnostics-show-template-tree -fno-elide-type -fmacro-backtrace-limit=0 -fexceptions -fcxx-exceptions     -I/home/evansl/prog_dev/boost.org/1_80_0download/download     -ftemplate-depth=200  mpl_equal_variants.cpp -MMD -o c:/msys64/tmp/build/clangxx14_0_3/boost.org/boost.exploratory/boost.replacements/examples/realworld/issue707-80/mpl_equal_variants.o 
clang++    c:/msys64/tmp/build/clangxx14_0_3/boost.org/boost.exploratory/boost.replacements/examples/realworld/issue707-80/mpl_equal_variants.o   -o c:/msys64/tmp/build/clangxx14_0_3/boost.org/boost.exploratory/boost.replacements/examples/realworld/issue707-80/mpl_equal_variants.exe
c:/msys64/tmp/build/clangxx14_0_3/boost.org/boost.exploratory/boost.replacements/examples/realworld/issue707-80/mpl_equal_variants.exe 
12:1;
17:1;

Compilation finished at Wed Sep 21 03:46:13
 */
   
