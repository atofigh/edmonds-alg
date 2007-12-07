#ifndef DISJOINTSET_HPP
#define DISJOINTSET_HPP
#include <vector>

class Disjointset {
private: 
  std::vector<unsigned> parent;
  std::vector<unsigned> rank;
  void link(unsigned x, unsigned y);
public:
  Disjointset() {};
  Disjointset(unsigned size);
  ~Disjointset() {};
  void makeset(unsigned x);  
  void makeunion(unsigned x,unsigned y);
  unsigned findset(unsigned x);
};

Disjointset::Disjointset( unsigned size ) {
  parent.resize( size );
  rank.resize( size );
}

void Disjointset::makeset(unsigned x) {
  parent[x]=x;
  rank[x]=0;
}

void Disjointset::makeunion(unsigned x,unsigned y) {
  link(findset(x),findset(y));
  return;
}

unsigned Disjointset::findset(unsigned x) {
  if ( x != parent[x] ) {
    parent[x]=findset(parent[x]);
  };
  return parent[x];
}

void Disjointset::link(unsigned x, unsigned y) {
  if ( rank[x] > rank[y] ) {
    parent[y]=x;
  } 
  else {
    parent[x]=y;
    if ( rank[x] == rank[y] ) {
      rank[y]++;
    }
  };
  return;
}

#endif /* DISJOINTSET_HPP */
