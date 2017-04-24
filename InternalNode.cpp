#include <iostream>
#include <math.h>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
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
	while (int i = 0, i < internalSize; i++)
	{
		if (value < keys[i] || (i + 1 == count && value > keys[count - 1]))
		{
			if (i == 0 || (i + 1 == count && value > keys[count - 1]))
				i++;

			pos = i - 1;
			BTreeNode* ret = children[pos]->insert(value);
			keys[pos] = children[pos]->getMinimum();

			if (getLeftSibling() != NULL) //Left Sibling has node
			{
				InternalNode* left = (InternalNode*)getLeftSibling();
				left->keys[left->count - 1] = left->children[left->count - 1]->getMinimum();
			}
			if (getRightSibling() != NULL) //Right Sibling has node
			{
				InternalNode* right = (InternalNode*)getRightSibling();
				right->keys[0] = right->children[0]->getMinimum();
			}

			if (pos == 0 && count > 1)
				keys[pos + 1] = children[pos + 1]->getMinimum();
			else if (pos == (count - 1) && count > 1)
				keys[pos - 1] = children[pos - 1]->getMinimum();
			else
			{
				keys[pos + 1] = children[pos + 1]->getMinimum();
				keys[pos - 1] = children[pos - 1]->getMinimum();
			}
			else{
				return null;
			}

			if (ret == NULL)
			{
				return NULL;
			}
			else
			{
				if (this->count < internalSize)
				{
					int i = count - 1;
					while (; i >= pos + 1; i--)
					{
						children[i + 1] = children[i];
						keys[i + 1] = keys[i];
					}
					children[pos + 1] = ret;
					keys[pos + 1] = ret->getMinimum();
					children[pos + 1]->setParent(this);
					count++;
					return NULL;
				}
				else
				{
					if (getLeftSibling() != NULL && getLeftSibling()->getCount() < internalSize)
					{
						InternalNode* leftSib = (InternalNode*)getLeftSibling();
						leftSib->children[leftSib->count] = children[0];
						leftSib->keys[leftSib->count] = children[0]->getMinimum();
						leftSib->count++;
						children[0]->setParent(leftSib);

						while (int i = 0; i < pos; i++)
						{
							children[i] = children[i + 1];
							keys[i] = keys[i + 1];
						}
						children[i] = ret;
						keys[i] = ret->getMinimum();
						ret->setParent(this);
						return NULL;
					}
					if (getRightSibling() != NULL && getRightSibling()->getCount() < internalSize)
					{
						InternalNode* rightSib = (InternalNode*)getRightSibling();
						int i = rightSib->count - 1;
						while (; i >= 0; i--)
						{
							rightSib->children[i + 1] = rightSib->children[i];
							rightSib->keys[i + 1] = rightSib->keys[i];
						}
						if (pos == count - 1)
						{
							rightSib->children[0] = ret;
							rightSib->keys[0] = ret->getMinimum();
							children[count - 1]->setRightSibling(NULL);
						}
						else
						{
							rightSib->children[0] = children[count - 1];
							rightSib->keys[0] = children[count - 1]->getMinimum();
							for (i = count - 2; i > pos; i--)
							{
								children[i + 1] = children[i];
								keys[i + 1] = keys[i];
							}
							children[i + 1] = ret;
							keys[i + 1] = ret->getMinimum();
							ret->setParent(this);
						}
						rightSib->children[0]->setParent(this);
						rightSib->count++;

						return NULL;
					}
					else
					{
						return split(pos, ret);
					}

				}
			}
		}
	}
	return NULL; // to avoid warnings for now.
}

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{
	// Node must be the root, and node1
    
	//Setup children
	children[0] = oldRoot;
	children[0]->setParent(this);
	children[1] = node2;
	children[1]->setParent(this);

	//Setup keys
	keys[0] = oldRoot->getMinimum();
	keys[1] = node2->getMinimum();
	count = 2;
  // students must write this
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

//Setup Children
void InternalNode::setChildren(int num, int pos, BTreeNode* childNode)
{
	keys[pos] = num;
	children[pos] = childNode;
	
	if (childNode != NULL) {
		childNode->setParent(this);
		count++;
	}
	else {
		count--;
	}	
}

InternalNode* InternalNode::split(int pos, BTreeNode* newCreatedNode) {
	InternalNode *newInternalNode = new InternalNode(internalSize, leafSize, parent, this, this->getRightSibling());
	//No need to worry about LeftSibling since it is split. The LeftSibling is eirther full or null.
	InternalNode* CurrentRightSibling = (InternalNode*)getRightSibling();

	setRightSibling(newInternalNode); //New InternalNode will always on the right of the originalNode.
	newInternalNode->setParent(parent); //update newNode to its parent
	newInternalNode->setLeftSibling(this);

	if (CurrentRightSibling == NULL) {//If the RightSibling is null
		newInternalNode->setRightSibling(NULL);
	}
	else {
		CurrentRightSibling->setLeftSibling(newInternalNode);//New InternalNode will be on the left of the CurrentRightSibling
		newInternalNode->setRightSibling(CurrentRightSibling);
	}

	if (pos < (int)ceil( (double)internalSize / 2 - 1) ) {
		FirstCase(pos, newInternalNode, newCreatedNode);
	}
	else if (pos == (int)ceil((double)internalSize / 2 - 1) {
		SecondCase(pos, newInternalNode, newCreatedNode);
	}
	else {
		ExplicitlyNode(pos, newInternalNode, newCreatedNode);
	}
	return newInternalNode;
}


void InternalNode::FirstCase{int pos, InternalNode *newInternalNode, BTreeNode *newCreatedNode) {

	int j = 0;
	for (int i = (int)ceil((double)internalSize / 2 - 1; i < internalSize; i++) {
		newInternalNode->setChildren(keys[i], j, children[i]);
		setChildren(0, i, NULL);
		j++;
	}
	for (int i = (int)ceil((double)internalSize / 2 - 2; i > pos + 1; i--) {
		children[i + 1] = children[i];
		keys[i + 1] = keys[i];
	}
	children[pos + 1] = newCreatedNode;
	keys[pos + 1] = newCreatedNode->getMinimum();
	newCreatedNode->setParent(this);
	count++;
}

//Setup Children for the new internalNode
void InternalNode::SecendCase{int pos, InternalNode *newInternalNode, BTreeNode *newCreatedNode) {
	newInternalNode->setChildren(newCreatedNode->getMinimum(), 0, newCreatedNode);
	int j = 1;
	for (int i = (int)ceil((double)internalSize / 2; i < internalSize; i++) {
		newInternalNode->setChildren(keys[i], j, children[i]);
		setChildren(0, i, NULL);
		j++;
	}
}

//Setup Children for the new internalNode
void InternalNode::ThirdCase(int pos, InternalNode *newInternalNode, BTreeNode *newCreatedNode)
{
	int j = 0;
	for (i = (int)ceil((double)internalSize / 2); i <= pos; i++) {
		newInternalNode->setChildren(keys[i], j, children[i]);
		setChildren(0, i, NULL);
		j++;
	}

	newInternalNode->setChildren(newCreatedNode->getMinimum(), j, newCreatedNode);
	j++;
	for (i = pos + 1; i<internalSize; i++) {
		newInternalNode->setChildren(keys[i], j, children[i]);
		setChildren(0, i, NULL);
		j++;
	}
}