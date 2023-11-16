//OriginalSource:
//  http://coliru.stacked-crooked.com/a/b4de81a919bec94a
//LinkedToFrom:
//  https://stackoverflow.com/questions/74963861/parse-html-with-boost-spirit-x3
//Changes:
/*
  Simplification:
    Simplified parser expression of tag_name to just x3::int_.  
    
    The justification for this simplification is the essential purpose
    for this file is to show how to handle testing for same begin end
    tags.  That purpose is not advanced using much more complex parser
    expression in the original code:
    
      auto tag_name_                                                   
        = x3::rule<struct HtmlTagName_tag, std::string>{"HtmlTagName"}
        = x3::lexeme[*~x3::char_(" />")];
    
    Similar simplifications were made for other parser expressions.
    
    Also, some renames were made.  In particular Parser::inner_text
    was renamed to Parser::html_content_ to conform to the naming
    convention for Parser::tag_name_ and attribute Ast::tag_name.  The
    original code had no Ast::html_content, instead, it had, in
    element_base, std::string.  The following code is clearer in that
    it using a descriptive name, Ast::html_content instead of
    std::string, to name the attribute.
    
    In addition, the primitive parser definitions were moved above the
    'namespace Ast' to allow proper attributes, using
    x3::traits::attribute_of, to be calculated for the primitive
    Ast's.
    
  Comparison:
    An alternative method to using semantic actions is using attribute transforms
    done by as_attr.
    
    The alternative's are selected based on defined(USE_SEMANTIC_ACTIONS).
*/    
//===============
#include <iomanip>
#include <iostream>
#include <string>
#define ISSUE749_EXE_TRACE "issue749/coliru_sehe-debug-simple-compare.cpp"
#ifdef ISSUE749_EXE_TRACE
  #include <boost/iostreams/utility/templ_expr/demangle_fmt_type.hpp>
  #include <boost/utility/trace_scope.hpp>
  using boost::trace_scope;
#else
  #include "trace_scope_dummy.hpp"
#endif//ISSUE749_EXE_TRACE

//#define BOOST_SPIRIT_X3_DEBUG
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <iomanip>
#include <iostream>
//#define USE_SEMANTIC_ACTIONS
#ifdef USE_SEMANTIC_ACTIONS
  #include <stack>
#else
  #include <boost/spirit/home/x3/core/make_transform_parser.hpp>
  #include <boost/fusion/sequence/intrinsic/at_c.hpp>
#endif//USE_SEMANTIC_ACTIONS

namespace x3 = boost::spirit::x3;
using namespace std::string_literals;

namespace Parser 
//primitive parsers
{
    auto html_content_ =
      x3::alpha
      ;
    auto tag_name_ =
      x3::uint_
      ;
}//Parser
namespace Ast 
{
    struct html_tag;

    using html_content = 
      typename x3::traits::
      attribute_of
      < std::remove_const_t<decltype(Parser::html_content_)>
      , x3::unused_type
      >::type
      ;
    using html_element =
      x3::variant
      < html_content
      , boost::recursive_wrapper<html_tag>
      >;
    using html_elements =
      std::vector<html_element>
      ;
    using tag_name = 
      typename x3::traits::
      attribute_of
      < std::remove_const_t<decltype(Parser::tag_name_)>
      , x3::unused_type
      >::type
      ;
    struct html_tag 
      {
        tag_name    header;
        html_elements children;
      };
} // namespace Ast

BOOST_FUSION_ADAPT_STRUCT(Ast::html_tag, header, children)

namespace Parser 
{
    struct tag_block__tag;
    struct html_element__tag;
  #ifdef USE_SEMANTIC_ACTIONS
      struct
    tag_stack final
      {};
    //#define TAG_NAME_FUNC_TRACE
    auto tag_name_begin_func = [](auto& ctx) 
      { 
      #ifdef TAG_NAME_FUNC_TRACE
        trace_scope ts("tag_name_begin_func");
      #endif
        auto& tag_stk = get<tag_stack>(ctx);
        Ast::tag_name tag_beg=_attr(ctx);
      #ifdef TAG_NAME_FUNC_TRACE
        std::cout<<":tag_beg="<<tag_beg<<";\n";
      #endif
        tag_stk.push(tag_beg);
      };
    auto tag_name_end_func = [](auto& ctx) 
      {
      #ifdef TAG_NAME_FUNC_TRACE
        trace_scope ts("tag_name_end_func");
      #endif
        auto& tag_stk = get<tag_stack>(ctx);
        Ast::tag_name tag_beg = tag_stk.top();
        Ast::tag_name tag_end = _attr(ctx);
        auto tags_matched=tag_beg == tag_end;
      #ifdef TAG_NAME_FUNC_TRACE
        std::cout<<":tag_end="<<tag_end<<";\n";
        std::cout<<":tags_matched="<<tags_matched<<";\n";
      #endif
        _pass(ctx) = tags_matched;
        tag_stk.pop();
      };
  #endif//USE_SEMANTIC_ACTIONS
      auto 
    tag_header_
      = 
      (  '<' 
      >> tag_name_ 
  #ifdef USE_SEMANTIC_ACTIONS
         [ tag_name_begin_func
         ]
  #endif//USE_SEMANTIC_ACTIONS
      >> '>'
      )
      ;
      auto 
    tag_footer_
      = 
      (  "</"
  #ifdef USE_SEMANTIC_ACTIONS
      >> x3::omit
         [ tag_name_
           [ tag_name_end_func
           ]
         ] 
  #else
      >> tag_name_
  #endif//USE_SEMANTIC_ACTIONS
      >> '>'
      )
      ;
      x3::rule<html_element__tag, Ast::html_element> 
    html_element_ = "HtmlElement"
      ;
      auto
    tag_block__def =
         tag_header_
      >> *html_element_ 
      >> tag_footer_
      ;
  #ifdef USE_SEMANTIC_ACTIONS
      x3::rule<tag_block__tag, Ast::html_tag>
    tag_block_    = "TagBlock"
      ;
  #else
      auto
    tag_block_=
      x3::traits::as_attr
      < Ast::html_tag
      , x3::traits::transform_tag_id<tag_block__tag>
      >
      [ tag_block__def
      ]
      //transform attribute_of tag_block__def to an Ast::html_tag type.
      ;
  #endif//USE_SEMANTIC_ACTIONS
    auto html_element__def = 
        html_content_
      | tag_block_
      ;
    auto start = 
  #ifdef USE_SEMANTIC_ACTIONS
      x3::with<tag_stack>
      ( std::stack<Ast::tag_name>{}
      )
      [ html_element_
      ]
  #else
      html_element_
  #endif//USE_SEMANTIC_ACTIONS
      ;
    
  #ifdef USE_SEMANTIC_ACTIONS
    BOOST_SPIRIT_DEFINE(tag_block_, html_element_)
  #else
    BOOST_SPIRIT_DEFINE(html_element_)
  #endif//USE_SEMANTIC_ACTIONS
}
#ifndef USE_SEMANTIC_ACTIONS
namespace boost 
{ 
namespace spirit 
{ 
namespace x3 
{ 
namespace traits
{
    //#define TRANSFORM_ATTRIBUTE_TAGGED_TAG_BLOCK__TAG
      template 
      < typename ToFrom
        //attribute_of<decltype(Parser::tag_block__def),Context>::type
        //IOW, the attribute_of the parser passed as operator[] arg
        //to the as_attr call in Parser namespace.
      >
      struct 
    transform_attribute_tagged
      < Ast::html_tag
      , ToFrom
      , x3::traits::transform_tag_id<Parser::tag_block__tag>
      >
      {
        using FromTo=Ast::html_tag;
        using type=ToFrom;

          static type 
        pre
          ( FromTo&
          ) 
          { 
          #ifdef TRANSFORM_ATTRIBUTE_TAGGED_TAG_BLOCK__TAG
              boost::trace_scope 
            ts
              ( stringify
                ( ISSUE749_EXE_TRACE
                , ':',__LINE__
                , ':',__func__
                )
              );
            std::cout<<":type=\n"<<demangle_fmt_type<type>()<<";\n";
          #endif
            return ToFrom(); 
          }
          static bool 
        post
          ( FromTo& val
          , ToFrom const& attribute
          )
          {
          #ifdef TRANSFORM_ATTRIBUTE_TAGGED_TAG_BLOCK__TAG
              boost::trace_scope 
            ts
              ( stringify
                ( ISSUE749_EXE_TRACE
                , ':',__LINE__
                , ':',__func__
                )
              );
          #endif
            using boost::fusion::at_c;
            Ast::tag_name tag_beg=at_c<0>(attribute);//attribute_of tag_header_
            Ast::tag_name tag_end=at_c<2>(attribute);//attribute_of tag_footer_
            bool const matched=tag_beg == tag_end;
          #ifdef TRANSFORM_ATTRIBUTE_TAGGED_TAG_BLOCK__TAG
            std::cout
              <<":tag_beg="<<tag_beg<<"\n"
              <<":tag_end="<<tag_end<<";\n"
              ;
          #endif
            if(matched)
            {
                at_c<0>(val)=at_c<0>(attribute);
                at_c<1>(val)=at_c<1>(attribute);//attribute_of *html_element_
            }
            return matched;
          }
      };
}//traits
}//x3
}//spirit
}//boost
#endif//ndef USE_SEMANTIC_ACTIONS
#include "unit_tests.hpp"

int main() {
  #if 1 && defined(ISSUE749_EXE_TRACE)
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif
    std::cout<<std::boolalpha;
  #if 0
    { 
        trace_scope ts("tag_name_");
        unit_tests::test
        ( Parser::tag_name_
        , { R"(123)"
          , R"(456)"
          }
        );
    }
  #endif
  #if 0
    { 
        trace_scope ts("html_content_ pass");
        unit_tests::test
        ( Parser::html_content_
        , { R"(a)"
          , R"(b)"
          }
        );
    }
  #endif
  #if 0
    { 
        trace_scope ts("html_content_ fail");
        unit_tests::test<false>
        ( Parser::html_content_
        , { R"(1)"
          , R"(23)"
          }
        );
    }
  #endif
  #if 1
    {
        trace_scope ts("start pass");
        unit_tests::test
        ( Parser::start
        , { R"(a)"
          , R"(<0></0>)"
          , R"(<0>a</0>)"
          , R"(<0>a b c</0>)"
          , R"(<0>a b<1>c d</1> e</0>)"
          }
        );
    }
  #endif
  #if 1
    {
        trace_scope ts("start fail");
        unit_tests::test<false>
        ( Parser::start
        , { R"(<0></1>)"
          , R"(<0>a b c</1>)"
          }
        );
    }
  #endif                            
}
