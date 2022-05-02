//Purpose:
//  See how to print variants within vector because having trouble doing that
//  in alternative-debug.cpp.
//===========================
#include "std_vec_boost_var_io.hpp"
template<unsigned I>
struct type
  {
        friend
      std::ostream&
    operator<<
      ( std::ostream&sout
      , type
      )
      {  return sout<<"type<"<<I<<">";
      }
  };
int main()
  {
    std::cout<<"LINE="<<__LINE__<<":type<99>="<<type<99>{}<<";\n";
    std::vector<int> vec_int{1,2,3};
    std::cout<<"LINE="<<__LINE__<<":vec_int="<<vec_int<<"\n";
    using var_t=boost::variant<type<10>,type<11>,type<12>>;
    var_t var_v{type<10>{}};
    std::cout<<"var_v=";
    std::cout<<var_v<<";\n";
  #if 1
    std::vector<var_t> vec_var;
    vec_var.push_back(type<10>{});
    vec_var.push_back(type<12>{});
    std::cout<<"LINE="<<__LINE__<<":vec_var="<<vec_var<<"\n";
  #endif
    return 0;
  }  
