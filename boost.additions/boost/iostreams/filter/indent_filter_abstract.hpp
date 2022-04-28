#ifndef BOOST_IOSTREAMS_FILTER_INDENT_FILTER_ABSTRACT_HPP
#define BOOST_IOSTREAMS_FILTER_INDENT_FILTER_ABSTRACT_HPP

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/operations.hpp>

// Must come last.
#include <boost/iostreams/detail/config/disable_warnings.hpp> // VC7.1 C4244.

namespace boost { namespace iostreams {

//
// Class name: indent_filter_abstract.
//
// Template paramters:
//      Ch - The character type.
// Description: Output Filter which states indents line by user modifiable amount.
//
  template
  < typename Concrete
  , typename Ch = BOOST_IOSTREAMS_DEFAULT_ARG(char)
  >
class indent_filter_abstract
{
 public:

    typedef Ch char_type;
    static int const width_default=2;
    
    template<typename Sink>
    bool put_abstract(Sink& dest, char_type c)
    {
        Concrete*self_concrete=static_cast<Concrete*>(this);
        if (c == '\n')
            linestart_ = true;
        else
            if (linestart_) {
                for(int n=0; n<indent_; ++n)
                    self_concrete->put_concrete(dest, ' ');
                linestart_ = false;
            }
        return self_concrete->put_concrete(dest, c);
    }
    
    void reset()
    {
        indent_ = 0;
        linestart_ = true;
    }
    
    void indent_in()
    {
        indent_by(+width_);
    }
    
    void indent_out()
    {
        indent_by(-width_);
    }
    
    int indentation()const
    {
        return indent_;
    }
    
    int width()const
    {
        return width_;
    }
    
    void indent_by(int amount)
    {
        int my_indent=indent_+amount;
        indent_=my_indent>0?my_indent:0;
    }
    
    explicit indent_filter_abstract
      ( int width=width_default
      )
        : indent_(0)
        , linestart_(true)
        , width_((width>0)?width:0)
    {
    }
 private:    
    int  indent_;//current indentation
    bool  linestart_;//at the start of line?
    int width_;//amount to change indentation.
};

} } // End namespaces iostreams, boost.

#include <boost/iostreams/detail/config/enable_warnings.hpp>

#endif // INDENT_FILTER_ABSTRACT
