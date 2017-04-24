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

void InternalNode::addInMid(int pos, BTreeNode *newNode) {
	for (int i = count - 1; i > pos; i--){
		children[i + 1] = children[i];
		keys[i + 1] = children[i + 1]->getMinimum();
	}
	children[pos + 1] = newNode;
	keys[pos + 1] = children[pos + 1]->getMinimum();
	count++;
}

InternalNode* InternalNode::insert(int value)
{
	//cout << "insert1 \n";
	int pos = count - 1;
	while(value <= keys[pos] && pos > 0) //Find the right pos for insert value
    {
        --pos;
    }
	BTreeNode *newNode = children[pos]->insert(value);
	keys[pos] = children[pos]->getMinimum();
	if(newNode){
		if(count < internalSize){ //Need to add new internalnode
			addInMid(pos, newNode);
		}
	}
	else{ //If count >= internalsize, pass to left or right with split

	}


	return NULL; // to avoid warnings for now.
} // InternalNode::insert()// students must write this

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{
	cout << "insert2 \n";
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

InternalNode* InternalNode::split(int pos, BTreeNode* newCreatedNode) {

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
	if (pos < (int)ceil( (double)internalSize) / 2 - 1) {
		FirstCase(pos, newInternalNode, newCreatedNode);
	}
	else if (pos == (int)ceil((double)internalSize) / 2 - 1) {
		SecendCase(pos, newInternalNode, newCreatedNode);
	}
	else {
		ThirdCase(pos, newInternalNode, newCreatedNode);
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