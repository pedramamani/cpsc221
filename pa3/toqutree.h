
/**
 *
 * toqutree (pa3)
 * slight modification of a quad tree wherein the
 * splitting location can vary across the region.
 */

#ifndef _TOQUTREE_H_
#define _TOQUTREE_H_

#include <utility>
#include <cmath>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "stats.h"

using namespace std;
using namespace cs221util;


class toqutree {
private:
   class Node {
   public:
      Node(pair<int,int> ctr, int dim, HSLAPixel a);

      pair<int,int> center;
      int dimension;
      HSLAPixel avg;

      Node * NW;
      Node * NE;
      Node * SE;
      Node * SW;

   };


public:
   ~toqutree();
   toqutree(const toqutree & other);
   toqutree & operator=(const toqutree & rhs);
   toqutree(PNG & imIn, int k);

   PNG render();
   void prune(double tol);
   int size();


private:
   Node* root;

   void clear(Node * &croot);
   Node * copy(const Node * croot);
   Node * buildTree(PNG & im, int k);

   int recSize(Node * root);
   PNG & subImage(PNG & im, pair<int, int> start, int k);
   double avgEntropy(pair<int, int> center, int k, stats* info);

   void recRender(Node* sRoot, vector<vector<HSLAPixel*>> & pointers);

   void pruneNode(Node * curNode, double tol);
   bool withinTol (Node * curNode, HSLAPixel avg, double tol);
};

#endif
