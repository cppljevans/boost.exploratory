# Purpose:

This folder provides, in:

<a id="rule_defn nonterminal"><a href="boost/spirit/home/x3/nonterminal">rule_defn/*/nonterminal</a></a>

replacements for the existing:

<a id="spirit nonterminal" href="https://github.com/boostorg/spirit/blob/master/include/boost/spirit/home/x3/nonterminal/rule.hpp">spirit/*/nonterminal</a>

# Rationale

There are reasons for the changes:

## [Separation of Concerns](https://en.wikipedia.org/wiki/Separation_of_concerns)

'rule.hpp` has at least the following 3 concerns:
<ol>
<li>recursion implementation</li>

Rules have
[rule ID's](https://github.com/boostorg/spirit/blob/master/include/boost/spirit/home/x3/nonterminal/rule.hpp#L42)'s
and definitions or Right Hand Side's
([RHS](https://github.com/boostorg/spirit/blob/master/include/boost/spirit/home/x3/nonterminal/rule.hpp#L43))
and those definitions can call any other Rules, enabling the rules to be recursive.

There are 2 methods for implementing recursion:
<ol>
<li>context entry  method</li>

This method stores the a map between a rule's ID and it's RHS in the
Context arg to the parsers.  This is of only limited use since lots of
rules would lead to lots of context entries, slowing compile times.

The utility of this method was further limited by the git pull, [x3-skip-rule-definition-injection](https://github.com/boostorg/spirit/pull/650).

This method is unchanged by <a href="#rule_defn nonterminal">rule_defn nonterminal</a>.

<li>ADL parse_rule method</li>

This is implemented by declaring each rule with an ID.  If that rule
is declared with `rule_name` as it's name, then, in the same
namespace, the user must define <a id="rule_name_def">`rule_name_def`</a> whose definition is the RHS for `rule_name`.

For example,
[employee rule](https://github.com/boostorg/spirit/blob/master/example/x3/employee.cpp#L64),
where `rule_name` is "employee", and
[employee def]()
where the `rule_name_def` is "employee_def".

The "connection" of the `rule_name` to it's `rule_name_def` is done by 1 of 2 methods:

<li> BOOST_SPIRIT_DEFINE macro</li>

Shown [here](https://github.com/boostorg/spirit/blob/master/include/boost/spirit/home/x3/nonterminal/rule.hpp#L228),
In case inline compilation desired.

<li>BOOST_SPIRIT_DECLARE macro</li>

shown [here](https://github.com/boostorg/spirit/blob/master/include/boost/spirit/home/x3/nonterminal/rule.hpp#L197)
In case separate compilation of rule definition is desired.

</ol>

Either of the above 2 macro call creates a specialization of the
[parse_rule
function](https://github.com/boostorg/spirit/blob/master/include/boost/spirit/home/x3/nonterminal/rule.hpp#L28)
by providing, as the 1st argument the `rule_id<ID>`, where ID is the
rule's ID.  The call to the rule's RHS is done [here](https://github.com/boostorg/spirit/blob/master/include/boost/spirit/home/x3/nonterminal/rule.hpp#L239) by using the
preprocessor macro, `BOOST_JOIN` to join the `rule_name` to the
suffix, `_def` to create the `rule_name_def` which should be the definition for `rule_name` <a href="#rule_name_def">as described above</a>.

In the [rule's parse function](https://github.com/boostorg/spirit/blob/master/include/boost/spirit/home/x3/nonterminal/rule.hpp#L155), the specialized parse_rule function is found by using
[ADL](https://en.cppreference.com/w/cpp/language/adl#:~:text=Argument%2Ddependent%20lookup%2C%20also%20known,function%20calls%20to%20overloaded%20operators.).

<li id="separate_compilation">Separate compilation of rule definition</li>
<li>Assurance that rule's attribute is rule<*>::attribute</li>
</ol>

