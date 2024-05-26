#pragma once
namespace boost 
{ namespace spirit 
{ namespace x3
{
    namespace detail
    {
        template<typename RuleId>
        struct undefined_parser
        //causes easily understood error message when no 
        //rule_defn(RuleId) for some specific RuleId.
        {
          //If you get a compile-time error message about a missing 
          //parser member function in undefined_parser<RuleId>, 
          //then you need to define rule_defn(RuleId)
          //which returns the definition for rule<RuleId,RuleAttribute>.
          //Alternately, you can use BOOST_SPIRIT_DEFINE(rule_name)
        };
    }
    // default rule_defn implementation
    template <typename RuleId>
    auto rule_defn(RuleId*)
      {
        return detail::undefined_parser<RuleId>{};
      }
}}}
