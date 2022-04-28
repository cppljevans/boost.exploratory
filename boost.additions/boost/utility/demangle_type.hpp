//demangle typename.
#ifndef BOOST_UTILITY_DEMANGLE_HPP_LJE20160607
#define BOOST_UTILITY_DEMANGLE_HPP_LJE20160607
#define USE_BOOST_TYPEINFO
#ifdef USE_BOOST_TYPEINFO
//Reference:
//  http://www.boost.org/doc/libs/1_56_0/boost/core/demangle.hpp
//
#include <boost/core/demangle.hpp>
template<typename Type>
std::string demangle_type()
{
  std::string mangled=typeid(Type).name();
  std::string dem=boost::core::demangle(mangled.c_str());
  return dem;
}
template<typename Type>
std::string demangle_type(Type x)
{
  std::string mangled=typeid(Type).name();
  std::string dem=boost::core::demangle(mangled.c_str());
  return dem;
}
#else
#include <cxxabi.h>
#endif
//#include <typeinfo>
//#include <iostream>

#endif
