To "unfold" operators associating to left.
E.g. x >> y >> z -> >>(x,y,z)
     x | y | z -> |(x,y,z)

Also, to possibly figure how to transform user attributes to the spirit attribute_of<parser>, if possible:

In the following, the notation:

  op / [x0,x1,...,xn]

is shorthand for:

  x0 op x1 op, ..., xn

IOW, / is the fold operator.

For example, the artribute_of in this library, when compiled with -DUSE_UNFOLD_LEFT, can contain:

  aof<x | y | z> =
  |/[aof<x>
    ,aof<y>
    ,aof<z>
    ];
  aof<x & y & z> =
  &/[aof<x>
    ,aof<y>
    ,aof<z>
    ];

but what if y=&/[aof<y0>],aof<y1>,aof<y2>]:

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
  
