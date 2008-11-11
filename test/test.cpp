#include <iostream>
#include <vector>
#include <utility>
#include <iterator>
#include <cstdlib>
#include <limits>
#include <cerrno>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/concept_check.hpp>
#include <boost/operators.hpp>
#include <boost/iterator.hpp>
#include <boost/multi_array.hpp>
#include <boost/random.hpp>

#include <sys/time.h>
#include "../src/edmonds_optimum_branching.hpp"

using namespace std;
using namespace boost;

const char *PNAME =  "test";

// global random number generator
boost::mt19937 g_generator;

// definitions of a complete graph that implements the EdgeListGraph
// concept of Boost's graph library.
namespace boost {
    struct complete_graph {
        complete_graph(int n_vertices) : n_vertices(n_vertices) {}
        int n_vertices;
        
        struct edge_iterator : public input_iterator_helper<edge_iterator, int, std::ptrdiff_t, int const *, int>
        {
            int edge_idx, n_vertices;
            
            edge_iterator() : edge_idx(0), n_vertices(-1) {}
            edge_iterator(int n_vertices, int edge_idx) : edge_idx(edge_idx), n_vertices(n_vertices) {}
            edge_iterator &operator++()
            {
                if (edge_idx >= n_vertices * n_vertices)
                    return *this;
                ++edge_idx;
                if (edge_idx / n_vertices == edge_idx % n_vertices)
                    ++edge_idx;
                return *this;
            }
            int operator*() const {return edge_idx;}
            bool operator==(const edge_iterator &iter) const
            {
                return edge_idx == iter.edge_idx;
            }
        };
    };
    
    template<>
    struct graph_traits<complete_graph> {
        typedef int                             vertex_descriptor;
        typedef int                             edge_descriptor;
        typedef directed_tag                    directed_category;
        typedef disallow_parallel_edge_tag      edge_parallel_category;
        typedef edge_list_graph_tag             traversal_category;
        typedef complete_graph::edge_iterator   edge_iterator;
        typedef unsigned                        edges_size_type;
        
        static vertex_descriptor null_vertex() {return -1;}
    };

    pair<complete_graph::edge_iterator, complete_graph::edge_iterator>
    edges(const complete_graph &g)
    {
        return make_pair(complete_graph::edge_iterator(g.n_vertices, 1),
                         complete_graph::edge_iterator(g.n_vertices, g.n_vertices*g.n_vertices));
    }
    
    unsigned
    num_edges(const complete_graph &g)
    {
        return (g.n_vertices - 1) * (g.n_vertices - 1);
    }
    
    int
    source(int edge, const complete_graph &g)
    {
        return edge / g.n_vertices;
    }
    
    int
    target(int edge, const complete_graph &g)
    {
        return edge % g.n_vertices;
    }
}

typedef graph_traits<complete_graph>::edge_descriptor Edge;
typedef graph_traits<complete_graph>::vertex_descriptor Vertex;

// is_cyclic()
//
// returns true if the choice of parents for the vertices creates a
// cycle, i.e. if 'parent' does not represent a branching
bool
is_cyclic(const vector<Vertex> &parent)
{
    unsigned size = parent.size();
    vector<bool> visited(size, false);

    for (unsigned i = 0; i < size; ++i)
    {
        if (visited[i])
            continue;

        vector<bool> visited_this_iteration(size, false);
        visited[i] = true;
        visited_this_iteration[i] = true;
        Vertex v = i;
        while (parent[v] != v)
        {
            v = parent[v];
            if (visited_this_iteration[v])
                return true;
            visited_this_iteration[v] = true;
            if (visited[v])
                break;
        }
    }
    return false;
}


// next_parent()
//
// Creates the next lexicographical parent vector. Returns true if the
// the parent returned is not the lexicographically first parent vector.
bool
next_parent(vector<Vertex> &parent)
{
    unsigned size = parent.size();

    bool carry = true;
    unsigned i = 0;
    while (carry && i < size)
    {
        parent[i] += 1;
        if (parent[i] < size)
        {
            carry = false;
        }
        else
        {
            parent[i] = 0;
        }
        ++i;
    }

    return !carry;
}



int
main(int argc, char *argv[])
{
    // Parse and read the command line arguments.
    if (argc != 5)
    {
        cerr << "Usage: " << PNAME
             << " <min-weight> <max-weight> <#vertices> <#tests>\n";
        exit(1);
    }

    long int min_weight = strtol(argv[1], 0, 0);
    long int max_weight = strtol(argv[2], 0, 0);
    long int n_vertices = strtol(argv[3], 0, 0);
    long int n_tests = strtol(argv[4], 0, 0);

    if (errno)
    {
        cerr << "Bad arguments\n";
        exit(1);
    }

    // Check consistency of command line arguments.
    if (n_tests < 0)
    {
        cerr << "the number of tests must be a positive integer\n";
        exit(1);
    }
    
    if (n_vertices > numeric_limits<unsigned>::digits)
    {
        cerr << "Too many vertices\n"
             << "Maximum n.o. vertices is "
             << numeric_limits<unsigned>::digits << "\n";
        exit(1);
    }

    if (n_vertices < 2)
    {
        cerr << "Too few vertices\n"
             << "must be at least two\n";
        exit(1);
    }

    if (min_weight > max_weight)
    {
        cerr << "min weight must be greater than max weight\n";
        exit(1);
    }

    if (abs(max_weight) >= numeric_limits<int>::max() / n_vertices ||
        abs(min_weight) >= numeric_limits<int>::max() / n_vertices)
    {
        cerr << "weights too large\n";
        exit(1);
    }

    // Initialize the random number generator.
    timeval tv;
    gettimeofday(&tv, 0);
    g_generator.seed(tv.tv_sec * 1000000 + tv.tv_usec);
    boost::uniform_int<int> uniform_int_dist(min_weight, max_weight);
    boost::variate_generator<mt19937, uniform_int<int> > rng_i(g_generator, uniform_int_dist);
    
    // Perform the tests.
    enum {MAX_SPAN_0, MAX_SPAN_1, MAX_SPAN_2,
          MAX_NOSPAN_0, MAX_NOSPAN_1, MAX_NOSPAN_2,
          MIN_SPAN_0, MIN_SPAN_1, MIN_SPAN_2,
          MIN_NOSPAN_0, MIN_NOSPAN_1, MIN_NOSPAN_2,
          N_CASES};
    string case_names[] = {
        "MAX_SPAN_0", "MAX_SPAN_1", "MAX_SPAN_2",
        "MAX_NOSPAN_0", "MAX_NOSPAN_1", "MAX_NOSPAN_2",
        "MIN_SPAN_0", "MIN_SPAN_1", "MIN_SPAN_2",
        "MIN_NOSPAN_0", "MIN_NOSPAN_1", "MIN_NOSPAN_2"
    };

    complete_graph      g(n_vertices);
    multi_array<int, 2> weights(extents[n_vertices][n_vertices]);
    vector<Vertex>      parent(n_vertices);
    Vertex              roots[] = {0, 1};

    vector<Edge>        branching;
    vector< vector< Edge > > all_branchings(N_CASES);
    vector<int>         ans(N_CASES);

    while (n_tests--)
    {
        // Clear all the branchings from previous iteration.
        BOOST_FOREACH (vector<Edge> &vec, all_branchings)
        {
            vec.clear();
        }

        // create new weights
        int *end = weights.origin() + n_vertices * n_vertices;
        for (int *ip = weights.origin(); ip != end; ++ip)
        {
            *ip = rng_i();
        }
        
        // run edmonds algorithm for a few cases. The cases will be
        // the cross product of the following properties:
        // optimum-is-maximum x attempt-to-span x num-specified-roots
        // where num-specified roots is either 0, 1, or 2. Also the
        // specified roots are either none, the vertex 0, or the
        // vertices 0 and 1.
        edmonds_optimum_branching<true, true, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 0, back_inserter(all_branchings[MAX_SPAN_0]));
        edmonds_optimum_branching<true, true, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 1, back_inserter(all_branchings[MAX_SPAN_1]));
        edmonds_optimum_branching<true, true, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 2, back_inserter(all_branchings[MAX_SPAN_2]));
        edmonds_optimum_branching<true, false, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 0, back_inserter(all_branchings[MAX_NOSPAN_0]));
        edmonds_optimum_branching<true, false, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 1, back_inserter(all_branchings[MAX_NOSPAN_1]));
        edmonds_optimum_branching<true, false, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 2, back_inserter(all_branchings[MAX_NOSPAN_2]));
        edmonds_optimum_branching<false, true, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 0, back_inserter(all_branchings[MIN_SPAN_0]));
        edmonds_optimum_branching<false, true, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 1, back_inserter(all_branchings[MIN_SPAN_1]));
        edmonds_optimum_branching<false, true, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 2, back_inserter(all_branchings[MIN_SPAN_2]));
        edmonds_optimum_branching<false, false, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 0, back_inserter(all_branchings[MIN_NOSPAN_0]));
        edmonds_optimum_branching<false, false, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 1, back_inserter(all_branchings[MIN_NOSPAN_1]));
        edmonds_optimum_branching<false, false, true>
            (g, identity_property_map(), weights.origin(),
             roots, roots + 2, back_inserter(all_branchings[MIN_NOSPAN_2]));

        for (int i = 0; i < N_CASES; ++i)
        {
            ans[i] = 0;
            BOOST_FOREACH (Edge e, all_branchings[i])
            {
                ans[i] += weights[source(e, g)][target(e, g)];
            }
        }

        // Exhaustively test all branchings and check if the answers
        // from edmonds's algorithm is optimal.
        int bad_case = -1;
        fill(parent.begin(), parent.end(), 0);
        do
        {
            if (is_cyclic(parent))
                continue;
            
            int weight = 0;
            for (int i = 0; i < n_vertices; ++i)
            {
                if (parent[i] == i)
                    continue;
                
                weight += weights[parent[i]][i];
            }

            int num_roots = 0;
            for (int i = 0; i < n_vertices; ++i)
            {
                num_roots += parent[i] == i ? 1 : 0;
            }

            bad_case = MAX_NOSPAN_0;
            if (weight > ans[bad_case])
                break;

            bad_case = MIN_NOSPAN_0;
            if (weight < ans[bad_case])
                break;

            bad_case = MAX_NOSPAN_1;
            if (parent[0] == 0 && weight > ans[bad_case])
                break;

            bad_case = MIN_NOSPAN_1;
            if (parent[0] == 0 && weight < ans[bad_case])
                break;

            bad_case = MAX_NOSPAN_2;
            if (parent[0] == 0 && parent[1] == 1 && weight > ans[bad_case])
                break;

            bad_case = MIN_NOSPAN_2;
            if (parent[0] == 0 && parent[1] == 1 && weight < ans[bad_case])
                break;

            bad_case = MAX_SPAN_0;
            if (num_roots == 1 && weight > ans[bad_case])
                break;

            bad_case = MIN_SPAN_0;
            if (num_roots == 1 && weight < ans[bad_case])
                break;

            bad_case = MAX_SPAN_1;
            if (num_roots == 1 && parent[0] == 0 && weight > ans[bad_case])
                break;

            bad_case = MIN_SPAN_1;
            if (num_roots == 1 && parent[0] == 0 && weight < ans[bad_case])
                break;

            bad_case = MAX_SPAN_2;
            if (num_roots == 1 && parent[0] == 0 && parent[1] == 1 &&
                weight > ans[bad_case])
                break;

            bad_case = MIN_SPAN_2;
            if (num_roots == 1 && parent[0] == 0 && parent[1] == 1 &&
                weight < ans[bad_case])
                break;

            bad_case = -1;
        } while (next_parent(parent));

        // If we broke out of the loop prematurely, then something was
        // wrong.
        if (bad_case > 0)
        {
            cout << "\ntest failed.\n";
            
            cout << "weights:\n";
            for (int i = 0; i < n_vertices; ++i)
            {
                for (int j = 0; j < n_vertices; ++j)
                {
                    cout << weights[i][j] << " ";
                }
                cout << "\n";
            }
            cout << "\n";
            
            int weight = 0;
            for (int i = 0; i < n_vertices; ++i)
            {
                if (parent[i] == i)
                    continue;
                
                weight += weights[parent[i]][i];
            }

            cout << "counter example (weight = " << weight << "):\n";
            for (int i = 0; i < n_vertices; ++i)
            {
                if (parent[i] == i)
                    continue;
                cout << "(" << parent[i] << ", " << i << ")\t";
            }
            cout << "\n";

            weight = 0;
            cout << "case: " << case_names[bad_case] << ":\n";
            BOOST_FOREACH (Edge e, all_branchings[bad_case])
            {
                cout << "(" << source(e, g) << ", " << target(e, g) << ")\t";
                weight += weights[source(e, g)][target(e, g)];
            }
            cout << "\n";
            cout << "weight:\t" << weight << "\n";

            exit(1);
        }
        cerr << ".";
    }
    cerr << "\n";

    return EXIT_SUCCESS;
}
