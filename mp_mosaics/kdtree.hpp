/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
     if(first[curDim] < second[curDim]){
       return true;
     }
     if(first[curDim] == second[curDim]){
       return (first<second);
     }
     return false;
}

template<int Dim>
int KDTree<Dim>::distance(const Point<Dim>& init, const Point<Dim>& target) const{
  int dist = 0;
  for(int i =0; i < Dim; i++){
   int add = (init[i] - target[i])*(init[i] - target[i]);
   dist+=add;
  }
  return dist;
}
template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
    double pot_dist = distance(potential, target);
    double curr_dist = distance(currentBest, target);

    if(pot_dist < curr_dist)
      return true;
    if(curr_dist == pot_dist){
      if(potential < currentBest)
        return true;
      else
        return false;
    }
    return false;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
     if(newPoints.empty()){
       root=NULL;
       return;
     }
     vector<Point<Dim>> copy;
     for(unsigned i =0; i<newPoints.size(); ++i){
       copy.push_back(newPoints[i]);
     }
     root = construct(copy, 0, newPoints.size()-1, 0);

}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& points, int a, int b, int currDim){
  Point<Dim> piv = points[b];
  int i = a-1;
  for(int j = a; j < b; ++j){
    if(smallerDimVal(points[j], piv, currDim)){
      ++i;
      swap(points[i], points[j]);
    }
  }
  ++i;
  swap(points[i], points[b]);
  return i;
}

template<int Dim>
int KDTree<Dim>::quickselect(vector<Point<Dim>>& points, int a, int b, int k, int currDim){
  if(a==b){
    return a;
  }
  int pivot = partition(points, a, b, currDim);
  if(pivot - a == k-1){
    return pivot;
  }
  else if(pivot-a>k-1){
			return quickselect(points,a,pivot-1,k,currDim);
	}
  else{
			return quickselect(points,pivot+1,b,k-pivot+a-1,currDim);
	}
}

template<int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::construct(vector<Point<Dim>>& points, int a, int b, int currDim){
  if(a>b){
    return NULL;
  }
  int k = (int)((b-a)/2 + 1);
  int med = quickselect(points, a, b, k, currDim);
  KDTreeNode* internal = new KDTreeNode(points[med]);
  internal->left = construct(points, a, med-1, (currDim+1)%Dim);
  internal->right = construct(points, med+1, b, (currDim+1)%Dim);

  return internal;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
   this->root=copy(other.root);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copy(KDTreeNode* subRoot){
  if(subRoot == NULL){
    return NULL;
  }
  KDTreeNode* newroot = new KDTreeNode(subRoot);
  newroot->left = copy(subRoot->left);
  newroot->right = copy(subRoot->right);
  return newroot;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  KDTreeNode* newroot = copy(rhs.root);
  this->destroy(root);
  root = newroot;
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
   destroy(root);
}

template <int Dim>
void KDTree<Dim>::destroy(KDTreeNode* subRoot){
  if(subRoot == NULL){
    return;
  }
  destroy(subRoot->left);
  destroy(subRoot->right);
  delete subRoot;
  subRoot = NULL;
}


template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
     if(root==NULL){
       return Point<Dim>();
     }
     return findhelper(query, root, 0);
}


template <int Dim>
Point<Dim> KDTree<Dim>::findhelper(const Point<Dim>& query, KDTreeNode* root, int currDim) const{
    if(root==NULL){
      return Point<Dim>();
    }
    if(root->left == NULL && root->right==NULL){
      return root->point;
    }
    bool leftRecurse = false;
    Point<Dim> curr = root->point;
    Point<Dim> curr_best = root->point;
    if(smallerDimVal(query, root->point, currDim)){
      if(root->left!=NULL){
        curr = findhelper(query, root->left, (currDim+1)%Dim);
        leftRecurse = true;
      }
    }
    else{
      if(root->right!=NULL){
        curr = findhelper(query, root->right, (currDim+1)%Dim);
        leftRecurse = false;
      }
    }
    //recursive backtracking included
    curr_best = curr;
    int curr_dist = distance(query, curr);

    //the recursive backtracking aspect of this algorithm
    if(shouldReplace(query, curr_best, root->point)){
      curr_best = root->point;
      curr_dist = distance(query, curr_best);
    }
    //final check to see if there are other points in other partitions that could be closer
    if(query[currDim] - std::sqrt(curr_dist) <= root->point[currDim] && query[currDim] + std::sqrt(curr_dist) >= root->point[currDim]){
      Point<Dim> newcurr;
      //if we left recursed, we must now recurse right
      if(leftRecurse && root->right != NULL){
        newcurr = findhelper(query, root->right, (currDim+1)%Dim);
        if(shouldReplace(query, curr_best, newcurr)){
          curr_best = newcurr;
          curr_dist = distance(curr_best, query);
        }
      }
      //if we right recursed, we now must recurse left
      if(!leftRecurse && root->left != NULL){
        newcurr = findhelper(query, root->left, (currDim+1)%Dim);
        if(shouldReplace(query, curr_best, newcurr)){
          curr_best = newcurr;
          curr_dist = distance(curr_best, query);
        }
      }
    }
    return curr_best;
}
