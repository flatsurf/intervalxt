The main purpose of the library is to decompose an interval exchange
transformation (with exact coordinates) into periodic pieces and non-periodic
pieces. To certify that some pieces do not have periodic trajectory we use a
variant of an algorithm of M. Boshernitzan that he used for iet over quadratic
fields.

For each cylinder found we compute:
- the homology class in `H_1(M \ Sigma; Z)` of its core curve
- the list of saddle connections on the boundaries
- its width (cylinders are thought as vertical and circumference can be
  obtained from the coordinates of its core curve)

For each non-periodic part we compute:
- [?] the span in homology of `H_1(M \ Sigma; Z)`

All datastructures are templated with two types:
`Tlen`: the type used for the lengths of the iet (typically `mpq_class` or
`renf_elem_class` or `Element`)
`Tmat`: the integral type used for the circumference coordinates (typically
`unsigned long` or `mpz_class`)
