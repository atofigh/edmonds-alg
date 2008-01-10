#ifndef OPTIMUM_BRANCHING_NAIVE_HPP
#define OPTIMUM_BRANCHING_NAIVE_HPP

#include <vector>

template<typename Real, class Matrix>
void
optimum_branching_naive(const Matrix          &weights,
                          unsigned               num_vertices,
                          unsigned               root,
                          std::vector<unsigned> &parent);

#define OPTIMUM_BRANCHING_NAIVE
#include "common-opt-naive.hpp"
#undef OPTIMUM_BRANCHING_NAIVE

#endif /* OPTIMUM_BRANCHING_NAIVE_HPP */
