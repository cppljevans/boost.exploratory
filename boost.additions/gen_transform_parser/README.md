<html>
<h1> Purpose:</h1>

This folder provides, in:

<a href="boost/spirit/home/x3/core/gen_transform_parser.hpp">gen_transform_parser.hpp</a>

three [variable templates](https://en.cppreference.com/w/cpp/language/variable_template):
<ol>
<li>`as_defn`</li>
<li>`as_attr`</li>
<li>`as_ident`</li>
</ol>
described in detail below.<br><br>

<dl> <b>Variable Templates:</b>
  <dt id="dt_as_defn">as_defn&lt;ToAttribute&gt;&#91;from_parser&#93;</dt>
  <dd>
    This emulates the `as` variable template <a href="http://coliru.stacked-crooked.com/a/6a357b07e0535f26">here</a>.
    It creates what's been called an `intermediate rule` in <a href="https://github.com/boostorg/spirit/issues/714#issue-1110585799">issue714</a>.
    Another name is `anonymous rule` coined in
    <a href="https://stackoverflow.com/questions/56819120/spirit-x3-how-to-get-attribute-type-to-match-rule-type?rq=1">how-to-get-attribute-type-to-match-rule-type</a>.
  </dd>
  <dt id="dt_as_attr">as_attr&lt;ToAttribute&gt;[from_parser]</dt>
  <dd>
    This transforms `from_parser` to a parser expression with attribute of type, `ToAttribute`
    with the help of `transform_attribute_tagged&lt;ToAttribute,FromAttribute&gt;`.
  </dd>
</dl>

</html>
