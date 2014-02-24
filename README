# Edmonds's Algorithm

This project contains an efficient implementation of Edmonds's optimum
branching algorithm as originally described by Tarjan.

For information on the licence, please refer to the file `COPYING`. For
usage information see the file `INSTALL`.


## Introduction

The package *edmonds-alg* contains a C++-implementation of Edmonds's
optimum branching algorithm as described by Tarjan in 1977. The code is
licensed under the open source MIT license.

A branching in a directed graph is defined as a set of directed edges
that contain no cycles and such that no two edges are directed towards
the same vertex. Note that this is equivalent to a forest of rooted
trees. Given weights on the edges of a directed graph, we define a
maximum branching as a branching such that the sum of the weights of its
edges is maximal. Since finding a maximum branching is equivalent to
finding a minimum branching, we often speak of optimum branchings
instead.

In 1967, Edmonds gave an
[algorithm](http://en.wikipedia.org/wiki/Edmonds's_algorithm) for
finding an optimum branching in a directed graph (others discovered
similar algorithms independantly at approximately the same time). In
1977, Tarjan gave the first description of an efficient implementation
of Edmonds's algorithm.


## Requirements

In order to maximize code reuse, the implementation is built on top of
concepts in the [Boost Graph Library](http://www.boost.org/doc/libs/release/libs/graph/doc/index.html)
The package also uses some other parts of Boost, most notably,
[Boost foreach](http://www.boost.org/doc/libs/release/doc/html/foreach.html)
which was first introduced in Boost version 1.34. Therefore, Boost
version 1.34 or later is required for use of this package.

[Scons](http://www.scons.org) is used as a build tool to build the
accompanying documentation and test programs. Also, test scripts are
written in [Python](http://www.python.org). The pdf documentation is
written in [LaTeX](http://en.wikipedia.org/wiki/LaTeX)


## Installation

The algorithm is implemented as a single template function in C++, and
as such, no libraries or executables need to be built. To use the
function, just copy the files

    src/edmonds_optimum_branching.hpp
    src/edmonds_optimum_branching_impl.hpp

to suitable locations where your compiler looks for header files. Under
Linux, a good place is usually `/urs/include`. You can also just copy the
two header files directly to the source directory of your program. Any
source file from which a call is made should include the header
`edmonds_optimum_branching.hpp`.

If you want to build the accompanying documentation and test programs,
you need to have Scons and Python installed (see Requirements
above). To build and test on Linux, run the following commands from
the root of the source directory:

    scons
    scons test

Or if the boost library is in a non-standard location, use the
following commands instead:

    scons boost=/path/to/boost/installation/root
    scons boost=/path/to/boost/installation/root test

Information on how to use the algorithm, see the file
`edmonds_optimum_branching.hpp`. Also, a good example of usage can be
found in the test program test/test.cpp. In the future, more examples
may be provided (feedback from users may speed up this process).
