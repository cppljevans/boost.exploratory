//Purpose:
//  provide test case for issue707:
//    
/*
https://github.com/boostorg/spirit/issues/707
 */
//=============
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <string>
/*=============================================================================
    Copyright (c) 2022 Larry Joe Evans

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <boost/spirit/home/x3.hpp>

#include <vector>

#include "test.hpp"

namespace x3 =  boost::spirit::x3;


namespace issue707
{
    struct A{}; A a;
    struct B{}; B b;
    struct C{}; C c;
    struct D{}; D d;
    struct uda_s1
      { 
        C c;
        A a;
        B b;
      }; 
    struct uda_s2
      { 
        A a;
        B b;
      }; 
    using uda_var=
      x3::variant
      < uda_s1
      , uda_s2
      >
      ; 
    struct uda_data
      {
        D d;
        uda_var v;
      };
}
    BOOST_FUSION_ADAPT_STRUCT(issue707::uda_s1, c, a, b);
    BOOST_FUSION_ADAPT_STRUCT(issue707::uda_s2, a, b);
    BOOST_FUSION_ADAPT_STRUCT(issue707::uda_data, d, v);
int
main()
{
    using spirit_test::test_attr;
    using namespace issue707;
    
    auto parser_s1=
        x3::attr(c)
      >>x3::attr(a) 
      >>x3::attr(b)
      ;
    auto parser_s2=
        x3::attr(a) 
      >>x3::attr(b)
      ;
    auto parser_var=
        parser_s1
      | parser_s2
      ;
    auto parser_data=
        x3::attr(d)
      >>parser_var
      ;
    auto parser_vec=
      x3::repeat(2)[parser_data]
      ;
    using uda_vec=
      std::vector
      < uda_data
      >
      ;
      
    uda_vec uda_vec_v;
    char const* inp = "";
    BOOST_TEST(test_attr( inp, parser_vec, uda_vec_v));
    
    return boost::report_errors();
}        
