#ifndef BOOST_IOSTREAMS_UTILITY_INDENT_OSTREAM_HPP_INCLUDED
#define BOOST_IOSTREAMS_UTILITY_INDENT_OSTREAM_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/iostreams/filter/indent.hpp>
#include <boost/iostreams/filtering_stream.hpp>

namespace boost { namespace iostreams {

template<typename Ch=char, typename Tr=std::char_traits<Ch> >
class indent_ostream 
    : public filtering_stream<output,Ch,Tr>
{
 public:
    typedef indent_filter<Ch> filt_type;
    typedef std::basic_ostream<Ch,Tr> sink_type;
    sink_type& sink;
    
    filt_type& get_filt(void)const
        {
            filt_type*my_comp=this->template component<0,filt_type>();
            return *my_comp;
        }
    sink_type& get_sink(void)const
        {
            return sink;
        }
    indent_ostream
        ( sink_type& a_ostrm
        , int width=filt_type::width_default
        )
        : sink(a_ostrm)
        {
            filt_type filt_(width);
            this->push
              ( filt_
              , 0, 0 //make unbuffered (to avoid need for flush).
              );
            this->push
              ( a_ostrm
              , 0, 0 //unbuffered
              );
        }
    void indent_in(void)
        {
            get_filt().indent_in();
        }
    void indent_out(void)
        {
            get_filt().indent_out();
        }
    void operator++(void)
        {
            indent_in();
        }
    void operator--(void)
        {
            indent_out();
        }
        
};

} } // End namespaces iostreams, boost.

template<class charT>
inline std::basic_ostream<charT>&
indent_in(std::basic_ostream<charT>& os)
{
    typedef boost::iostreams::indent_ostream<charT> filt_os_type;
    filt_os_type*p = dynamic_cast<filt_os_type*>(&os);
    if (p) { 
        p->indent_in(); 
    }
    return os;
}

template<class charT>
inline std::basic_ostream<charT>&
indent_out(std::basic_ostream<charT>& os)
{
    typedef boost::iostreams::indent_ostream<charT> filt_os_type;
    filt_os_type*p = dynamic_cast<filt_os_type*>(&os);
    if (p) { 
        p->indent_out(); 
    }
    return os;
}

template<class charT>
inline std::basic_ostream<charT>&
get_sink(std::basic_ostream<charT>& os)
{
    typedef boost::iostreams::indent_ostream<charT> filt_os_type;
    filt_os_type*p = dynamic_cast<filt_os_type*>(&os);
    if (p) { 
        return p->get_sink(); 
    }
    return os;
}

#endif // #ifndef BOOST_IOSTREAMS_INDENT_OSTREAM_HPP_INCLUDED
