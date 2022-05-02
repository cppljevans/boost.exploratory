#pragma once
#ifdef MOVE_TO_SRC_LOGDATA
namespace boost{namespace spirit{namespace x3{namespace traits{
    unsigned move_to_count=0;
    template<typename Source>
    void print_to_cout(Source& src)
    {  std::cout<<src;
    }
    template<typename Source>
    inline void move_to(Source& src, LogData& dest)
    {
        dest.f0=++move_to_count;
        std::cout
        <<__FILE__<<':'
        <<__LINE__<<':'
        <<__func__
        <<":move_to_count="<<move_to_count
        <<":demangle_fmt_type(src)="
        <<demangle_fmt_type(src)
        <<":src=";
        print_to_cout(src);
        std::cout<<":dest=LogData"
        <<";\n";
    }
}}}}
#endif//MOVE_TO_SRC_LOGDATA
