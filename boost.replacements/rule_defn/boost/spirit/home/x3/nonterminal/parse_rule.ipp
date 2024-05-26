//#define BOOST_SPIRIT_INSTANTIATE_NO_CHECK_DEF
#ifdef BOOST_SPIRIT_INSTANTIATE_NO_CHECK_DEF
  //This mimic's what's done in spirit in 1_80_0 download.
  //IOW, it doesn't check if there's a BOOST_SPIRIT_DEFINE for the rule.
  //As a result, at compile of the .cpp where this is located, there might
  //be some error like:
  //  'no member named 'has_action' in 'boost::spirit::x3::detail::undefined_parser'
#define BOOST_SPIRIT_INSTANTIATE(rule_type, Iterator, Context)                  \
      template<>                                                                \
      bool                                                                      \
    parse_rule                                                                  \
      < Iterator                                                                \
      , Context                                                                 \
      >                                                                         \
      ( typename rule_type::id*                                                 \
      , Iterator& first, Iterator const& last                                   \
      , Context const& context                                                  \
      , rule_type::attribute_type& attr                                         \
      )                                                                         \
      {                                                                         \
        auto defn_r=::boost::spirit::x3::rule_defn(rule_id);                    \
        return parse_rule_rhs<rule_type>                                        \
          ( first, last                                                         \
          , context                                                             \
          , attr                                                                \
          , defn_r                                                              \
          );                                                                    \
       }                                                                        \
    /***/
#else      
  //In this case, if there's no BOOST_SPIRIT_DEFINE for the rule,
  //the compile of the .cpp where this is located will be more informative:
  /*
static assertion failed due to requirement '!is_same<boost::spirit::x3::unused_type, boost::spirit::x3::unused_type>::value': BOOST_SPIRIT_DEFINE undefined for this rule.
                static_assert(!is_same<context_rule_t, unused_type>::value,
                ^             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
   */
#define BOOST_SPIRIT_INSTANTIATE(rule_type, Iterator, Context)                  \
      template<>                                                                \
      bool                                                                      \
    parse_rule                                                                  \
      < Iterator                                                                \
      , Context                                                                 \
      >                                                                         \
      ( typename rule_type::id*rule_id                                          \
      , Iterator& first, Iterator const& last                                   \
      , Context const& context                                                  \
      , rule_type::attribute_type& attr                                         \
      )                                                                         \
      {                                                                         \
        return                                                                  \
          parse_rule                                                            \
          < typename rule_type::id                                              \
          , Iterator                                                            \
          , Context                                                             \
          , rule_type::attribute_type                                           \
          >                                                                     \
          ( rule_id                                                             \
          , first, last                                                         \
          , context                                                             \
          , attr                                                                \
          );                                                                    \
       }                                                                        \
    /***/
#endif//BOOST_SPIRIT_INSTANTIATE_NO_CHECK_DEF

