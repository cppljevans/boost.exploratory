# Purpose:

This folder provides, in:

<a href="boost/spirit/home/x3/core/make_transform_parser.hpp">make_transform_parser.hpp</a>

three [variable templates](https://en.cppreference.com/w/cpp/language/variable_template):
<ol>
<li>`as_defn`</li>
<li>`as_attr`</li>
<li>`as_ident`</li>
</ol>
described in detail below.

<dl> <b>Variable Templates</b>
  <dt id="dt_as_defn">as_defn&lt;ToAttribute&gt;&#91;from_parser&#93;</dt>

  <dd>
    This emulates the `as` variable template <a href="http://coliru.stacked-crooked.com/a/6a357b07e0535f26">here</a>.<br><br>
    The method uses two submethods:
      <ol>
        <li>Use of <a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/nonterminal/detail/transform_attribute.hpp#L82">`transform_attribute&lt;Exposed,Transformed,x3::parser_id&gt;`</a></li>
	<br>
	This is used to transform an attribute, `Exposed e`, into a `Transformed t`
	in the `transform_attribute` `static pre` function, and then do the inverse in the `static post` function.
	<br><br>
	This is only useful when the `Attribute_` passed to either the `rule&lt;ID,Attribute&gt;` <b>OR</b> `rule_definition&lt;ID,RHS,Attribute,...>` parser is different than the `Attribute` in <a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/nonterminal/rule.hpp#L73">`rule<ID,Attribute>`</a>.<br><br>
	This is the reason that <b>BOTH</b>
	<ol>
	<li><a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/nonterminal/rule.hpp#L58">rule_definition::parse</a></li>
	<li><a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/nonterminal/rule.hpp#L143">rule::parse</a></li>
	</ol>
	have `Attribute_& attr` instead of `Attribute& attr` in their <a href="https://developer.mozilla.org/en-US/docs/Glossary/Signature/Function">signature</a>`s.
	<br><br>
	In short, use of `transform_attribute<Attribute_,attribute_type,...>::pre` function transfers
	information top down from the value of `Attribute_& attr` to the variable `attr_` <a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/nonterminal/rule.hpp#L153">here</a>. `attr_`'s type must be convertible to a reference to the `rule`'s <a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/nonterminal/rule.hpp#L80">`Attribute`</a> because it's used in the <a href="">`call to parse_rule`</a> where the corresponding argument type is <a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/nonterminal/rule.hpp#LL233C33-L233C69"></a> which resolves to the `rule`'s `Attribute`.  The "top down" adjective is appropriate because `attr`
	comes either from the "calling" rule or passed directly to the
	<a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/core/parse.hpp#L54">`parse`</a> or
	<a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/core/parse.hpp#L128">`phrase_parse`</a> function.
	<br>
	On the other hand, the `transform_attribute<...>::post` function transfers information bottom up.<br>
	
	An example of the need for this is <a href="../../boost.replacements/examples/realworld/codebrowser/qi.regression_transform_assignment/simpler.cpp">simpler.cpp</a>. A trace of a run of `simpler.cpp` is <a href="../../boost.replacements/examples/realworld/codebrowser/qi.regression_transform_assignment/simpler.-UUSE_RULE_DEFN.-DUSE_TRANSFORM_ATTRIBUTE.run_nl">simpler.-UUSE_RULE_DEFN.-DUSE_TRANSFORM_ATTRIBUTE.run_nl</a>.  Line `000091]:` shows that `transform_attribute:pre` "inherits" from the `start` rule, the `Attribute_` of type `foo_parts` which it then transforms to an `Attribute` of type `foo_refs`(defined on `simpler.cpp:30`).<br>
	<br>
	<li>Use of what's been called an `immediate rule` in <a href="https://github.com/boostorg/spirit/issues/714#issue-1110585799">`issue714`</a>.</li>
	<br>
	An `immediate rule` is one where there is no `rule` stored, but only a <a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/nonterminal/rule.hpp#L39">`rule_definition`</a>.  So what purpose does this serve when there is no specialization of `transform_attribute`? Well, in this `issue714`, <a href="https://en.wikipedia.org/wiki/Use_case">`use_case`</a>, it's the extra call to <a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/core/detail/parse_into_container.hpp#L88">`call_synthesize_x`</a>, and in particular, the creation of a container element, `val`, <a href="https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/core/detail/parse_into_container.hpp#L95">here</a>.
<br><br>
An example of this use is shown in <a href="../../boost.replacements/examples/realworld/issue714.immediate_rule/`work_around.cpp`">work_around.cpp</a>.
A debug version of that code is <a href="../../boost.replacements/examples/realworld/issue714.immediate_rule/`work_around-debug.cpp`">work_around-debug.cpp</a>.
The `work_around-debug.cpp` file, has various "implementations" to show various ways of not solving or solving the problem.
<br>
The `work_around-debug.cpp` implementation using `as_defn` emulates the original `work_around.cpp`.
A trace of this `as_defn` implementation is <a href="../../boost.replacements/examples/realworld/issue714.immediate_rule/work_around-debug.-UUSE_MOVE_TO_CHAR_VECTOR_CHAR.-DUSE_AS_DEFN.-UUSE_AS_ATTR.run_nl">here</a>.  The trace was produced by #including the code from the <a href="../../boost.replacements/debug/boost/">debug boost folder</a>, and this <a href="../../boost.replacements/examples/realworld/issue714.immediate_rule">`Makefile`</a>.
<br>
The trace shows that at the first `call_synthesize_x` call, `val` has type:
<pre>	
000150]:                   :value_type=
000151]:                   std::vector
000152]:                   < char
000153]:                   >;
</pre>
<br>
At the second call, `val` has type:
<pre>
000297]:                                           :value_type=
000298]:                                           char;
</pre>
      </ol>
      <br>Bottom line is that an extra parser was inserted in the grammar for the <b>SOLE</b> purpose of destructing, at 1 level, the 2 level Attribute:
<pre>
000033]:   :Attribute=
000034]:   std::vector
000035]:   < std::vector
000036]:     < char
000037]:     >
000038]:   >;
</pre>
      <br>Now, the fact that the `transform_attribute` is a <a href="https://en.wikipedia.org/wiki/NOP_(code)">nop</a> in this `use case` may seem unnotable; however, it
      does lead to `softward maintenance overhead` because any maintainer trying to understand the code, would still have to look at this code and reason that it actually
      was a `nop` before understanding the code.  That's unneeded.  The 
  </dd><br>

  <dt id="dt_as_attr">as_attr&lt;ToAttribute&gt;[from_parser]</dt>

  <dd>
    This transforms `from_parser` to a parser expression with attribute of type, `ToAttribute`
    with the help of `transform_attribute_tagged&lt;ToAttribute,FromAttribute&gt;`.
  </dd>
</dl>
  

