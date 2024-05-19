/** \file playerk.separate_tu.my_rule.hpp
 */
//=============
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/binary.hpp>

namespace x3 = boost::spirit::x3;

  using
My_rule=
  boost::fusion::vector<int, int>
  ;
  using
my_rule_type=
  x3::rule<class My_rule_tag, My_rule>
  ;
  my_rule_type const
my_rule{ "my-rule" }
  ;
BOOST_SPIRIT_DECLARE(my_rule_type)
  using
stream_element=
  std::uint8_t const
  ;
#include <vector>
  using 
stream=
  std::vector<stream_element>
  ;
