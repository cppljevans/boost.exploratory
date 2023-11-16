#include <iostream>
  struct
undefined
  {
        friend
      std::ostream&
    operator<<
      ( std::ostream& os
      , undefined const&
      )
      {
      ; return os<<"undefined"
      ;}  
  };
  template
  < typename Tag
  >
  auto
def
  ( Tag
  )
  { return undefined()
  ;}
  enum 
tags
  { tag0
  , tag1
  , tag2
  };
  template
  < tags Tag
  >
  struct
tag
  {
        friend
      std::ostream&
    operator<<
      ( std::ostream& os
      , tag const&
      )
      {
      ; return os<<"tag<"<<Tag<<">"
      ;}  
  };
  auto
def
  ( tag<tag0>
  )
  { return tag<tag0>{}
  ;}
  auto
def
  ( tag<tag1>
  )
  { return tag<tag1>{}
  ;}
  int
main()
  {
  ; std::cout<<__LINE__<<def(tag<tag0>{})<<";\n"
  ; std::cout<<__LINE__<<def(tag<tag1>{})<<";\n"
  ; std::cout<<__LINE__<<def(tag<tag2>{})<<";\n"
  ; return 0
  ;}
