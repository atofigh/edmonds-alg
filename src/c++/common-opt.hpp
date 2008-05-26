#include <numeric>
#include <set>
#include <stack>
#include <vector>
#include <ext/slist>
#include <list>

#include <iostream>
#include <boost/multi_array.hpp>
#include "disjointset.hpp"

// http://gcc.gnu.org/onlinedocs/libstdc++/faq/index.html#5_4

#ifdef __GNUC__
#if __GNUC__ < 3
 #include <slist.h>
 namespace Sgi { using ::slist; }; // inherit globals
#else
 #include <ext/slist>
 #if __GNUC_MINOR__ == 0
  namespace Sgi = std;               // GCC 3.0
 #else
  namespace Sgi = ::__gnu_cxx;       // GCC 3.1 and later
 #endif
#endif
#else      // ...  there are other compilers, right?
 namespace Sgi = std;
#endif

#ifdef MAX_SPANNING_ARBORESCENCE
 #define OPTFUNCTION max_spanning_arborescence
#endif

#ifdef OPTIMUM_BRANCHING
 #define OPTFUNCTION optimum_branching
#endif

#ifndef HELP_FUNCTIONS
#define HELP_FUNCTIONS

struct edge {
  unsigned tail;
  unsigned head;
};

typedef Sgi::slist< edge > edgelist;
typedef std::vector< edge > edgevector;

template<class Matrix>
double sum(  Matrix  &weights,
	     unsigned               num_vertices,
	     unsigned               root,
	     std::vector<unsigned> &parent )
{
  double s = 0;
  for ( unsigned i = 0; i < num_vertices; ++i )
    {
      if ( i != root ) 
	{
          double w =  weights[parent[i]][i];
	  s += w;
	}
    };
  return s;
}



void 
fillC(edgevector & C, edge & ij, Disjointset & S, std::vector< edge > & enter, unsigned & num_vertices )
{
  C.push_back(ij);
  unsigned k=S.findset(ij.tail);

  while ( enter[k].tail != num_vertices ) 
    {
      C.push_back( enter[k] );
      k=S.findset(enter[k].tail);
    }
  return;
}

bool
pop( edgevector::const_iterator & it, edgevector & newvect, unsigned & representative, Disjointset & S )
{
  bool ret = false;
  if (  S.findset(it->tail) != representative )  
    {
      newvect.push_back( *it );
      ret = true;
    }
  ++it;
  return ret;
}

void
pop2( edgevector::const_iterator & it1, edgevector::const_iterator & it2, edgevector & newvect, unsigned & representative, Disjointset & S )
{
  if ( pop(it1,newvect, representative, S) == true ) 
    {
      ++it2;
    } else
    {
      pop(it2,newvect, representative, S);
    };
}

template<class Matrix>
void 
join( edgevector & vector1, edgevector & vector2, edgevector & newvector, unsigned num_vertices, unsigned & representative , Disjointset & S, Matrix & m )
{
  edgevector::const_iterator it1=vector1.begin();
  edgevector::const_iterator it2=vector2.begin();
  while ( it1 != vector1.end() and it2 != vector2.end() )  
    {
      if (  it1->tail == it2->tail ) 
	{
	  if ( m[(*it1).tail][(*it1).head] >  m[(*it2).tail][(*it2).head] ) {
	    pop2(it1,it2,newvector, representative, S);
	  }
	  else
	    {
	      pop2(it2,it1,newvector, representative, S);
	    };
	} 
      else 
	{
	  if ( it1->tail < it2->tail )
	    {
	      pop(it1,newvector, representative, S);
	    }
	  else
	    {
	      pop(it2,newvector, representative, S);
	    };
	};
    }
  while ( it1 != vector1.end()  )  
    {
      pop(it1,newvector, representative, S);
    };

  while ( it2 != vector2.end()  )  
    {
      pop(it2,newvector, representative, S);
    };
}

struct edgenode {
  edgenode * parent;
  std::list<edgenode *> children;
  edge e;
};

edgenode *
getOrCreate(edge & e,  boost::multi_array< edgenode *,2 >  &  enMatrix, std::vector< edgenode* > & enVector ) 
{
  if ( enMatrix[e.tail][e.head] == NULL ) 
    {
      edgenode * en = new edgenode;
      enMatrix[e.tail][e.head] = en;
      enVector.push_back(en);
      en->e = e;
      return en;
    };
  return enMatrix[e.tail][e.head];
}

void
F_insert( edge & parent, edgevector & children ,   boost::multi_array< edgenode *,2 >  &  enMatrix, std::vector< edgenode* > & enVector )
{
  edgenode * parentnode = getOrCreate(parent,enMatrix,enVector);
  parentnode->parent = NULL;
  for ( edgevector::const_iterator iter = children.begin(); iter != children.end(); ++iter ) 
    {
      edge e = *iter;
      edgenode * childnode = getOrCreate(e,enMatrix,enVector);
      childnode->parent = parentnode;
      parentnode->children.push_front(childnode);
    }
}

#endif

template<typename Real, class Matrix>
void
OPTFUNCTION(const Matrix          &weights,
                          unsigned               num_vertices,
                          unsigned               root,
                          std::vector<unsigned> &parent)
{
  Matrix m(weights);
  Disjointset S(num_vertices), W(num_vertices);
  std::stack<unsigned> roots;
  boost::multi_array< edgenode *,2 > enMatrix(boost::extents[num_vertices][num_vertices]);
  std::vector< edgenode* > enVector;

  edge default_edge = {num_vertices, 0};
  std::vector< edge > lambda(num_vertices, default_edge); 
  std::vector< edge > enter(num_vertices, default_edge); // num_vertices means the empty set
  std::vector< edgevector > I(num_vertices);
  std::vector< edgevector > cycle(num_vertices);

#ifdef OPTIMUM_BRANCHING
  std::vector< unsigned > min(num_vertices);
  std::stack<unsigned> rset;
#endif

  for ( unsigned u = 0; u < num_vertices; ++u ) 
    {
      S.makeset(u); 
      W.makeset(u);
      for ( unsigned v = 0; v < num_vertices; ++v ) 
	{
          I[u].reserve( num_vertices );
          // can we set default earlier???
	  enMatrix[u][v]=NULL;
	  if ( u != v ) 
	    {
	      edge e;
	      e.tail = v;
	      e.head = u;
	      I[u].push_back( e );
	     };
	};

#ifdef MAX_SPANNING_ARBORESCENCE
      if ( u != root ) 
	{ roots.push(u);
	};
#endif

#ifdef OPTIMUM_BRANCHING
	roots.push(u);
	min[u]=u;
#endif

    }
  while ( ! roots.empty() ) 
    {
      unsigned r = roots.top(); roots.pop();

#ifdef OPTIMUM_BRANCHING
      if (I[r].empty())
        {
          rset.push(r); 
          continue;
        }
#endif

      edge ij;

      edgevector::iterator it = I[r].begin();
      Real cost = m[(*it).tail][(*it).head];
      ij = *it;
      ++it;
      while ( it != I[r].end() ) 
	{
	if ( m[(*it).tail][(*it).head] > cost ) 
	  {
	    ij = *it;
            cost = m[(*it).tail][(*it).head] ;
	  };
        ++it;
      };
  
#ifdef OPTIMUM_BRANCHING
      if ( m[ij.tail][ij.head] <= 0 ) 
	{
          rset.push(r); 
          continue;
	}
#endif

      F_insert( ij, cycle[r] , enMatrix, enVector );
      if ( cycle[r].empty() )
	{ 
	  // if cycle[r] = 0 then
	  // lambda j <- pointer that points to the node (i, j) of F

	  lambda[ij.head]=ij;
	};
      if ( W.findset(ij.tail) != W.findset(ij.head) )
	{
	  W.makeunion(W.findset(ij.tail),W.findset(ij.head));
	  enter[r]=ij;
	} 
      else
	{
	  edgevector C;
	  fillC(C,ij,S,enter,num_vertices);

#ifdef OPTIMUM_BRANCHING
          edgevector::const_iterator it5 = C.begin();
	  // C contains at least one
          assert( it5 != C.end() );
          Real mincost =  m[it5->tail][it5->head];
          edge uv = *it5;
          ++it5;
	  while ( it5 != C.end() ) 
	    {
	      if ( m[it5->tail][it5->head] < mincost )
		{
		  mincost =  m[it5->tail][it5->head];
		  uv = *it5;
		}
              ++it5;
	    }
          Real cost1 = m[ij.tail][ij.head] - mincost;
          unsigned min_r = min[S.findset(uv.head)];
#endif

#ifdef MAX_SPANNING_ARBORESCENCE
          Real cost1 =  m[ij.tail][ij.head];
#endif
	  for ( edgevector::const_iterator it2 = I[r].begin(); it2 != I[r].end(); ++it2 ) 
	    {
               m[it2->tail][it2->head] -=  cost1;
	    }

	  for ( edgevector::const_iterator it3 = C.begin(); it3 != C.end(); ++it3 ) 
	    {

#ifdef OPTIMUM_BRANCHING
              if ( uv.tail == it3->tail and uv.head == it3->head ) 
		{ 
		  continue;
		}; 
	      Real cost2 =  m[it3->tail][it3->head] - m[uv.head][uv.tail];
#endif

#ifdef MAX_SPANNING_ARBORESCENCE
	      Real cost2 =  m[it3->tail][it3->head];
#endif
	      unsigned k = S.findset(it3->head);

	      for ( edgevector::const_iterator it4 = I[k].begin(); it4 != I[k].end(); ++it4 ) 
		{
                   m[it4->tail][it4->head] -= cost2;
		};
	      S.makeunion(r,k);
	      unsigned r2 = S.findset(r);

              edgevector vect;
	      join(I[k],I[r],vect,num_vertices,r2,S,m);
	      I[r2].swap(vect);
	      r = r2;
	    }

#ifdef OPTIMUM_BRANCHING
	  min[r]=min_r;
#endif
	  enter[r]=default_edge;
	  roots.push(r);
	  cycle[r].swap(C);
	}
    }

  std::vector< edgenode* > N;

  for ( std::vector< edgenode* >::const_iterator it=enVector.begin(); it != enVector.end(); ++it ) 
    { 
      if ( (*it)->parent == NULL )
	{ 
	  N.push_back( *it );
	};
    };

#ifdef MAX_SPANNING_ARBORESCENCE
  while ( ! N.empty() ) 
    {
      unsigned v;
#endif

#ifdef OPTIMUM_BRANCHING
  std::stack<unsigned> R;
  while ( ! rset.empty() ) 
    {
      unsigned i = rset.top(); rset.pop();
      R.push(min[i]);
    }

      while ( ! N.empty() or ! R.empty() ) 
    {
      unsigned v;
      if ( ! R.empty() ) 
	{
        v = R.top(); R.pop();
	} 
      else 
#endif
      {
      edgenode * n = N.back(); N.pop_back();
      assert( n != NULL );
      v = n->e.head;
      parent[v]=n->e.tail;
      }
      assert( lambda[v].tail >= 0 and lambda[v].tail < num_vertices );
      assert( lambda[v].head >= 0 and lambda[v].head < num_vertices );
      if (lambda[v].tail != num_vertices)
        {
            edgenode * en = enMatrix[lambda[v].tail][lambda[v].head];
            assert( en != NULL );
            std::list<edgenode *> P;

            edgenode * oldnode = NULL;      
            do
                {
                    P.push_front( en );
                    edgenode * parentnode = en->parent;
                    for ( std::list<edgenode *>::iterator it = en->children.begin(); it != en->children.end(); ++it ) 
                        {
                            if ( *it != oldnode ) 
                                {
                                    assert( (*it)->parent != NULL );
                                    (*it)->parent = NULL;              
                                    N.push_back( *it );
                                }
                        };
                    enMatrix[en->e.tail][en->e.head] = NULL;
                    oldnode = en;
                    en = parentnode;
                } while ( en != NULL  );
            for ( std::list<edgenode *>::iterator it = P.begin(); it != P.end(); ++it )  
                {
                    if ((*it)->parent == 0)
                        {
                            for (std::vector<edgenode *>::iterator it2 = N.begin(); it2 != N.end(); ++it2)
                                {
                                    if (*it2 == *it)
                                        {
                                            N.erase(it2);
                                            break;
                                        }
                                }
                        }

                    delete *it;
                };
        }  
    };

#ifdef MAX_SPANNING_ARBORESCENCE
  parent[root]=root;
#endif
}

#undef OPTFUNCTION
