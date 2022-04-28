#ifndef BOOST_UTILITY_STRINGIFY_HPP
#define BOOST_UTILITY_STRINGIFY_HPP
#include <sstream>
  template
  < typename... ArgT
  >
  std::string
stringify(ArgT&&... arg_v)
  {
      std::ostringstream sout;
      ( sout<< ... << arg_v);
      return sout.str();
  }
#endif//BOOST_UTILITY_STRINGIFY_HPP
