#ifndef BOOST_UTILITY_TRACE_SCOPE_HPP
#define BOOST_UTILITY_TRACE_SCOPE_HPP
#include <boost/utility/indent_scope.hpp>
namespace boost
{
  class
trace_scope
  //Purpose:
  //  Indicate on a std::ostream when a scope entered and exited.
  : indent_scope<>
  {
  public:
      template
      < typename ScopeName
      >
    trace_scope
      ( ScopeName a_scopeName
      , bool doit=true
      , std::ostream& a_strm=std::cout
      )
      : indent_scope<>
        ( doit
        , std::string("===>Enter:{{")+a_scopeName+"\n"
        , std::string("===>Exit_:}}")+a_scopeName+"\n"
        , a_strm
        )
      {
      }
  };//end trace_scope class
}//exit boost namespace
#include <boost/utility/stringify.hpp>
#endif
