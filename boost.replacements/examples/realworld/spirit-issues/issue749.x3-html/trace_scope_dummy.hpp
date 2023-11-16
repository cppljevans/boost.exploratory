#pragma once
  struct trace_scope
    {
        std::string tag;
      //#define TRACE_SCOPE_DUMMY_TRACE
        trace_scope(std::string tag_)
          : tag(tag_)
          {
            #ifdef TRACE_SCOPE_DUMMY_TRACE
              std::cout<<"{tag="<<tag<<";\n";
            #endif
          }
        ~trace_scope()
          {  
            #ifdef TRACE_SCOPE_DUMMY_TRACE
              std::cout<<"}tag="<<tag<<";\n";
            #endif
          }
    };
