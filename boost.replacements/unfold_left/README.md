# PURPOSE

In the 1.78 version of spirit x3, the expression:

  x >> y >> z

has type:

  sequence
  < type(x)
  , sequence
    < type(y)
    , type(z)
    >
  >

and the expression:

  x  | y  | z

has type:


  x  | y  | z &rarr;  |/[x,y,z]  

where:

  op/[x,y,z]

for:

* op = >>
* op = |

represents a binary spirit-x3 parser operator constructor:

	   sequence    for >>
	   alternative for |

but modified so that it's applicable to n-ary(n>=0) parser expressions instead of just 2 parser expressions.
Such n-ary constructors are defined within the `#ifdef USE_UNFOLD_LEFT` sections of:

* [sequence.hpp](./boost/spirit/home/x3/operator/sequence.hpp)
* [alternative.hpp](./boost/spirit/home/x3/operator/alternative.hpp)

Also, to possibly figure how to transform user attributes to the spirit attribute_of<parser>, if possible:

In the following, the notation:

   op / [x0,x1,...,xn]

is shorthand for:

   x0 op x1 op, ..., xn

IOW, / is the apl [reduce operator](https://en.wikipedia.org/wiki/APL_syntax_and_symbols#Operators_and_axis_indicator).

For example, the artribute_of in this library, when compiled with -DUSE_UNFOLD_LEFT, can contain:

	aof<x | y | z> &rarr;
	|/[aof<x>
	  ,aof<y>
	  ,aof<z>
	  ];
	aof<x & y & z> &rarr;
	&/[aof<x>
	  ,aof<y>
	  ,aof<z>
	  ];

but what if:

    y=&/[aof<y0>],aof<y1>,aof<y2>]:

then, the expression is "flattened":

  &/[aof<x>
    ,&/[aof<y0>
       ,aof<y1>
       ,aof<y2>
       ]
    ,aof<z>
    ]
  ==>
  &/[aof<x>
    ,aof<y0>
    ,aof<y1>
    ,aof<y2>
    ,aof<z>
    ]

  IOW, the tree is flattened if the operator is same for subtree's.
  OTOH, if the operators are different, then no flattening:

  &/[aof<x>
    ,|/[aof<y0>
       ,aof<y1>
       ,aof<y2>
       ]
    ,aof<z>
    ]
  ==>
  &/[aof<x>
    ,|/[aof<y0>
       ,aof<y1>
       ,aof<y2>
       ]
    ,aof<z>
    ]

  But will those transformations (indicated by ==>) always converge to
  same answer, no matter what order the transformation is applied?  I
  believe that's called the Church/Rosser property, and also the term
  "confluence" has been used.

  Yep, websearching "church rosser" reveals:

https://en.wikipedia.org/wiki/Church%E2%80%93Rosser_theorem

  which says:

In lambda calculus, the Church–Rosser theorem states that, when
applying reduction rules to terms, the ordering in which the
reductions are chosen does not make a difference to the eventual
result.
.
.
.
Viewing the lambda calculus as an abstract rewriting system, the
Church–Rosser theorem states that the reduction rules of the lambda
calculus are confluent.
  
