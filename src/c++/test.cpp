#include <cstdlib>
#include <iostream>
#include <cassert>
#include <libxml/xpath.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/relaxng.h>

#include "max-spanning-arborescence.hpp"
#include "max-spanning-arborescence-naive.hpp"
#include "optimum-branching.hpp"
#include "optimum-branching-naive.hpp"
#include "config.h"
#include "xml.hpp"

#include <boost/multi_array.hpp>
#include <vector>
#include <getopt.h>


int compareSum( boost::multi_array<double,2> &m,int num_vertices,int root,  std::vector<unsigned> parent1, std::vector<unsigned> parent2)
{
  double s1 = sum(m,num_vertices,root,parent1);
  double s2 = sum(m,num_vertices,root,parent2);
  if ( s1 == s2 )
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {

  if ( argc != 2 ) 
    {
       std::cerr << "error: test program not started properly. Wrong number of arguments." <<  std::endl;
       exit(EXIT_FAILURE); 
    };

  char * filename = argv[1];
  FILE * f = fopen( filename, "r" );
  if ( f == NULL )
    {     
      std::cerr << "Could not open the file  \"" << filename << "\""  <<  std::endl;
      exit(EXIT_FAILURE);
    };

  int fd = fileno(f);
  int root;
  int num_vertices;
  xmlChar * algorithm;
  boost::multi_array<double,2> m;
  parseEdmondsXML(fd, filename, m, root, num_vertices, &algorithm );
  std::vector<unsigned> parent(num_vertices);
  std::vector<unsigned> parent_naive(num_vertices);

  if ( root == -1 ) { 
	  std::cerr << std::endl << "non-root algorithm not implemented yet" << std::endl;
	  exit(EXIT_FAILURE);
  }

  if ( ! xmlStrcmp( algorithm , (const xmlChar *) "msa" ) ) {  
    max_spanning_arborescence<double>(m, num_vertices, root, parent);
    max_spanning_arborescence_naive<double>(m, num_vertices, root, parent_naive);
  } 
  else 
      if ( ! xmlStrcmp( algorithm , (const xmlChar *) "ob" ) ) {  
	optimum_branching<double>(m, num_vertices, root, parent);
	optimum_branching_naive<double>(m, num_vertices, root, parent_naive);
      } 
      else 
	{
	  std::cerr << std::endl << "error: no algorithm found" << std::endl;
	  exit(EXIT_FAILURE);
	}
  return compareSum(m,num_vertices,root,parent,parent_naive);
}
