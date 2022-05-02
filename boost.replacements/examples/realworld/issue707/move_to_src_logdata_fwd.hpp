#pragma once
//#define MOVE_TO_SRC_LOGDATA
#ifdef MOVE_TO_SRC_LOGDATA
struct LogData;
namespace boost{namespace spirit{namespace x3{namespace traits{
    template<typename Source>
    inline void move_to(Source& src, LogData& dest)
    ;
}}}}
#endif//MOVE_TO_SRC_LOGDATA
