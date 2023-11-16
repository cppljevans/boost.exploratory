//Purpose:
//  Find out why 1.78breakage.cpp fails compilation without the #include demangle_fmt_type.hpp
//============
#define USE_TRACE_SCOPE
#ifdef USE_TRACE_SCOPE
#include <boost/utility/trace_scope.hpp>
#endif
#ifdef CPP_FMT
  #include <boost/iostreams/utility/templ_expr/fmt.hpp>
  #pragma message("yes CPP_FMT")
#else
  #pragma message("not CPP_FMT")
#endif
#if 1
  //partial contents of skip.hpp:
  #include <boost/spirit/home/x3/support/context.hpp>
  #include <boost/spirit/home/x3/support/unused.hpp>
  #include <boost/spirit/home/x3/core/skip_over.hpp>
  #include <boost/spirit/home/x3/core/parser.hpp>
  #include <boost/utility/enable_if.hpp>
namespace boost { namespace spirit { namespace x3
{
    template <typename Subject>
    struct reskip_directive : unary_parser<Subject, reskip_directive<Subject>>
    {
        typedef unary_parser<Subject, reskip_directive<Subject>> base_type;
        static bool const is_pass_through_unary = true;
        static bool const handles_container = Subject::handles_container;

        constexpr reskip_directive(Subject const& subject)
          : base_type(subject) {}

        template <typename Iterator, typename Context
          , typename RContext, typename Attribute>
        typename disable_if<has_skipper<Context>, bool>::type
        parse(Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attr) const
        {
            auto const& skipper =
                detail::get_unused_skipper(x3::get<skipper_tag>(context));

            return this->subject.parse(
                first, last
              , make_context<skipper_tag>(skipper, context)
              , rcontext
              , attr);
        }
        template <typename Iterator, typename Context
          , typename RContext, typename Attribute>
        typename enable_if<has_skipper<Context>, bool>::type
        parse(Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attr) const
        {
            return this->subject.parse(
                first, last
              , context
              , rcontext
              , attr);
        }
    };

    template <typename Subject, typename Skipper>
    struct skip_directive : unary_parser<Subject, skip_directive<Subject, Skipper>>
    {
        typedef unary_parser<Subject, skip_directive<Subject, Skipper>> base_type;
        static bool const is_pass_through_unary = true;
        static bool const handles_container = Subject::handles_container;

        constexpr skip_directive(Subject const& subject, Skipper const& skipper)
          : base_type(subject)
          , skipper(skipper)
        {}

        template <typename Iterator, typename Context
          , typename RContext, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attr) const
        {
            return this->subject.parse(
                first, last
              , make_context<skipper_tag>(skipper, context)
              , rcontext
              , attr);
        }

        Skipper const skipper;
    };

    struct reskip_gen
    {
        template <typename Skipper>
        struct skip_gen
        {
            constexpr skip_gen(Skipper const& skipper)
              : skipper_(skipper) {}

            template <typename Subject>
            constexpr skip_directive<typename extension::as_parser<Subject>::value_type, Skipper>
            operator[](Subject const& subject) const
            {
                return { as_parser(subject), skipper_ };
            }

            Skipper skipper_;
        };

        template <typename Skipper>
        constexpr skip_gen<Skipper> const operator()(Skipper const& skipper) const
        {
            return { skipper };
        }

        template <typename Subject>
        constexpr reskip_directive<typename extension::as_parser<Subject>::value_type>
        operator[](Subject const& subject) const
        {
            return { as_parser(subject) };
        }
    };

  #if 1
    constexpr auto skip = reskip_gen{};
  #endif  
}}}
#endif
#ifndef CPP_FMT
  #include <boost/iostreams/utility/templ_expr/fmt.hpp>
#endif
int main()
{
  #ifdef USE_TRACE_SCOPE
      boost::iostreams::indent_scoped_ostreambuf<char>
    indent_outbuf(std::cout,2);
  #endif
    return 0;
}  
