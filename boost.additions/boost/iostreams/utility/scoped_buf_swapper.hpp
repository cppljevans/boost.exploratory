#ifndef BOOST_IOSTREAMS_SCOPED_BUF_SWAPPER_HPP_INCLUDED
#define BOOST_IOSTREAMS_SCOPED_BUF_SWAPPER_HPP_INCLUDED

#include <boost/iostreams/filtering_stream.hpp>

//#define SCOPED_BUF_SWAPPER_DEBUG
#ifdef  SCOPED_BUF_SWAPPER_DEBUG
#include <iostream>
#include <boost/utility/demangled_type_name.hpp>
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace boost { namespace iostreams {

//
// Template name: scoped_buf_swapper.
// Description: Temporarily replaces the streambuf of an iostream 
//   passed to CTOR. DTOR restores original streambuf.
// Methods:
//   push<Filter>(Filter& a_filter)
//     inserts a_filter just before the streambuf of the iostream 
//     passed to CTOR on the boost::iostreams Chain that's
//     part of the member variable, my_filt_buf.
//   
// Template paramters:
//      Mode - the i/o mode.
//      Ch - The charactertype.
//      Tr - The character traits type
//
  template
  < typename Mode
  , typename Ch
  , typename Tr = std::char_traits < Ch >
  >
class scoped_buf_swapper 
{
 public:
        typedef 
      Mode
    mode_type;
        typedef 
      Ch
    char_type;
        typedef 
      Tr
    traits_type;
        typedef 
      typename detail::filtering_stream_traits
      < mode_type
      , char_type
      , traits_type
      >::stream_type 
    std_strm_type;
        typedef 
      std::basic_streambuf<char_type,traits_type> 
    std_buf_type;
        typedef 
      filtering_streambuf<mode_type,char_type,traits_type> 
    filt_buf_type;
        typedef
      detail::mode_adapter
      < mode_type
      , std::basic_streambuf
        < char_type
        , traits_type
        > 
      >
    adapt_type;
    
    
    // Class name: push_filt_strmbuf
    // Description: filtering_streambuf with
    //   method for finding pushed filter type.
    //   
    struct push_filt_strmbuf
    : public filt_buf_type
    {
#ifdef SCOPED_BUF_SWAPPER_DEBUG
          void
        print_comp_types
          ( void
          )
        {
            std::cout<<"push_filt_strmbuf::component_types=\n";
            int const nfilt=this->size();
            for
              ( int ifilt=0
              ; ifilt<nfilt
              ; ++ifilt
              )
            {
                std::cout<<"  type("<<ifilt<<")="
                  <<utility::compiler::demangled_typeinfo_name(this->component_type(ifilt))
                  <<"\n";
            }
        }
#endif
    
          template
          < typename PushFilt
          >
        push_filt_strmbuf
          ( std_buf_type& a_buf
          , PushFilt const& a_filter
          )
        {
            this->push(a_filter,0,0);
            this->push(a_buf);
          #ifdef SCOPED_BUF_SWAPPER_DEBUG
            std::cout<<"push_filt_strmbuf::CTOR.\n";
            print_comp_types();
          #endif
        }
        
        //
        // Method name: get_std_buf_ptr.
        // Description: 
        //   If this->is_complete
        //     Returns std_buf_type* of the last component.
        //     This should be the address of the a_buf
        //     arg to the push_filt_strmbuf CTOR.
        //   Else
        //     Returns 0.
        //
          std_buf_type*
        get_std_buf_ptr(void)
        {
            std_buf_type*my_std_buf_ptr=0;
            if(this->is_complete())
            {
                int const ifilt=this->size()-1;
                adapt_type*adapt_valu=this->template component<adapt_type>(ifilt);
                if(adapt_valu)
                {
                    std_buf_type&my_std_buf_ref=adapt_valu->adapted_component();
                    my_std_buf_ptr=&my_std_buf_ref;
                  #if 0
                    {
                        std::cout<<":ifilt="<<ifilt<<":adapt_valu="<<(void*)adapt_valu<<"\n";
                        std::cout<<"get_std_buf_ptr::type_name(this->ref())=\n";
                        std::string dtn(utility::demangled_type_name(this->ref()));
                        std::cout<<dtn<<"\n";
                        std::cout<<"list=\n";
                        typedef typename filt_buf_type::chain_type::list_type list_type;
                        list_type const&my_list=this->ref().list();
                        std::cout<<utility::demangled_type_name(my_list)<<"\n";
                        typedef typename list_type::const_iterator iter_type;
                        unsigned count=0;
                        for
                          ( iter_type cur=my_list.begin()
                          ; cur != my_list.end()
                          ; ( ++cur
                            , ++count
                            )
                          )
                        {
                            std::cout<<"count="<<count<<"\n";
                            std::cout<<"  "<<utility::demangled_type_name(*cur)<<"\n";
                            //std::cout<<"  component="<<(*cur)->component_impl()<<"\n";
                        }
                    }
                  #endif
                }
            }
            return my_std_buf_ptr;
        }
        
        //
        // Method template name: find_filt_filtbuf.
        // Description: 
        //   Finds filter pointer of given template argument type
        //   if it exists; otherwise, returns 0.
        // Template paramters:
        //      PushFilt - the filter type fo find.
        //
          template
          < typename PushFilt
          >
          PushFilt*
        find_filt_filtbuf(void)
        {
            push_filt_strmbuf* a_buf=this;
            PushFilt*my_filt=0;
            int nfilt=a_buf->size();
            if(a_buf->is_complete())
            {
                --nfilt;//last component not filter.
            }
            for
              ( int ifilt=0
              ; ifilt<nfilt
              ; ++ifilt
              )
            {
                my_filt=a_buf->template component<PushFilt>(ifilt);
                if(my_filt)break;
            }
            return my_filt;
        }
        
        //
        // Static Method template name: find_filt_stdbuf
        // Description: 
        //   Finds filter pointer of given template argument type
        //   within the std_buf_type* argument.
        // Template paramters:
        //      PushFilt - the filter type fo find.
        // Parameters:
        //   std_buf_type* a_buf
        //     The buffer maybe containing the given filter type.
        //
          template
          < typename PushFilt
          >
            static
          PushFilt*
        find_filt_stdbuf(std_buf_type* a_buf)
        {
            PushFilt*my_filt=0;
            push_filt_strmbuf*my_push_buf=dynamic_cast<push_filt_strmbuf*>(a_buf);
            while(my_push_buf)
            {
                my_filt=my_push_buf->find_filt_filtbuf<PushFilt>();
                if(my_filt)break;
                std_buf_type*my_std_buf_ptr=my_push_buf->get_std_buf_ptr();
                my_push_buf=dynamic_cast<push_filt_strmbuf*>(my_std_buf_ptr);
            }
            return my_filt;
        }
        
    };

    //
    // Method name: push
    // Description: 
    //   Inserts given PushFilt between the Device (returned by get_std_buf_ptr)
    //   and the other filters on my_filt_buf's Chain (part of filtering_streambuf).
    //
      template
      < typename PushFilt
      >
      void
    push
      ( PushFilt const& a_filt
      )
    {
        flush(my_filt_buf);
        std_buf_type*my_old_buf=my_filt_buf.get_std_buf_ptr();//save stack top (the device).
        bool before_auto_close=my_filt_buf.auto_close();
        my_filt_buf.set_auto_close(false);
        my_filt_buf.pop();//pop stack top
        my_filt_buf.push(a_filt,0,0);//push new filter onto stack top.
        my_filt_buf.push(*my_old_buf,0,0);//push saved device onto stack top 
#ifdef  SCOPED_BUF_SWAPPER_DEBUG
        std::cout<<"scoped_buf_swapper::push(after):my_filt_buf.size="<<my_filt_buf.size()<<"\n";
        my_filt_buf.print_comp_types();
#endif
        my_filt_buf.set_auto_close(before_auto_close);
    }
    
      template
      < typename PushFilt
      >
    scoped_buf_swapper
    ( std_strm_type& a_strm
    , PushFilt const& a_filter
    )
      : my_strm(a_strm)
      , my_filt_buf(*(a_strm.rdbuf()),a_filter)
    {
        my_strm.rdbuf(&my_filt_buf);
    }
    
    ~scoped_buf_swapper(void)
    {
        std_buf_type*my_old_buf=my_filt_buf.get_std_buf_ptr();
        my_filt_buf.pop();
        my_strm.rdbuf(my_old_buf);
    }
    
 private:
    std_strm_type& my_strm;
    
    push_filt_strmbuf my_filt_buf;
};

} } // End namespaces iostreams, boost.

#endif // #ifndef BOOST_IOSTREAMS_SCOPED_BUF_SWAPPER_HPP_INCLUDED
