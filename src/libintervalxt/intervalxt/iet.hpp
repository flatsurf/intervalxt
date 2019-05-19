/*****************************************************************************/
/*                                                                           */
/* Interval exchange transformations.                                        */
/*                                                                           */
/*       Copyright (C) 2019 Vincent Delecroix <vincent.delecroix@labri.fr>   */
/*                                                                           */
/*  Distributed under the terms of the GNU General Public License (GPL)      */
/*                                                                           */
/*                  http://www.gnu.org/licenses/                             */
/*                                                                           */
/*                                                                           */
/* The main purpose of the library is to decompose an interval exchange      */
/* transformation (with exact coordinates) into periodic pieces and          */
/* non-periodic pieces. To certify that some pieces do not have              */
/* periodic trajectory we use a variant of an algorithm of                   */
/* M. Boshernitzan that he used for iet over quadratic fields.               */
/*                                                                           */
/* For each cylinder found we compute:                                       */
/* - the homology class in H_1(M \ Sigma; Z) of its core curve               */
/* - the list of saddle connections on the boundaries                        */
/* - its width (cylinders are thought as vertical and circumference can      */
/*   be obtained from the coordinates of its core curve)                     */
/*                                                                           */
/* For each non-periodic part we compute:                                    */
/* - [?] the span in homology of H_1(M \ Sigma; Z)                           */
/*****************************************************************************/

#ifndef FLATSURF_IET
#define FLATSURF_IET

#include <ostream>
#include <vector>

// All datastructures are templated with two types:
//    Tlen: the type used for the lengths of the iet (typically mpq_class or
//          renf_elem_class or RealNumber)
//    Tmat: the integral type used for the circumference coordinates (typically
//          unsigned long or mpz_class)
 
// floor division (needed for Zorich acceleration)
template <typename Tlen, typename Tmat> Tmat fdiv(Tlen& a, Tlen &b);

// forward declarations
template <typename Tlen, typename Tmat> class Label;
template <typename Tlen, typename Tmat> struct Interval;
template <typename Tlen, typename Tmat> class IntervalExchangeTransformation;

// A Label is the data attached to a pair of matched intervals on the top
// and bottom
template <typename Tlen, typename Tmat>
class Label
{
public:

    Tlen length;             // length of the subinterval (real part, >= 0)
    std::vector<Tmat> v;     // Kontsevich-Zorich cocycle (= coordinate of core curves)
    size_t index;            // index in the iet (a number in {0, 1, ..., nb label - 1}

    Interval<Tlen, Tmat> i1, i2;  // top and bot subintervals

    Label();

    friend class IntervalExchangeTransformation<Tlen,Tmat>;
    friend class Interval<Tlen,Tmat>;
};

typedef std::vector<unsigned int> Permutation;

// An Interval models either a subinterval of the bottom or the top.
// It keeps pointers to the subinterval on the left, on the right
// and its twin. It is implemented as a doubled chained list so that Rauzy
// induction can be efficiently done
template <typename Tlen, typename Tmat>
class Interval
{
public:
    struct Interval *prev;    // interval on the left
    struct Interval *next;    // interval on the right
    struct Interval *twin;    // the twin interval
    Label<Tlen, Tmat> * lab;  // the label

    friend class IntervalExchangeTransformation<Tlen, Tmat>;
    friend class Label<Tlen, Tmat>;
};

template <typename Tlen, typename Tmat>
class IntervalExchangeTransformation
{
    size_t n;                // number of intervals (= pairs of Label)

    Interval<Tlen,Tmat> * top;       // pointer to the first interval on top
    Interval<Tlen,Tmat> * bot;       // pointer to the first interval on bottom

    std::vector<Label<Tlen,Tmat>> labels;          // the labels

    void reset(size_t nintervals);
public:
    IntervalExchangeTransformation(size_t n);
    IntervalExchangeTransformation(const Permutation& topperm, const Permutation& botperm);

    // check whether data is consistent
    void check();

    // data access
    Permutation topPermutation() const;
    Permutation botPermutation() const;
    std::vector<Tlen> lengths() const;
    std::vector<std::vector<Tmat>> KontsevichZorichCocycle() const;

    // set or reset data
    void setIdentityPermutation();
    void setTop(const Permutation &p);
    void setBot(const Permutation &p);
    void setLengths(const std::vector<Tlen>& lengths);

    // test whether the permutation is irreducible. If it is reducible
    // return true and set argument to the labels on top and bot
    // after which we cut. Otherwise returns fals.
    bool isReducible() const;
    bool isReducible(Label<Tlen,Tmat> *&, Label<Tlen,Tmat> *&) const;

    // check for periodic trajectory via Boshernitzan's algorithm
    bool hasNoPeriodicTrajectory() const;

    // assuming that top[0] > bot[0] and perform one step of Zorich induction
    // (if top[0] <= bot[0] the iet is left unchanged)
    void zorichInductionStep();
    void zorichInductions(size_t n);

    // swap the top and bottom intervals
    void swapTopBot();

    // remove the first pair of intervals (assuming that it corresponds to
    // a cylinder, that is the leftmost singularity is a connection of length
    // one). After such cylinder is removed the permutation could be reducible.
    void dropSaddleConnection();

    // Warning: destructive!
    // each component found is put as a vector component
    std::vector<IntervalExchangeTransformation<Tlen,Tmat>> periodicNonPeriodicDecomposition();

    template <typename TTlen, typename TTmat>
    friend std::ostream& operator << (std::ostream &, const IntervalExchangeTransformation<TTlen, TTmat>&);
};


#endif
