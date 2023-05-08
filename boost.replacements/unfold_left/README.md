# Purpose:

In the 1.78 version of spirit x3, the expression:
```

  x >> y >> z
  
```  
has type:

```
  sequence
  < decltype(x)
  , sequence
    < decltype(y)
    , decltype(z)
    >
  >
  
```
and the expression:
```

  x | y | z

```
has type:
```

  alternative
  < decltype(x)
  , alternative
    < decltype(y)
    , decltype(z)
    >
  >
  
```
but, if these were "unfolded", the types would be simplified to:
```

  sequence
  < decltype(x)
  , decltype(y)
  , decltype(z)
  >

```
and
```

  alternative
  < decltype(x)
  , decltype(y)
  , decltype(z)
  >
  
```

Advocacy of this unfolding has been expressed
[here](https://github.com/boostorg/spirit/issues/465#issuecomment-464757190),
where the term `flatten` was used; however, there was some worry about
increased compile times 
[here](https://github.com/boostorg/spirit/issues/465#issuecomment-464775542).

# Why "unfold"?

The term "unfold" is appropriate because the term "fold" or "reduce"
is used to describe what the [apl reduce expression](https://aplwiki.com/wiki/Reduce) :

```

  +/[1,2,3]
  
```
does by applying binary operator, +, in between the vector of values,
`[1,2,3]`.  In effect what it does is:
```

  1+2+3

```
In c++, a [similar operation](https://en.cppreference.com/w/cpp/language/fold)
can be used as follows:
```

  template<int... Values>
  auto constexpr plus_fold()
  { return (... + Values);}
  
```

What `unfold` does is the opposite.  It converts:

```
  1+2+3
```

back into the simpler expression:

```
  +/[1,2,3]
```

Now, replace `+/` with `alternative` or `sequence` and `[1,2,3]` with
`<x,y,z>` and you see the correspondence.

# alternative fold/unfold:

Similar to the fold operation for `Values...` shown in the `plus_fold`
template function above, there's a similar operation done in
`operator/alternative.hpp` in the `parse_fold` function:

```

  bool result = 
    ( false
    || ...
    || this->template sub_parser<Indices>().parse
       ( first
       , last
       , context
       , rcontext
       , attr
       )
    );
    
```
however, that was only possible because the alternative's operands
where first `unfolded` later in operator/alternative.hpp:
```

  template <typename Left, typename Right>
  constexpr auto
  operator|(Left const& left, Right const& right)
  {
      return n_ary_make_parser<alternative>( as_parser(left), as_parser(right) );
  }
  
```

where `n_ary_make_parser` does the unfold in `core/parser.hpp`.  More specifically, it's done by:

```

  n_ary_sub_parsers<Derived>( left, right);

```

in same file.

# sequence fold/unfold:

Similar code is used for sequence, in `operator/sequence.hpp`
**WHEN** `!defined(USE_COLLAPSE_LEFT)`:

```
                bool result = 
                  ( true
                  && ...
                  && this->template sub_parser<Indices>().parse
                     ( first
                     , last
                     , context
                     , rcontext
                     , fusion::at_c<Indices>(attr)
                     //without collapsed attributes, the number of elements
                     //of sub_parser's is the same as the number of elements
                     //of Attribute; hence, the same Indices can be used
                     //for both.
                     )
                  );

```

However, when `defined(USE_COLLAPSE_LEFT)`, the much more complicated
sequence_parse_fold template class, defined in `detail/sequence.hpp`,
is needed.  The reason for the difference is that when
`defined(USE_COLLAPSE_LEFT)`, the user defined attributes are
**ASSUMED** to be collapsed, i.e. the unused_type's are removed.
This leads to the need for a much more complicated parsing expression
within the:

```

  sequence_parse_fold<*>::parse_at<ParserIndex,AttributeIndex>

```
member function:

```
          ; if constexpr(is_unused)
            { result=parser.parse(_first,_last,_context,_rcontext,unused)
            ; if(result)
              { result= parse_at<ParserIndex+1,AttributeIndex>()
              ;}
            ;}

```

The `is_unused` indicates if the current parser (the one indexed by
`ParserIndex`) has `unused_type` as its attribute. If so, then for the
next parse, `ParserIndex` is incremented but `AttributeIndex` is not
(because the Attribute, being collapsed, has no unused_type's).

The `else` branch increments both ParserIndex and AttributeIndex.

Now the complexity of sequence_parser_fold is nothing compared to the
much more complex `detail/sequence.hpp` code
[here](https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/x3/operator/detail/sequence.hpp#L124)
where the increased complexity is largely caused by the parser
elements being binary (`Left` and `Right`) instead of unfolded into a
template parameter pack (`SubParsers...`) .

