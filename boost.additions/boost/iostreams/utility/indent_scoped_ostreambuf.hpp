#ifndef BOOST_IOSTREAMS_INDENT_SCOPED_OSTREAMBUF_HPP_INCLUDED
#define BOOST_IOSTREAMS_INDENT_SCOPED_OSTREAMBUF_HPP_INCLUDED

#include <boost/iostreams/filter/indent.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace boost { namespace iostreams {

//
// Template name: indent_scoped_ostreambuf.
// Description: Temporarily replaces the ostreambuf of ostream 
//   passed to CTOR. DTOR restores original ostreambuf.
// Template paramters:
//      Ch - The charactertype.
//      Tr - The character traits type.
//
template<typename Ch=char, typename Tr=std::char_traits<Ch> >
class indent_scoped_ostreambuf 
{
 public:
    typedef std::basic_streambuf<Ch,Tr> std_buf_type;
    typedef std::basic_ostream<Ch,Tr> std_ostrm_type;
    typedef filtering_streambuf<output,Ch,Tr> filt_buf_type;
    
    indent_scoped_ostreambuf
    ( std_ostrm_type& a_ostrm
    , int width=indent_filter<Ch>::width_default
    )
      : my_old_buf(a_ostrm.rdbuf())
      , my_strm(a_ostrm)
      , my_filt_buf(*my_old_buf,width)
    {
        my_strm.rdbuf(&my_filt_buf);
    }
    
    ~indent_scoped_ostreambuf(void)
    {
        my_strm.rdbuf(my_old_buf);
    }
    
    int pubsync()
    {
        return my_old_buf->pubsync();
    }

    // Class name: push_filt_strmbuf
    // Description: streambuf allowing user to indent
    //   the stream (by calling filt_get and then
    //   calling filt_type::indent_in or filt_type::indent_out).
    struct push_filt_strmbuf
    : public filt_buf_type
    {
      typedef indent_filter<Ch> filt_type;
      
      push_filt_strmbuf
      ( std_buf_type& a_buf
      , int width=indent_filter<Ch>::width_default
      )
      {
          filt_type my_filter(width);
          this->push(my_filter,0,0);
          this->push(a_buf);
      }
      
      filt_type*filt_get(void)
      {
          filt_type*const my_filter = this->template component<filt_type>(0);
          return my_filter;
      }
       
    };
    
 private:
    std_buf_type* my_old_buf;//For DTOR.
    
    std_ostrm_type& my_strm;//For DTOR.
    
    push_filt_strmbuf my_filt_buf;//The temporary replacement buffer.
};

} } // End namespaces iostreams, boost.

template<class Ch, class Tr>
inline typename boost::iostreams::indent_scoped_ostreambuf<Ch,Tr>::push_filt_strmbuf*
indent_filt_buf(std::basic_ostream<Ch, Tr>const& os)
{
    typedef boost::iostreams::indent_scoped_ostreambuf<Ch,Tr> filt_scoped_type;
    typedef typename filt_scoped_type::push_filt_strmbuf filt_strmbuf_type;
    filt_strmbuf_type*buf_ptr = dynamic_cast<filt_strmbuf_type*>(os.rdbuf());
    return buf_ptr;
}

template<class Ch, class Tr>
inline typename boost::iostreams::indent_filter<Ch>*
indent_filter(std::basic_ostream<Ch, Tr>const& os)
{
    auto*buf_ptr = indent_filt_buf(os);
    if(buf_ptr)
      return buf_ptr->filt_get();
    else
      return 0;
}

template<class Ch, class Tr>
inline int
indent_width(std::basic_ostream<Ch, Tr>const& os)
{
    auto const*filt_ptr = indent_filter(os);
    if(filt_ptr)
      return filt_ptr->width();
    else
      return -1;
}

// Function name: indent_buf_in
// Descrption:: Indents the buffer of ostream argument, if possible.
//
template<class Ch, class Tr>
inline std::basic_ostream<Ch, Tr>&
indent_buf_in(std::basic_ostream<Ch, Tr>& os)
{
    auto*const buf_ptr=indent_filt_buf(os);
    if (buf_ptr) {
        auto* filt_ptr=buf_ptr->filt_get();
        filt_ptr->indent_in(); 
    }
    return os;
}

// Function name: indent_buf_out
// Descrption:: Indents outwardly the buffer of ostream argument, if possible.
//
template<class Ch, class Tr>
inline std::basic_ostream<Ch, Tr>&
indent_buf_out(std::basic_ostream<Ch, Tr>& os)
{
    auto*const buf_ptr=indent_filt_buf(os);
    if (buf_ptr) {
        auto* filt_ptr=buf_ptr->filt_get();
        filt_ptr->indent_out(); 
    }
    return os;
}

template<class Ch, class Tr>
inline std::basic_ostream<Ch, Tr>&
operator++(std::basic_ostream<Ch, Tr>& os)
{
    return indent_buf_in(os);
}    
template<class Ch, class Tr>
inline std::basic_ostream<Ch, Tr>&
operator--(std::basic_ostream<Ch, Tr>& os)
{
    return indent_buf_out(os);
}    

#endif // #ifndef BOOST_IOSTREAMS_INDENT_SCOPED_OSTREAMBUF_HPP_INCLUDED
