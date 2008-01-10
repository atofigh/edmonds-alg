#include <numeric>
#include <set>
#include <iostream>


#ifdef MAX_SPANNING_ARBORESCENCE_NAIVE
 #define OPTFUNCTION max_spanning_arborescence_naive
#endif

#ifdef OPTIMUM_BRANCHING_NAIVE
 #define OPTFUNCTION optimum_branching_naive
#endif

#ifndef HELP_FUNCTIONS_NAIVE
#define HELP_FUNCTIONS_NAIVE

            
/*
 * next_graph()
 *
 * Changes the vector 'parent[0..num_vertices-1]' to the
 * lexicographically next graph. If each element of
 * 'parent[0..num_vertices-1]' is less than num_vertices, then the
 * same will hold after the call. Note that no other guarantees are
 * made. In particular, it is possible that p[i] == i.
 */
void
next_graph(std::vector<unsigned>        &parent,
           unsigned                      num_vertices,
           unsigned                      root)
{
    for (unsigned i = 0; i < num_vertices; ++i)
        {
            if (i == root)
                continue;

            parent[i] += 1;
            
            if (parent[i] < num_vertices)
                {
                    break;
                }
            else
                {
                    parent[i] = 0;
                }
        }
}


/*
 * is_spanning_arborescence()
 *
 * returns true if and only if 'parent[0..num_vertices-1]' represents
 * a valid spanning arborescence.
 */
bool
is_spanning_arborescence(const std::vector<unsigned>  &parent,
                         unsigned                      num_vertices,
                         unsigned                      root)
{
    /*
     * Check that parent[i] < num_vertices and that parent[i] != i.
     */
    for (unsigned i = 0; i < num_vertices; ++i)
        {
            if (i == root)
                continue;
            
            if (parent[i] >= num_vertices || parent[i] == i)
                return false;
        }

    /*
     * Check that we can reach the root from every vertex, and that
     * there are no cycles.
     */

    /*
     * visited keeps track of already visited nodes. Once we reach one
     * of these, we know that they have already been checked and we
     * can continue with the next vertex.
     */
    std::vector<bool> visited(num_vertices, false);
    visited[root] = true;
    for (unsigned i = 0; i < num_vertices; ++i)
        {
            std::set<unsigned> visited_this_turn;
            if (visited[i] == true)
                continue;

            visited_this_turn.insert(i);
            unsigned j = parent[i];
            while (j != root && visited[j] == false)
                {
                    /* return false if we found a cycle. */
                    if (visited_this_turn.find(j) != visited_this_turn.end())
                        return false;
                    
                    visited_this_turn.insert(j);
                    j = parent[j];
                }
            
            for( std::set<unsigned>::const_iterator it = visited_this_turn.begin(); it != visited_this_turn.end() ; ++it )
                {
                    visited[*it] = true;
                }

        }
    
    return true;
}


/*
 * get_cost()
 *
 * returns the cost of 'parent'. It is assumed that
 * 'parent[0..num_vertices-1]' is a valid spanning arborescence.
 */
template<typename Real, class Matrix>
Real
get_cost(std::vector<unsigned>        &parent,
         unsigned                      num_vertices,
         unsigned                      root,
         const Matrix                 &weights)
{
    /*
     * Just loop through the parent vector and sum the costs of the
     * edges. We are assuming that parent is a valid arborescence!
     */
    Real cost(0.0);
    for (unsigned i = 0; i < num_vertices; ++i)
        {
            if (i != root)
                {
                    cost += weights[ parent[i] ][i];
                }
        }
    return cost;
}

#endif /* HELP_FUNCTIONS_NAIVE */


template<typename Real, class Matrix>
void
OPTFUNCTION(const Matrix          &weights,
                                unsigned               num_vertices,
                                unsigned               root,
                                std::vector<unsigned> &parent)
{
    std::fill(parent.begin(), parent.end(), root);
    std::vector<unsigned> parent_copy(parent);
    Real maxcost = get_cost<Real>(parent_copy, num_vertices, root, weights);

    for (next_graph(parent_copy, num_vertices, root);
         std::count(parent_copy.begin(), parent_copy.end(), root) != num_vertices;
         next_graph(parent_copy, num_vertices, root))
        {

#ifdef MAX_SPANNING_ARBORESCENCE_NAIVE
            if (is_spanning_arborescence(parent_copy, num_vertices, root))
#endif
                {
                    Real cost = get_cost<Real>(parent_copy, num_vertices,
                                               root, weights);
                    if (cost > maxcost)
                        {
                            maxcost = cost;
                            copy(parent_copy.begin(), parent_copy.end(),
                                 parent.begin());
                        }
                }
        }
}



#undef OPTFUNCTION 

