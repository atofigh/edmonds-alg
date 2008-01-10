#include <cstdlib>
#include <iostream>
#include <cassert>
#include <libxml/xpath.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/relaxng.h>

#include "max-spanning-arborescence-naive.hpp"
#include "max-spanning-arborescence.hpp"
#include "optimum-branching.hpp"
#include "config.h"
#include "xml.hpp"

#include <boost/multi_array.hpp>
#include <vector>
#include <getopt.h>


void usage(){
  std::cout << std::endl 
            << "This program evaluates a tree. The tree is read in xml format from standard input or from" << std::endl
	    << "a file if specified. The result is printed to standard output in xml format." 
            << std::endl << std::endl 
	    << "Usage: " << COMMANDLINENAME << " [OPTIONS] [FILE]" << std::endl << std::endl
	    << "  -h, --help              print help and exit" << std::endl
	    << "  -V, --version           print version and exit" << std::endl
	    << "  -R, --print-relaxng     print relaxng schema for the input xml format" << std::endl
            << std::endl
            << "The format of the xml in- and output is described at the home page:" << std::endl
            << "http://edmonds-alg.sourceforge.net/" << std::endl;
}

void version(){
  std::cout << COMMANDLINENAME << " " << VERSION << std::endl;
}

int main(int argc, char *argv[]) {

  int c;
  int option_index = 0;
  char * filename = "stdin";

  static const struct option options[] = {
    { "help",              no_argument,       0, 'h' },
    { "version",           no_argument,       0, 'v' },
    { "print-relaxng",           no_argument,       0, 'R' },
    { 0, 0, 0, 0 }
  };

  while ((c = getopt_long( argc, argv, "hvR",
			   options, &option_index )) != -1 ) 
    {
      switch (c)
	{
	case 'v': 
	  version();
	  exit(EXIT_SUCCESS);
	  break;
	case 'h': 
	  version();
	  usage();
	  exit(EXIT_SUCCESS);
	  break;
	case 'R': 
	  printRelaxng(); 
	  exit(EXIT_SUCCESS);
	  break;
	default:
	  exit(EXIT_FAILURE);
	}
    }

  int fd;

  if ( optind < argc) 
    {
      if ( argc - optind == 1 ) 
	{
	  filename = argv[optind ];
	  FILE * f = fopen( filename, "r" );
	  if ( f == NULL )
	    {     
	      std::cerr << "error: Could not open file:" << filename <<  std::endl;
              exit(EXIT_FAILURE); 
	    }

          fd = fileno(f);
        }
      else
	{
	  std::cerr << "Too many arguments" << optind << " " << argc <<  std::endl;
	  exit(EXIT_FAILURE);
	}
	
    } else
    {
      fd = fileno(stdin );
    }
  int root;
  int num_vertices;
  xmlChar * algorithm;
  boost::multi_array<double,2> m;
  parseEdmondsXML(fd, filename, m, root, num_vertices, &algorithm );

  std::vector<unsigned> parent(num_vertices);

  if ( root == -1 ) { 
	  std::cerr << std::endl << "non-root algorithm not implemented yet" << std::endl;
	  exit(EXIT_FAILURE);
  }

  if ( ! xmlStrcmp( algorithm , (const xmlChar *) "msa" ) ) {  
    max_spanning_arborescence<double>(m, num_vertices, root, parent);
  } 
  else 
      if ( ! xmlStrcmp( algorithm , (const xmlChar *) "ob" ) ) {  
	optimum_branching<double>(m, num_vertices, root, parent);
      } 
      else 
	{
	  std::cerr << std::endl << "error: no algorithm found" << std::endl;
	  exit(EXIT_FAILURE);
	}

  xmlDocPtr doc_out = NULL;   
  xmlNodePtr root_out = NULL;

  doc_out = xmlNewDoc(BAD_CAST "1.0");
  root_out = xmlNewNode(NULL, BAD_CAST "result");
  xmlDocSetRootElement(doc_out, root_out);
  char buff[256];

  double s = sum(m,num_vertices,root,parent);
  snprintf(buff,256,"%f",s);  
  xmlNewProp(root_out, BAD_CAST "sum", BAD_CAST buff );

  for ( std::vector<unsigned>::const_iterator iter = parent.begin(); iter != parent.end(); ++iter ) {
    snprintf(buff,256,"%i",*iter);
    xmlNodePtr iedge = xmlNewChild(root_out, NULL, BAD_CAST "incomingedge", BAD_CAST buff);
  }

  xmlSaveFormatFileEnc("-",doc_out,"UTF-8",1);
  xmlFreeDoc(doc_out);
  xmlCleanupParser();

  return EXIT_SUCCESS;
}
