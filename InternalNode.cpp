#include <iostream>
#include "InternalNode.h"
#include <math.h>

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];//array of pointers pointing to each child
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)//This function insert the value to internal nodes
{
	int i = 0;
	int k = 0;
	for(; i < internalSize; i++)
	{
		if(value < keys[i] || (i+1 == count && value > keys[count - 1]))
		{
			if(i == 0 || (i+1 == count && value > keys[count - 1]))//problematic
				i++;
			k = i - 1;
			BTreeNode* ret = children[k]->insert(value);
			keys[k] = children[k]->getMinimum();
			if(getLeftSibling() != NULL)
			{
				InternalNode* left = (InternalNode*)getLeftSibling();
				left->keys[left->count-1] = left->children[left->count-1]->getMinimum();
			}
			if(getRightSibling() != NULL)
			{
				InternalNode* right = (InternalNode*)getRightSibling();
				right->keys[0] = right->children[0]->getMinimum();
			}
			if(k == 0 && count > 1)
				keys[k+1] = children[k+1]->getMinimum();
			else if(k==(count-1) && count > 1)
				keys[k-1] = children[k-1]->getMinimum();
			else if(count > 1)
			{
				keys[k+1] = children[k+1]->getMinimum();
				keys[k-1] = children[k-1]->getMinimum();
			}
			else
			{
				//do nothing
			}
			if(ret == NULL)//no plit
			{
				return NULL;
			}
			else//in the end of this recursion, not null means the leafnode splits.
			{
				if(this->count < internalSize)//this part means creat a new leafnode and attached to the parent/parent not splitting
				{
					int i=count - 1;
					for(;i >= k+1;i--)
					{
						children[i+1]=children[i];
						keys[i+1]=keys[i];
					}
					children[k+1] = ret;
					keys[k+1] = ret->getMinimum();
					children[k+1]->setParent(this);
					count ++;
					return NULL;
				}
				else//split parent/ or give children for adoption
				{
					if(getLeftSibling() != NULL && getLeftSibling()->getCount() < internalSize)//left sibling can adopt
					{
						InternalNode* leftSib = (InternalNode*)getLeftSibling();
						leftSib->children[leftSib->count] = children[0];
						leftSib->keys[leftSib->count] = children[0]->getMinimum();
						leftSib->count++;
						children[0]->setParent(leftSib);
						//children[0]->setRightSibling(NULL);
						//children[0]->setLeftSibling(leftSib->children[leftSib->count-2]);
						//leftSib->children[leftSib->count-2]->setRightSibling(children[0]);
						int i = 0;
						for(; i < k; i++)
						{
							children[i] = children[i + 1];
							keys[i] = keys[i + 1];
						}
						//children[0]->setLeftSibling(NULL);
						children[i] = ret;
						keys[i] = ret->getMinimum();
						ret->setParent(this);
						//ret->setRightSibling(children[i+1]);
						//ret->setLeftSibling(children[i-1]);
						//if(i >= 1)
							//children[i-1]->setRightSibling(ret);
						//if(i+1 < count)
							//children[i+1]->setLeftSibling(ret);
						return NULL;
					}
					else
					{//end here
						if(getRightSibling() != NULL && getRightSibling()->getCount() < internalSize)
						{
							InternalNode* rightSib = (InternalNode*)getRightSibling();
							int i = rightSib->count - 1;
							for(; i >= 0; i--)
							{
								rightSib->children[i + 1] = rightSib->children[i];
								rightSib->keys[i + 1] = rightSib->keys[i];
							}
							if(k == count -1)
							{
								rightSib->children[0] = ret;
								rightSib->keys[0] = ret->getMinimum();
								children[count-1]->setRightSibling(NULL);
							}
							else
							{
								rightSib->children[0] = children[count-1];
								rightSib->keys[0] = children[count-1]->getMinimum();
								for(i = count - 2; i > k; i--)
								{
									children[i+1] = children[i];
									keys[i+1] = keys[i];
								}
								children[i+1] = ret;
								keys[i+1] = ret->getMinimum();
								ret->setParent(this);
							}
							//rightSib->children[0]->setRightSibling(rightSib->children[1]);
							//rightSib->children[0]->setLeftSibling(NULL);
							rightSib->children[0]->setParent(this);
							rightSib->count++;

							//count++;
							return NULL;
						}
						else//now split the parent node
						{
							return split(k, ret);
						}
					}
				}
			}
		}
	}
  return NULL; // to avoid warnings for now.
} // InternalNode::insert()// students must write this

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{
	// Node must be the root, and node1
    // students must write this
	children[0]=oldRoot;
	children[0]->setParent(this);
	children[1]=node2;
	children[1]->setParent(this);
	keys[0] = oldRoot->getMinimum();
	keys[1] = node2->getMinimum();
	count = 2;
	
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
	children[count] = newNode;
	count++;
	// students may write this
} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()

void InternalNode::setChildren(int num, int pos, BTreeNode* childNode)
{
	keys[pos] = num;
	children[pos] = childNode;
	
	if(childNode == NULL)
		count--;
	else
	{
		childNode->setParent(this);
		count++;
	}	
}


void InternalNode::ExplicitlyNode(int pos, InternalNode* newInternalNode, BTreeNode* newCreatedNode)
{
		int center = (int)ceil((double)internalSize/2);
	    int j = 0;
		int i = center;
		for(; i <= pos; i++)
		{
			newInternalNode->setChildren(keys[i], j, children[i]);
			setChildren(0, i, NULL);
			j++;
		}
		newInternalNode->setChildren(newCreatedNode->getMinimum(), j, newCreatedNode);
		j++;
		for(i=pos+1;i<internalSize;i++)
		{
			newInternalNode->setChildren(keys[i], j, children[i]);
			setChildren(0, i, NULL);
			j++;
		}
}

InternalNode* InternalNode::split(int pos, BTreeNode* newCreatedNode)
{
	InternalNode *newInternalNode = new InternalNode(internalSize, leafSize, parent, this, this->getRightSibling());
	//No need to worry about LeftSibling since it is split. The LeftSibling is eirther full or null.
	InternalNode* originalRight = (InternalNode*)getRightSibling();
	newInternalNode->setParent(parent);
	setRightSibling(newInternalNode);
	if(originalRight == NULL)//If the RightSibling is null
	{
		newInternalNode->setRightSibling(NULL);
		newInternalNode->setLeftSibling(this);
	}
	else//New InternalNode will be on the left of the CurrentRightSibling
	{
		newInternalNode->setRightSibling(originalRight);
		newInternalNode->setLeftSibling(this);
		originalRight->setLeftSibling(newInternalNode);
	}

	int center = (int)ceil((double)internalSize/2);
	if(pos < center - 1)
	{
		int j = 0;
		for(int i = center - 1; i < internalSize; i++)
		{
			newInternalNode->setChildren(keys[i], j, children[i]);
			setChildren(0, i, NULL);
			j++;
		}
		for(int i = center -2; i > pos+1; i--)
		{
			children[i+1] = children[i];
			keys[i+1] = keys[i];
		}
		children[pos+1] = newCreatedNode;
		keys[pos+1]=newCreatedNode->getMinimum();
		newCreatedNode->setParent(this);
		count++;
	}
	else if(pos == center - 1)//6
	{
		newInternalNode->setChildren(newCreatedNode->getMinimum(), 0, newCreatedNode);
		int j = 1;
		for(int i = center; i < internalSize; i++)
		{
			newInternalNode->setChildren(keys[i], j, children[i]);
			setChildren(0, i, NULL);
			j++;
		}
	}
	else//pos > center - 1. here we need to explictly consider *ret
	{
		/*int j = 0;
		int i = center;
		for(; i <= pos; i++)
		{
			newInternalNode->setChildren(keys[i], j, children[i]);
			setChildren(0, i, NULL);
			j++;
		}
		newInternalNode->setChildren(newCreatedNode->getMinimum(), j, newCreatedNode);
		j++;
		for(i=pos+1;i<internalSize;i++)
		{
			newInternalNode->setChildren(keys[i], j, children[i]);
			setChildren(0, i, NULL);
			j++;
		}*/
		ExplicitlyNode(pos, newInternalNode, newCreatedNode);
	}
	return newInternalNode;
}

