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

//Add new internalNode
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
	int pos = count - 1;
	for (int i = 0; i <= pos; i++) {
		if (value < children[i]->getMinimum() || value < children[i + 1]->getMinimum() ) {
			return AddvalueIntoInternalNode(i, i, value);
		}
		else if (value > children[pos]->getMinimum() ){
			return AddvalueIntoInternalNode(i, pos, value);
		}
	}
	return NULL; // to avoid warnings for now.
} // InternalNode::insert()// students must write this

//Add new Value into InternalNode
InternalNode* InternalNode::AddvalueIntoInternalNode(int i, int pos, int value) {
	BTreeNode *newNode = children[pos]->insert(value);
	KeysUpdate();

	if (this->getCount() < internalSize && newNode != NULL) {
		AddNewInternalNode(pos, newNode);
		return NULL;
	}
	else if (this->getCount() >= internalSize && newNode != NULL) {

		if (getLeftSibling() != NULL && getLeftSibling()->getCount() < internalSize) { //Checking LeftSibling
			ShiftValueToLeft(pos, newNode);
			return NULL;
		}
		else if (getRightSibling() != NULL && getRightSibling()->getCount() < internalSize) { //Checking RightSibling
			ShiftValueToRight(pos, newNode);
			return NULL;
		}
		else {
			return split(pos, newNode); //Split
		}
	}
	return NULL;
}

//Shift the Value to the Right Sibling
void InternalNode::ShiftValueToRight(int pos, BTreeNode *newNode) {

	InternalNode* rightSibing = (InternalNode*)getRightSibling();
	int index = rightSibing->count - 1;
	for (; index >= 0; index--) {
		rightSibing->children[index + 1] = rightSibing->children[index];
		rightSibing->keys[index + 1] = rightSibing->keys[index];
	}
	if (pos == count - 1) {
		rightSibing->children[0] = newNode;
		rightSibing->keys[0] = newNode->getMinimum();
		children[count - 1]->setRightSibling(NULL);
	}
	else {
		rightSibing->children[0] = children[count - 1];
		rightSibing->keys[0] = children[count - 1]->getMinimum();
		for (index = count - 2; index > pos; index--)
		{
			children[index + 1] = children[index];
			keys[index + 1] = keys[index];
		}
		children[index + 1] = newNode;
		keys[index + 1] = newNode->getMinimum();
		newNode->setParent(this);
	}

	rightSibing->children[0]->setParent(this);
	rightSibing->count++;
}

//Shift the Value to the Left Sibling
void InternalNode::ShiftValueToLeft(int pos, BTreeNode *newNode) {

	InternalNode* leftSibing = (InternalNode*)getLeftSibling();
	leftSibing->children[leftSibing->count] = children[0];
	leftSibing->keys[leftSibing->count] = children[0]->getMinimum();
	leftSibing->count++;
	children[0]->setParent(leftSibing);

	for (int i = 0; i < pos; i++) //Reset current children and keys
	{
		children[i] = children[i + 1];
		keys[i] = keys[i + 1];
	}

	//add newNode into internalNode
	children[pos] = newNode;
	keys[pos] = newNode->getMinimum();
	newNode->setParent(this);
}

//update Keys
void InternalNode::KeysUpdate() {
	for (int i = 0; i < this->count; i++) {
		keys[i] = children[i]->getMinimum();
	}
}

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{
	//Setup children
	children[0] = oldRoot;
	children[1] = node2;

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

InternalNode* InternalNode::split(int pos, BTreeNode* newCreatedNode)
{
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

	if (pos < (int)ceil((double)internalSize / 2) - 1)
	{
		FirstCase(pos, newInternalNode, newCreatedNode);
	}
	else if (pos == (int)ceil((double)internalSize / 2) - 1)
	{
		SecendCase(pos, newInternalNode, newCreatedNode);
	}
	else
	{
		ThirdCase(pos, newInternalNode, newCreatedNode);
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
void InternalNode::FirstCase(int pos, InternalNode *newInternalNode, BTreeNode *newNode) {

	int Newindex = 0;
	for (int i = (int)ceil((double)internalSize / 2) - 1; i < internalSize; i++)
	{
		newInternalNode->setChildren(keys[i], Newindex, children[i]);
		keys[i] = 0; children[i] = NULL; count--;
		Newindex++;
	}

	for (int i = (int)ceil((double)internalSize / 2) - 2; i > pos + 1; i--)
	{
		children[i + 1] = children[i];
		keys[i + 1] = keys[i];
	}

	children[pos + 1] = newNode;
	keys[pos + 1] = newNode->getMinimum();
	newNode->setParent(this);
	count++;
}

//Setup Children for the new internalNode
void InternalNode::SecendCase(int pos, InternalNode *newInternalNode, BTreeNode *newNode) {

	newInternalNode->setChildren(newNode->getMinimum(), 0, newNode);
	int newindex = 1;
	for (int i = (int)ceil((double)internalSize / 2); i < internalSize; i++)
	{
		newInternalNode->setChildren(keys[i], newindex, children[i]);
		keys[i] = 0; children[i] = NULL; count--;
		newindex++;
	}
}

//Setup Children for the new internalNode
void InternalNode::ThirdCase(int pos, InternalNode *newInternalNode, BTreeNode *newNode)
{

	int newindex = 0;
	for (int i = (int)ceil((double)internalSize / 2); i <= pos; i++)
	{
		newInternalNode->setChildren(keys[i], newindex, children[i]);
		keys[i] = 0; children[i] = NULL; count--;
		newindex++;
	}
	newInternalNode->setChildren(newNode->getMinimum(), newindex, newNode);
	newindex++;
	for (int i = pos + 1; i<internalSize; i++)
	{
		newInternalNode->setChildren(keys[i], newindex, children[i]);
		keys[i] = 0; children[i] = NULL; count--;
		newindex++;
	}
}
