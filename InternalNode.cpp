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

void InternalNode::AddNewInternalNode(int pos, BTreeNode *newNode) {
	for (int i = count - 1; i > pos; i--){
		children[i + 1] = children[i];
		keys[i + 1] = keys[i];
	}
	children[pos + 1] = newNode; 
	keys[pos + 1] = newNode->getMinimum();
	children[pos + 1]->setParent(this);
	count++;
}

InternalNode* InternalNode::insert(int value)
{
	cout << "insert1 \n";
	int pos = count - 1;
	//cout << "pos: " << pos << " \n";
	for (int i = 0; i <= pos; i++) {
		if (value < children[i]->getMinimum() || value < children[i + 1]->getMinimum() ) {
			cout << "1 \n";
			BTreeNode *newNode = children[i]->insert(value); 
			keys[i] = children[i]->getMinimum();

			if (this->getCount() < internalSize && newNode != NULL) { // ewNode != NULL means Split
				cout << "2 \n";
				AddNewInternalNode(i, newNode);
				return NULL;
			}
			else {
				/*
				if (getLeftSibling() != NULL && getLeftSibling()->getCount() < internalSize) { //Checking LeftSibling
					//ShiftValueToLeft(value);
					return NULL;
				}
				else if (getRightSibling() != NULL && getRightSibling()->getCount() < internalSize) { //Checking RightSibling
					//ShiftValueToRight(value);
					return NULL;
				}
				else {
					return split(value); //Split
				}
				*/
			}
			return NULL;
		}
		else if (value > children[pos]->getMinimum() ){
			cout << "3 \n";
			BTreeNode *newNode = children[pos]->insert(value);
			keys[pos] = children[pos]->getMinimum();

			if (this->getCount() < internalSize && newNode != NULL) {
				cout << "4 \n";
				AddNewInternalNode(pos, newNode);
				return NULL;
			}
			else {
				/*
				if (getLeftSibling() != NULL && getLeftSibling()->getCount() < leafSize) { //Checking LeftSibling
					//ShiftValueToLeft(value);
					return NULL;
				}
				else if (getRightSibling() != NULL && getRightSibling()->getCount() < leafSize) { //Checking RightSibling
					//ShiftValueToRight(value);
					return NULL;
				}
				else {
					return split(value); //Split
				}
				*/
			}
			return NULL;
		}
	}

	return NULL; // to avoid warnings for now.
} // InternalNode::insert()// students must write this

bool InternalNode::CheckChilrenSplitCondition(BTreeNode *newNode) {
	if ((newNode->getLeftSibling() != NULL && newNode->getLeftSibling()->getCount() < internalSize) &&
		(newNode->getRightSibling() != NULL && newNode->getRightSibling()->getCount() < internalSize) ) {
		return true;
	}
	else{
		return false;
	}
}

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{
	cout << "insert2 \n";
	// Node must be the root, and node1
    
	//Setup children
	children[0] = oldRoot;
	//children[0]->setParent(this);
	children[1] = node2;
	//children[1]->setParent(this);

	//Setup keys
	keys[0] = oldRoot->getMinimum();
	keys[1] = node2->getMinimum();

	count = 2;
  // students must write this
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
	cout << "insert3 \n";
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

InternalNode* InternalNode::split(int value) {

	cout << "split \n";
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
	/*
	if (value > values[leafSize - 1]) {
		newInternalNode->insert(value);
		for (int i = (int)ceil(double(leafSize) / 2); i < leafSize; i++) {
			newNode->insert(values[i]);
			count--;
		}
	}
	else {
		ReplaceNewValueIntoLeafNode(value, newNode);
	}
	*/
	return newInternalNode;
}

//Setup Children for the new internalNode
void InternalNode::FirstCase(int pos, InternalNode *newInternalNode, BTreeNode *newCreatedNode) {

	int j = 0;
	for (int i = (int)ceil((double)internalSize) / 2 - 1; i < internalSize; i++) {
		//newInternalNode->setChildren(keys[i], j, children[i]);
		//setChildren(0, i, NULL);
		newInternalNode->keys[pos] = keys[i];
		newInternalNode->children[pos] = children[i];
		if (newInternalNode->children[i] == NULL) count--;
		else {
			children[i]->setParent(this);
			count++;
		}
		keys[i] = 0; children[i] = NULL; count--;
		j++;
	}
	for (int i = (int)ceil((double)internalSize) / 2 - 2; i > pos + 1; i--) {
		children[i + 1] = children[i];
		keys[i + 1] = keys[i];
	}
	children[pos + 1] = newCreatedNode;
	keys[pos + 1] = newCreatedNode->getMinimum();
	newCreatedNode->setParent(this);
	count++;
}

//Setup Children for the new internalNode
void InternalNode::SecendCase(int pos, InternalNode *newInternalNode, BTreeNode *newCreatedNode) {
	newInternalNode->setChildren(newCreatedNode->getMinimum(), 0, newCreatedNode);
	int j = 1;
	for (int i = (int)ceil((double)internalSize) / 2; i < internalSize; i++) {
		newInternalNode->setChildren(keys[i], j, children[i]);
		setChildren(0, i, NULL);
		j++;
	}
}

//Setup Children for the new internalNode
void InternalNode::ThirdCase(int pos, InternalNode *newInternalNode, BTreeNode *newCreatedNode)
{
	int j = 0;
	for (int i = (int)ceil((double)internalSize) / 2; i <= pos; i++) {
		newInternalNode->setChildren(keys[i], j, children[i]);
		setChildren(0, i, NULL);
		j++;
	}

	newInternalNode->setChildren(newCreatedNode->getMinimum(), j, newCreatedNode);
	j++;
	for (int i = pos + 1; i<internalSize; i++) {
		newInternalNode->setChildren(keys[i], j, children[i]);
		setChildren(0, i, NULL);
		j++;
	}
}