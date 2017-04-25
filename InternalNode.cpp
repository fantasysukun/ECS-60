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
			KeysUpdate();

			if (this->getCount() < internalSize && newNode != NULL) { // ewNode != NULL means Split
				cout << "2 \n";
				AddNewInternalNode(i, newNode);
				return NULL;
			}
			else if (this->getCount() >= internalSize && newNode != NULL) {
				cout << "New add 1\n";
				if (getLeftSibling() != NULL && getLeftSibling()->getCount() < leafSize) { //Checking LeftSibling
					cout << "New add 12\n";
					//ShiftValueToLeft(value);
					return NULL;
				}
				else if (getRightSibling() != NULL && getRightSibling()->getCount() < leafSize) { //Checking RightSibling
					cout << "New add 13\n";
					//ShiftValueToRight(value);
					return NULL;
				}
				else {
					return split(i, newNode); //Split
				}
			}
			return NULL;
		}
		else if (value > children[pos]->getMinimum() ){
			cout << "3 \n";
			BTreeNode *newNode = children[pos]->insert(value);
			KeysUpdate();

			if (this->getCount() < internalSize && newNode != NULL) {
				cout << "4 \n";
				AddNewInternalNode(pos, newNode);
				return NULL;
			}
			else if (this->getCount() >= internalSize && newNode != NULL) {
				cout << "New add 2\n";
				
				if (getLeftSibling() != NULL && getLeftSibling()->getCount() < leafSize) { //Checking LeftSibling
					//ShiftValueToLeft(value);
					return NULL;
				}
				else if (getRightSibling() != NULL && getRightSibling()->getCount() < leafSize) { //Checking RightSibling
					//ShiftValueToRight(value);
					return NULL;
				}
				else {
					return split(pos, newNode); //Split
				}
			}
			return NULL;
		}
	}

	return NULL; // to avoid warnings for now.
} // InternalNode::insert()// students must write this

//update Keys
void InternalNode::KeysUpdate() {
	for (int i = 0; i < this->count; i++) {
		keys[i] = children[i]->getMinimum();
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

/*
InternalNode* InternalNode::split(int value, BTreeNode *newCreatedNode) {

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
	
	//ReplaceNewValueIntoInternalNode(value, newInternalNode);
	
	if (value > children[internalSize - 1]->getMinimum()) {
		cout << "split 1\n";
		
		newInternalNode->insert(value);
		cout << "newInternalNode: " << newInternalNode->getCount() << "/n";
		int NewKeyindex = 1;
		//int NewKeyValues = 0;
		for (int i = (int)ceil(double(leafSize) / 2); i < leafSize; i++) {
			newInternalNode->setChildren(keys[i], NewKeyindex, children[i]);
			setChildren(0, i, NULL);
			count--;
			NewKeyindex++;
		}
	}
	else {
		cout << "split 2\n";
		ReplaceNewValueIntoInternalNode(value, newInternalNode);
	}
	
	return newInternalNode;
}
*/

InternalNode* InternalNode::split(int pos, BTreeNode* newCreatedNode)
{
	InternalNode *newInternalNode = new InternalNode(internalSize, leafSize, parent, this, this->getRightSibling());
	//set all pointers
	InternalNode* originalRight = (InternalNode*)getRightSibling();
	newInternalNode->setParent(parent);
	setRightSibling(newInternalNode);
	if (originalRight == NULL)//there is no node on the right
	{
		newInternalNode->setRightSibling(NULL);
		newInternalNode->setLeftSibling(this);
	}
	else//there is node on the right
	{
		newInternalNode->setRightSibling(originalRight);
		newInternalNode->setLeftSibling(this);
		originalRight->setLeftSibling(newInternalNode);
	}

	int center = (int)ceil((double)internalSize / 2);
	if (pos < center - 1)
	{
		int j = 0;
		//children[center - 1]->getLeftSibling()->setRightSibling(NULL);
		//children[center - 1]->setLeftSibling(NULL);
		for (int i = center - 1; i < internalSize; i++)
		{
			newInternalNode->setChildren(keys[i], j, children[i]);
			setChildren(0, i, NULL);
			j++;
		}
		for (int i = center - 2; i > pos + 1; i--)
		{
			children[i + 1] = children[i];
			keys[i + 1] = keys[i];
		}
		children[pos + 1] = newCreatedNode;
		keys[pos + 1] = newCreatedNode->getMinimum();
		newCreatedNode->setParent(this);
		count++;
	}
	else if (pos == center - 1)//6
	{
		//children[center - 1]->setRightSibling(NULL);
		//newCreatedNode->setLeftSibling(NULL);
		newInternalNode->setChildren(newCreatedNode->getMinimum(), 0, newCreatedNode);
		int j = 1;
		for (int i = center; i < internalSize; i++)
		{
			newInternalNode->setChildren(keys[i], j, children[i]);
			setChildren(0, i, NULL);
			j++;
		}
	}
	else//pos > center - 1. here we need to explictly consider *ret
	{
		int j = 0;
		//children[center - 1]->setRightSibling(NULL);
		//children[center]->setLeftSibling(NULL);
		int i = center;
		for (; i <= pos; i++)
		{
			newInternalNode->setChildren(keys[i], j, children[i]);
			setChildren(0, i, NULL);
			j++;
		}
		newInternalNode->setChildren(newCreatedNode->getMinimum(), j, newCreatedNode);
		j++;
		for (i = pos + 1; i<internalSize; i++)
		{
			newInternalNode->setChildren(keys[i], j, children[i]);
			setChildren(0, i, NULL);
			j++;
		}
	}
	return newInternalNode;
}

//Replace New Value Into LeafNode
void InternalNode::ReplaceNewValueIntoInternalNode(int value, InternalNode* newNode) {
	bool inserted = false;
	int tempCount = (int)ceil(double(internalSize + 1) / 2);
	int NewKeyindex = 0;
	for (int i = internalSize - 1; i >= 0 && tempCount > 0; i--) {
		if (value < children[i]->getMinimum()) {
			newNode->setChildren(keys[i], NewKeyindex, children[i]);
			setChildren(0, i, NULL);
			count--;
			tempCount--;
			NewKeyindex++;
		}
		else {
			newNode->setChildren(keys[i], NewKeyindex, children[i]);
			tempCount--;
			inserted = true;
		}
	}
	if (tempCount == 0 && inserted == false) {
		newNode->insert(value);
	}
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