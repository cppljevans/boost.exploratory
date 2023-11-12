#pragma once
//#define USE_FMT_XPRESSIVE
#ifdef USE_FMT_XPRESSIVE
  //this causes, on some inputs, apparently, an infinite loop, cuz it never stops.
  #include "../../../../examples/utility/templ_expr/xpressive/fmt.hpp"
#else
  #include "../../../../examples/utility/templ_expr/lexertl/fmt.hpp"
#endif
