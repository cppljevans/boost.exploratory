#pragma once
#include <initializer_list>
#include <string>
#include <boost/spirit/home/x3/support/traits/print_attribute.hpp>
namespace unit_tests 
{
      template 
      < bool ShouldSucceed = true
      , typename P
      >
      void 
    test
      ( P const& start
      , std::initializer_list<std::string> cases
      ) 
      {
          //std::cout << ":ShouldSucceed="<<ShouldSucceed<<";\n";
          using attr_t=typename x3::traits::attribute_of<P, x3::unused_type>::type;
          for (auto input : cases) 
          {
              attr_t attr_of;
              if constexpr (ShouldSucceed) 
              {
                  auto ok = phrase_parse(input.begin(), input.end(), start, x3::space, attr_of);
                  std::cout << quoted(input) << " -> " << __func__ << ':';
                  if (ok)
                      std::cout << "Ok,succeeded and should have succeeded.";
                  else
                      std::cout << "Not Ok,failed but should have succeeded!";
                  std::cout << std::endl;
                #if 0
                  std::cout<<":result attr_of=\n";
                  x3::traits::print_attribute(std::cout,attr_of);
                  std::cout<<";\n";
                #endif
              } 
              else 
              {
                  auto ok = phrase_parse(input.begin(), input.end(), start, x3::space, attr_of);
                  std::cout << quoted(input) << " -> " << __func__ << ':';
                  if (!ok)
                      std::cout << "Ok,failed and should have failed.";
                  else
                      std::cout << "Not Ok,succeeded but should have failed! ";
                  std::cout << std::endl;
              }
          }//for
      }//test
}//unit_tests
