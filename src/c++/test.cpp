#include "max-spanning-arborescence-naive.hpp"
#include "max-spanning-arborescence.hpp"
#include "optimum-branching.hpp"

#include <boost/multi_array.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>

void fill( boost::multi_array<double,2> & m, unsigned & msize ) {
  for ( boost::multi_array_types::index i = 0; i < msize ; ++ i) 
    {
      for ( boost::multi_array_types::index j = 0; j < msize; ++ j)
	{
	  m[i][j] = 3 * i + j; 
	}
    }
  return;
};  

void print( boost::multi_array<double,2> & m, unsigned & msize ) {
  for ( boost::multi_array_types::index i = 0; i < msize ; ++ i) 
    {
      for ( boost::multi_array_types::index j = 0; j < msize; ++ j)
	{
	  std::cout << 	  m[i][j] << " ";
	}
      std::cout << std::endl;
    }
  return;
};  


void fillrandom( boost::multi_array<double,2> & m, unsigned & msize,   std::vector<double> & values  ) {

  for ( boost::multi_array_types::index i = 0; i < msize ; ++ i) 
    {
      for ( boost::multi_array_types::index j = 0; j < msize; ++ j)
	{
          int random = int( (values.size() )*(rand()/(RAND_MAX + 1.0))); 
	  m[i][j] = values[random];
	}
    }
  return;
};  

void fillrandom2( boost::multi_array<double,2> & m, unsigned & msize,   std::vector<double> & values  ) {

  for ( boost::multi_array_types::index i = 0; i < msize ; ++ i) 
    {
      for ( boost::multi_array_types::index j = 0; j < msize; ++ j)
	{

	  m[i][j] = int( (4 )*(rand()/(RAND_MAX + 1.0))) ;
	}
    }
  return;
};  



int main(int argc, char *argv[]) {


    unsigned num_vertices = atoi(argv[1]);
    //    unsigned num_vertices = 4;
  unsigned root = 0;
  boost::multi_array<double,2>  m(boost::extents[num_vertices][num_vertices]);
  //  fill( m, num_vertices );
  std::vector<double> v;

  v.push_back(2);
  v.push_back(1);
  v.push_back(3);
  v.push_back(4);

  //  m[0][2]=2;
  // m[2][0]=1;

 for ( int j = 0; j < 100 ; j++ ) {
  srand(j);
       fillrandom2( m, num_vertices, v );
  //  fill( m, num_vertices );
       //         print(m, num_vertices );
  std::vector<unsigned> parent(num_vertices);
  std::vector<unsigned> parent2(num_vertices);
  std::vector<unsigned> parent3(num_vertices);

  //   std::cout << "j=" << j << "started n2"  << std::endl;
    //      max_spanning_arborescence<double>(m, num_vertices, root, parent);
  //  }
  //std::cout << "started naive"  << std::endl;
      //      max_spanning_arborescence_naive<double>(m, num_vertices, root, parent2);
   optimum_branching<double>(m, num_vertices, root, parent3);


   double sumn2  = sum(m,num_vertices,root,parent);

   double sumnaive  = sum(m,num_vertices,root,parent2);

   //  std::cout << "n=" << num_vertices << " j=" << j  << " sumn2=" << sumn2 << " sumnaive=" << sumnaive << std::endl;

   if ( sumn2 != sumnaive ) {
     std::cout << std::endl;
    print(m, num_vertices );
    std::cout << "num_vertices=" << num_vertices << " j=" << j  << " sumn2=" << sumn2 << " sumnaive=" << sumnaive << std::endl;
     std::cout << std::endl;
   };

 }

  return EXIT_SUCCESS;
}
