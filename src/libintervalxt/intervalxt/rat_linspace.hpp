/*****************************************************************************/
/* Rational linear subspaces                                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef FLATSURF_RAT_LINSPACE
#define FLATSURF_RAT_LINSPACE

// Class that models a Q-vector subspace of Q^d
// ideally there should be several implementation
// - a generator based version (ie, store generators)
// - a constraint based version (ie, store equations)
// of course they are dual to each other, but depending on the codimension
// of the space it might be more efficient to do one or the other.
//
// The most important non-trivial functionalities to focus on are
// - hasNonZeroNonNegativeVector()
// - hasPositiveVector()

class RationalLinearSubspace {
public:
    // sadly FLINT imposes matrix dimension
    // (no resize available)
    RatLinSpace();
    RatLinSpace(int dim);

    // action by the elementary matrix on R^d
    //   ei -> ei + x ej
    void elementaryTransformation(size_t i, size_t j, Coeff x);

    // swap action on R^d
    //   ei <-> ej
    void swap(size_t i, size_t j);

    // canonical form
    // This provides canonical generators (or equations) for the subspace
    // so that we can use 
    void canonicalize();

    // test for non-negative or positive vectors
    // (NOTE: this can be done via linear programming, eg call to PPL)
    bool hasNonZeroNonNegativeVector();
    bool hasPositiveVector();
}

#endif
