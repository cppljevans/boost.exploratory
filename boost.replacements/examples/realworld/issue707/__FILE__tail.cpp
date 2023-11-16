#include <iostream>
#include <filesystem>
  int 
main()
  {
  ; std::cout<<"__FILE__="<<__FILE__<<";\n"
  ; std::string root_path_str="C:/msys64/home/evansl/prog_dev/boost.org/boost.exploratory/boost.replacements/debug/boost/spirit/home/x3/core/detail/parse_into_container.hpp"
  ; std::cout<<"root_path_str="<<root_path_str<<";\n"
  ; std::filesystem::path root_path_fs(root_path_str)
  ; std::cout<<"root_name="<<root_path_fs.root_name()<<";\n"
  ; std::cout<<"root_path="<<root_path_fs.root_path()<<";\n"
  ; std::cout<<"relative_path="<<root_path_fs.relative_path()<<";\n"
  ; auto end=root_path_fs.end()
  ; auto prev=[](auto cur, unsigned n=0)
    { for(unsigned i=0; i<n; ++i) --cur
    ; return cur
    ;}
  ; auto beg=prev(end,3)
  ; std::filesystem::path rel_path("<?>/",std::filesystem::path::auto_format)
  ; for(auto now=beg; now!=end; ++now)
    { std::cout<<"now="<<*now<<";\n"
    ; rel_path/=*now
    ;}
  ; std::cout<<"rel_path="<<rel_path.generic_string()<<";\n"
  ; return 0
  ;}
