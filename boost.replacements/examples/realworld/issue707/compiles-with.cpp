//OriginalSource:
//  copy&pasted from 2nd source code listing on:
/*
https://github.com/boostorg/spirit/issues/707
 */
//  which started after the comment:
/*
It worked with boost 1.77. With 1.78 the compiler outputs almost 500 lines of incomprehensible messages. The gist of it is, boost::spirit::x3::detail::parse_into_container_impl fails. It compiles with the following changes:
 */
//===================
    auto dataSet = LogData{};
    if (x3::phrase_parse(
        s.begin(), s.end(),
        (
            x3::int_ >> x3::int_ >> x3::uint64
         >> (
                (
                    x3::double_ >> x3::uint64 >> x3::uint64
                 >> '{' >> (x3::int_ % ',') >> '}'
                )
              | (
                    x3::uint64 >> x3::uint64
                 >> '{' >> (x3::int_ % ',') >> '}'
                )
            )
        )
        x3::blank,
        dataSet
    ))
