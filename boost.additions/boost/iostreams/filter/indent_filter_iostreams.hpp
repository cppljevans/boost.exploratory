#ifndef BOOST_IOSTREAMS_FILTER_INDENT_FILTER_IOSTREAMS_HPP
#define BOOST_IOSTREAMS_FILTER_INDENT_FILTER_IOSTREAMS_HPP

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/operations.hpp>

// Must come last.
#include <boost/iostreams/detail/config/disable_warnings.hpp> // VC7.1 C4244.

namespace boost { namespace iostreams {

//
// Class name: indent_filter_iostreams.
//
// Template paramters:
//      Ch - The character type.
// Description: Output Filter which indents each line by user modifiable amount.
//
  template
  < typename Ch = BOOST_IOSTREAMS_DEFAULT_ARG(char)
  >
class indent_filter_iostreams
  : indent_filter_abstract
    < indent_filter_iostreams<Ch> 
    , Ch
    >
{
 public:
        typedef 
      indent_filter_abstract
      < indent_filter_iostreams<Ch> 
      , Ch
      >
    super_type;
    typedef Ch char_type;
    static int const width_default=2;
    
    template<typename Sink>
    bool put_concrete(Sink& dest, char_type c)
    {
        return boost::iostreams::put(dest, c);
    }
    
    template<typename Sink>
    bool put(Sink& dest, char_type c)
    {
        return super_type::put_abstract(dest,c);
    }
    
    void close()
    {
        this->reset();
    }
    
    explicit indent_filter_iostreams
      ( int width=width_default
      )
        : super_type(width)
    {
    }
};

} } // End namespaces iostreams, boost.

#include <boost/iostreams/detail/config/enable_warnings.hpp>

#endif // INDENT_FILTER_IOSTREAMS
