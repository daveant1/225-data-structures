/* Your code here! */
#include <vector>
#include <cstdlib>
#include "dsets.h"

using namespace std;

void DisjointSets::addelements(int num){
  for(int i = 0; i < num; ++i){
    sets_.push_back(-1);
  }
}

int DisjointSets::find(int elem){
  if(sets_[elem] < 0)
    return elem;
  else{
    int root = find(sets_[elem]);
    sets_[elem] = root;
    return root;
  }
}

void DisjointSets::setunion(int a, int b){
  //inputs a and b may not be roots to uptrees, so we must check
  int root1 = a;
  int root2 = b;
  if(sets_[a] >= 0)
    root1 = find(a);
  if(sets_[b] >= 0)
    root2 = find(b);
  if(root1==root2){ //if elements already part of same set, we do nothing
    return;
  }
  if(abs(sets_[root2]) <= abs(sets_[root1])){
    sets_[root1] = sets_[root1] + sets_[root2];
    sets_[root2] = root1;
  }
  else{
    sets_[root2] = sets_[root1] + sets_[root2];
    sets_[root1] = root2;
  }
}

int DisjointSets::size(int elem){
  int root = find(elem);
  return (abs(sets_[root]));
}
