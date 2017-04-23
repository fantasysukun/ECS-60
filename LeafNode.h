#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  void print(Queue <BTreeNode*> &queue);

  LeafNode* Split(int);
  void SortedArray();
  void quickSort(int*, int, int);
  void ShiftValueToLeft(int);
  void ShiftValueToRight(int);
  void DeleteFirstNodeFromLeaf();
  void DeleteLastNodeFromLeaf(int);
}; //LeafNode class

#endif
