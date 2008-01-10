#ifndef MAX_SPANNING_ARBORESCENCE_NAIVE_HPP
#define MAX_SPANNING_ARBORESCENCE_NAIVE_HPP

#include <vector>

/** @file */
/**
 * @brief Given weights on the complete graph on 'num_vertices' vertices, the
 * function computes a maximum weight spanning arborescence rooted at
 * 'root'.
 *
 * This function implements a variation of Tarjan's implementation of
 * Edmond's algorithm for maximum branchings (also described in a
 * paper by Camerini, Fratta, and Maffioli in Networks, vol. 10
 * (1980)). The function implements naive algorithm for dense
 * graphs with time complexity O(exp(n)), where n is the number of
 * vertices.
 *
 * The input to the function consists of a two-dimensional array,
 * giving the weights on the complete graph on n vertices, a 'root'
 * identifying the vertex that will be the root of the spanning
 * arborescence, and a vector 'parent' to which the result is
 * written. It is the responsibility of the caller to ensure that
 * 'parent' has been initialized and contains num_vertices elements,
 * i.e., parent.size() == num_vertices.
 *
 * For each vertex v != root, the vector 'parent' will contain
 * the parent of v in the spanning arborescence.
 *
 * The function is templatized on 'Real' representing the floating
 * point type used for the weights, and the 'Matrix' class that is the
 * container holding the weights. The 'Matrix' is expected to support
 * the usual matrix subscript operations (e.g. weights[i][j] =
 * Real(5.0)), and must have a copy constructor. The 'Real' type must
 * support the same basic operations as the built in floating point
 * types.
 *
 * @param[in] weights a two-dimensional array of the size num_vertices x num_vertices 
 * @param[in] num_vertices the number of vertices
 * @param[in] root identifying the vertex that will be the root of the spanning arborescence
 * @param[out] parent the resulting spanning arborescence, a vector of size num_vertices
 * @return nothing
 */

template<typename Real, class Matrix>
void
max_spanning_arborescence_naive(const Matrix          &weights,
                                unsigned               num_vertices,
                                unsigned               root,
                                std::vector<unsigned> &parent);


#define MAX_SPANNING_ARBORESCENCE_NAIVE
#include "common-opt-naive.hpp"
#undef MAX_SPANNING_ARBORESCENCE_NAIVE

#endif /* MAX_SPANNING_ARBORESCENCE_NAIVE_HPP */
