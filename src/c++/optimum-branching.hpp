#ifndef OPTIMUM_BRANCHING_HPP
#define OPTIMUM_BRANCHING_HPP

#include <vector>

template<typename Real, class Matrix>
void
optimum_branching(const Matrix          &weights,
                          unsigned               num_vertices,
                          unsigned               root,
                          std::vector<unsigned> &parent);

#define OPTIMUM_BRANCHING
#include "common-opt.hpp"
#undef OPTIMUM_BRANCHING

#endif /* OPTIMUM_BRANCHING_HPP */
