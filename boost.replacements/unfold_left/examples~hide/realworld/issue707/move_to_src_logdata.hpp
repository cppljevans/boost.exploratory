#pragma once
#ifdef MOVE_TO_SRC_LOGDATA
//#define USE_BOOST_STACKTRACE
#ifdef USE_BOOST_STACKTRACE
  #include <boost/stacktrace.hpp>
  namespace bs = boost::stacktrace;
#endif
namespace boost{namespace spirit{namespace x3{namespace traits{
    unsigned move_to_count=0;
    template<typename Source>
    inline void move_to(Source& src, LogData& dest)
    {
        boost::trace_scope ts(stringify(__FILE__,":LINE=",__LINE__,":",__func__,"(Source&,LogData&)"));
      #ifdef USE_BOOST_STACKTRACE
        std::cout<< "boost::stacktrace:\n";
        std::cout<<bs::stacktrace()<<";\n";
          //The above provides **NO** file or line info. despite claims under the:
          //  https://www.boost.org/doc/libs/1_66_0/doc/html/stacktrace/getting_started.html#stacktrace.getting_started.how_to_print_current_call_stack
          //Instead, it prints:
          /*
            0# 0x00007FF70871E7C3 in C:\msys64\tmp\build\clangxx14_0_3\boost.org\boost.exploratory\boost.replacements\unfold_left\examples\realworld\issue707-78\1.78breakage-simplified4.exe
            1# 0x00007FF70873A535 in C:\msys64\tmp\build\clangxx14_0_3\boost.org\boost.exploratory\boost.replacements\unfold_left\examples\realworld\issue707-78\1.78breakage-simplified4.exe
            2# 0x00007FF70872AD73 in C:\msys64\tmp\build\clangxx14_0_3\boost.org\boost.exploratory\boost.replacements\unfold_left\examples\realworld\issue707-78\1.78breakage-simplified4.exe 
            .
            .
            .
           */
      #endif
        dest.f0=move_to_count;//just to provide some evidence, in dest, that this function was called.
        std::cout
        <<":move_to_count="<<move_to_count++
        <<":demangle_fmt_type(src)="
        <<demangle_fmt_type(src)
        <<":src="<<src;
        std::cout<<":dest=LogData"
        <<";\n";
    }
}}}}
#endif//MOVE_TO_SRC_LOGDATA
