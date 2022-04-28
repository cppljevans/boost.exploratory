//ostream with indenting and specializations for attaching filter.
#ifndef BOOST_IOSTREAMS_FILTER_INDENT_FIND_FILT_OSTREAM_HPP_INCLUDED
#define BOOST_IOSTREAMS_FILTER_INDENT_FIND_FILT_OSTREAM_HPP_INCLUDED

#include <boost/iostreams/filter/indent.hpp>
#include <boost/iostreams/filter/ios_register.hpp>
#include <boost/iostreams/filtering_stream.hpp>

namespace boost 
{ 
namespace iostreams 
{

namespace indent_find_filt_tags
/**@brief
 *  Various tags for selecting method for
 *  attaching the filter to an indent_find_filt_ostream.
 */
{
    struct ios_register
    {};
    struct component
    {};
    struct ref
    {};
    
}//exit indent_find_filt_tags

namespace indent_find_filt_detail
{
  template
  < class CharT
  >
struct general
: public filtering_stream<output,CharT>
{
        typedef
      filtering_stream<output,CharT>
    super_type
    ;
        typedef 
      indent_filter<CharT>
    filt_type
    ;
    general(void)
    {}
    general(general const& self)
    : super_type(self)
    {}
        typedef
      void(filt_type::*
    indent_type
      )()
    ;
    
};
  template
  < class ImplT //A tag for selecting the implementation.
  , class CharT
  >
struct special
;
  template
  < class CharT
  >
struct special
  < indent_find_filt_tags::ios_register
  , CharT
  >
: public general<CharT>
{
        typedef typename
      general<CharT>::filt_type
    filt_type
    ;
        typedef
      ios_register
      < filt_type
      >
    reg_type
    ;
    special(void)
    {
        filt_type filt_valu(2);
        reg_type::push
          ( *this
          , filt_valu
          );
    }
        static
      filt_type*
    find_filt
      ( ::std::basic_ostream<CharT>& os
      )
    {
        return reg_type::find_filt(os);
    }
};

  template
  < class CharT
  >
struct special
  < indent_find_filt_tags::component
  , CharT
  >
: public general<CharT>
{
        typedef
      general<CharT>
    super_type
    ;
        typedef typename
      general<CharT>::filt_type
    filt_type
    ;
    special(void)
    {
        filt_type filt_valu(2);
        this->push
          ( filt_valu
          , 0,0
          );
    }
    special(special const& self)
    : super_type(self)
    {}
        static
      filt_type*
    find_filt
      ( special& os
      )
    {
        return os.template component<filt_type>(0);
    }
        static
      filt_type*
    find_filt
      ( ::std::basic_ostream<CharT>& os
      )
    {
        filt_type*filt_valu=0;
        special*filt_strm_valu=dynamic_cast<special*>(&os);
        if(filt_strm_valu)
        {
            filt_valu=find_filt(*filt_strm_valu);
        }
        return filt_valu;
    }
};

  template
  < class CharT
  >
struct special
  < indent_find_filt_tags::ref
  , CharT
  >
: public general<CharT>
{
        typedef typename
      general<CharT>::filt_type
    filt_type
    ;
    special(void)
    : filt_valu(2)
    {
        this->push
          ( ref(filt_valu)
            //by pushing a ref to member variable,
            //no need to call (presumably expensive)
            //component<filt_type>() (as done in the
            //special<*::component> specialization)
            //to retrieve the indent_filter.
          , 0,0
          );
    }
        static
      filt_type*
    find_filt
      ( special& os
      )
    {
        return &(os.filt_valu);
    }
        static
      filt_type*
    find_filt
      ( ::std::basic_ostream<CharT>& os
      )
    {
        filt_type*filt_valu=0;
        special*filt_strm_valu=dynamic_cast<special*>(&os);
        if(filt_strm_valu)
        {
            filt_valu=find_filt(*filt_strm_valu);
        }
        return filt_valu;
    }
 private:
      filt_type
    filt_valu
    ;    
};

}//exit indent_find_filt_detail

  template
  < class ImplT=indent_find_filt_tags::ref
  , class CharT=char
  >
struct indent_find_filt_part
: public indent_find_filt_detail::special
  < ImplT
  , CharT
  >
{
        typedef
      indent_find_filt_detail::special
      < ImplT
      , CharT
      >
    super_type
    ;
        typedef typename
      super_type::filt_type
    filt_type
    ;
        typedef typename
      super_type::indent_type
    indent_type
    ;
        static
      void
    indent
      ( ::std::basic_ostream<CharT>& os
      , indent_type indent_valu
      )
    {
        filt_type*filt_valu=find_filt(os);
        if(filt_valu)
        {
            (filt_valu->*indent_valu)();
        }
    }

};

  template
  < class ImplT=indent_find_filt_tags::ref
  , class CharT=char
  >
struct indent_find_filt_ostream
: public indent_find_filt_part
  < ImplT
  , CharT
  >
{
    indent_find_filt_ostream(std::ostream& a_ostr)
    {
        this->push(a_ostr,0,0);
    }
    
};

}//exit iostreams namespace
}//exit boost namespace


#endif
