#pragma once
#include <iostream>
#include <vector>
//#define USE_VARIANT_IO_WHICH
#ifdef USE_VARIANT_IO_WHICH
#define BOOST_VARIANT_DETAIL_VARIANT_IO_HPP
  //^don't #include <boost/variant/detail/variant_io.hpp>
  //to avoid the ambiguous def of operator<< for variant's.
#endif
#include <boost/variant.hpp>
  template
  < typename T
  >
  std::ostream&
operator<<
  ( std::ostream&sout
  , std::vector<T>const&x
  )
  ;
#ifdef USE_VARIANT_IO_WHICH  
  template
  < typename... T
  >
  std::ostream&
operator<<
  ( std::ostream&sout
  , boost::variant<T...>const&x
  )
  ;
#endif  
  template
  < typename T
  >
  std::ostream&
operator<<
  ( std::ostream&sout
  , std::vector<T>const&x
  )
  {
    for(unsigned i=0; i<x.size(); ++i)
    { if(i==0) sout<<"[ "; else sout<<", "; 
      sout<<i<<"="<<x[i];
    }
    return sout<<"]";
  }
#ifdef USE_VARIANT_IO_WHICH  
  template
  < typename... T
  >
  std::ostream&
operator<<
  ( std::ostream&sout
  , boost::variant<T...>const&x
  )
  {
    std::cout<<"(: "<<x.which()<<", ";
    boost::apply_visitor
    ( [](auto val){ std::cout<<val;}
    , x
    );
    return sout<<":)";
  }
#endif
