#pragma once
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_primitives.hpp>
namespace xpr = boost::xpressive;
using namespace xpr;
#include <iostream>
#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
struct output_nested_results_t
/**<@brief
 *    Display a tree of nested results,
 *  
 *    This code is modified from 'struct output_nested_results' here:
 *      libs/xpressive/example/main.cpp
 *    or published on the web here:
 *      https://www.boost.org/doc/libs/1_79_0_beta1/doc/html/xpressive/user_s_guide.html#boost_xpressive.user_s_guide.examples.display_a_tree_of_nested_results
 */
{
    template< typename BidiIterT >
    void operator ()( match_results< BidiIterT > const &what )const
    {
        // output the match
        std::cout <<":size="<<what.size()<<":position="<<what.position()<<":prefix="<<what.prefix()<<";\n";
        for(unsigned i=0; i<what.size(); ++i) std::cout<<"what["<<i<<"]=\n"<< what[i] << ";\n";
        // output any nested matches
        using match_results_t=match_results< BidiIterT >;
        //std::cout<<"nested results\n{\n";
        std::cout<<indent_buf_in;
        typename match_results_t::nested_results_type const&nst_res=what.nested_results();
          output_nested_results_t const 
        output_nested_results_v;
        std::for_each
        ( nst_res.begin()
        , nst_res.end()
        , output_nested_results_v
        );
        std::cout<<indent_buf_out;
    }
};//output_nested_results_t struct
output_nested_results_t output_nested_results_v;

