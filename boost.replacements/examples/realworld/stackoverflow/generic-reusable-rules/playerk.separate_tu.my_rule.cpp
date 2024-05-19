/** \file playerk.separate_tu.my_rule.cpp
 */
#include "playerk.separate_tu.my_rule.hpp"
const auto my_rule_def = 
  x3::byte_ >> x3::byte_
  ;
BOOST_SPIRIT_DEFINE(my_rule)
BOOST_SPIRIT_INSTANTIATE(my_rule_type, typename stream::iterator, x3::unused_type)
