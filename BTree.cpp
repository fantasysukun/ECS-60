#include <iostream>
#include "BTree.h"
#include "BTreeNode.h"
#include "LeafNode.h"
#include "InternalNode.h"
using namespace std;

BTree::BTree(int ISize, int LSize):internalSize(ISize), leafSize(LSize)
{
  root = new LeafNode(LSize, NULL, NULL, NULL);
} // BTree::BTree()


void BTree::insert(const int value)
{
	BTreeNode* newNode = root->insert(value);
	if (newNode != NULL)
	{
		//BTreeNode *oldRoot = root;
		//Need to change logic 
		//InternalNode *newRoot = new InternalNode(internalSize, leafSize, NULL, NULL, NULL);
		//newRoot->insert(oldRoot, newNode);//set new parent's property
		//oldRoot->setParent(newRoot);//set child point to parent
		//newNode->setParent(newRoot);
		//root = newRoot;
		//Need to change logic 
	}
  // students must write this
} // BTree::insert()


void BTree::print()
{
  BTreeNode *BTreeNodePtr;
  Queue<BTreeNode*> queue(1000);

  queue.enqueue(root);
  while(!queue.isEmpty())
  {
    BTreeNodePtr = queue.dequeue();
    BTreeNodePtr->print(queue);
  } // while
} // BTree::print()
