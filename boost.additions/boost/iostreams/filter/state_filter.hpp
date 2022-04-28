#ifndef BOOST_IOSTREAMS_FILTER_STATE_FILTER_HPP
#define BOOST_IOSTREAMS_FILTER_STATE_FILTER_HPP

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/operations.hpp>

// Must come last.
#include <boost/iostreams/detail/config/disable_warnings.hpp> // VC7.1 C4244.

namespace boost { namespace iostreams {

//
// Class name: state_filter.
//
// Template paramters:
//      Ch - The character type.
// Description: Output Filter which states each line by user modifiable amount.
//
  template
  < typename Ch = BOOST_IOSTREAMS_DEFAULT_ARG(char)
  >
class state_filter
    : public filter
      < output
      , Ch
      >
{
 public:

    typedef filter< output, Ch> super_type;
    typedef Ch char_type;
    static int const width_default=2;
    
    template<typename Sink>
    bool put(Sink& dest, char_type c)
    {
        if (c == '\n')
            linestart_ = true;
        else
            if (linestart_) {
                for(int n=0; n<state_; ++n)
                    boost::iostreams::put(dest, ' ');
                linestart_ = false;
            }
        return boost::iostreams::put(dest, c);
    }
    
    template<typename Sink>
    void close(Sink&)
    {
        state_ = 0;
        linestart_ = true;
    }
    
    void state_in()
    {
        state_by(+width_);
    }
    
    void state_out()
    {
        state_by(-width_);
    }
    
    int stateation()const
    {
        return state_;
    }
    
    int width()const
    {
        return width_;
    }
    
    void state_by(int amount)
    {
        int my_state=state_+amount;
        state_=my_state>0?my_state:0;
    }
    
    explicit state_filter
      ( int width=width_default
      )
        : state_(0)
        , linestart_(true)
        , width_((width>0)?width:0)
    {
    }
 private:    
    int  state_;//current stateation
    bool  linestart_;//at the start of line?
    int width_;//amount to change stateation.
};

} } // End namespaces iostreams, boost.

#include <boost/iostreams/detail/config/enable_warnings.hpp>

#endif // STATE
