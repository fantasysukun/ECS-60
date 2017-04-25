#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public://
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  int getMinimum()const;
  InternalNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);

  //added
  bool CheckChilrenSplitCondition(BTreeNode *newNode);
  void AddNewInternalNode(int, BTreeNode*);
  InternalNode* split(int);

  void setChildren(int, int, BTreeNode*);
  
  void FirstCase(int, InternalNode*, BTreeNode*);
  void SecendCase(int, InternalNode*, BTreeNode*);
  void ThirdCase(int, InternalNode*, BTreeNode*);
  
}; // InternalNode class

#endif
