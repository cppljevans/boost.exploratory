#include <boost/mpl/and.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/mpl/vector.hpp>
using namespace boost::mpl;
using mpl_and=and_<true_,true_>;
using mpl_and_apply=and_<na,na>::apply<true_,true_>;
struct and_meta
{
  template<typename...T>
  struct apply
  : and_<T...>
  {};
};
using mpl_and_unpack=unpack_args<and_meta>;
using args=vector<true_,true_>;
using apply_mpl_unpack=apply_wrap2<mpl_and_unpack,args>;
//The above line fails with error:
/*
mpl_and_.cpp:16:24: error: too few template arguments for class template 'apply_wrap2'
using apply_mpl_unpack=apply_wrap2<mpl_and_unpack,args>;
                       ^

 */
//However, the docs here:
//  https://www.boost.org/doc/libs/1_78_0/libs/mpl/doc/refmanual/unpack-args.html
//says it should succeed.

#include <iostream>
int main()
  { std::cout<<mpl_and::type::value<<";\n"
  ; std::cout<<mpl_and_apply::type::value<<";\n"
  ; std::cout<<apply_mpl_unpack::type::value<<";\n"
  ; return 0
  ;}
