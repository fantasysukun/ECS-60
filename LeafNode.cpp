#include <iostream>
#include <math.h>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
	if (count < leafSize) {
		values[count] = value;
		count++;
		SortedArray();
		return NULL;
	}
	else {
		if (getLeftSibling() != NULL && getLeftSibling()->getCount() < leafSize) { //Checking LeftSibling
			ShiftValueToLeft(value);
			return NULL;
		}
		else if (getRightSibling() != NULL && getRightSibling()->getCount() < leafSize) { //Checking RightSibling
			ShiftValueToRight(value);
			return NULL;
		}
		else {
			//cout << "good";
			return Split(value); //Split
		}
	}
  // students must write this
  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

//Split
LeafNode* LeafNode::Split(int value) {
	
	
	LeafNode* newNode = new LeafNode(leafSize, NULL, NULL, NULL);
	//No need to worry about LeftSibling since it is split. The LeftSibling is eirther full or null.
	BTreeNode* CurrentRightSibling = getRightSibling();

	setRightSibling(newNode); //New LeftNode will always on the right of the originalNode.
	newNode->setParent(parent); //update newNode to its parent

	if (CurrentRightSibling == NULL) //If the RightSibling is null
	{
		newNode->setRightSibling(NULL);
		newNode->setLeftSibling(this);
	}
	else
	{
		CurrentRightSibling->setLeftSibling(newNode); //New LeafNode will be on the left of the CurrentRightSibling
		newNode->setRightSibling(CurrentRightSibling);
		newNode->setLeftSibling(this);
	}

	

	
	/*
	LeafNode* newNode = new LeafNode(leafSize, NULL, NULL, NULL);
	//set all pointers
	BTreeNode* originalRight = getRightSibling();

	newNode->setParent(parent);//if parent is full?
	setRightSibling(newNode);
	if (originalRight == NULL)//there is no node on the right
	{
		newNode->setRightSibling(NULL);
		newNode->setLeftSibling(this);
	}
	else//there is node on the right
	{
		newNode->setRightSibling(originalRight);
		newNode->setLeftSibling(this);
		originalRight->setLeftSibling(newNode);
	}
	*/


	bool inserted = false;
	if (value > values[leafSize - 1]) { 
		newNode->insert(value);
		for (int i = (int)ceil(double(leafSize) / 2); i < leafSize; i++) {
			newNode->insert(values[i]);
			count--;
		}
	}
	else {
		int tempCount = (int)ceil(double(leafSize + 1) / 2);
		for (int i = leafSize - 1; i >= 0 && tempCount > 0; i--) { 
			if (value < values[i]) {
				newNode->insert(values[i]);
				count--;
				tempCount--;
			}
			else {
				newNode->insert(value);
				tempCount--;
				inserted = true;
				cout << "inserted: " << inserted;
			}
		}
		if (tempCount == 0 && inserted == false) {
			cout << "value: " << value;
			insert(value);
		}
	}
	return newNode;
	
	/*
	LeafNode* newNode = new LeafNode(leafSize, NULL, NULL, NULL);
	//set all pointers
	BTreeNode* originalRight = getRightSibling();

	newNode->setParent(parent);//if parent is full?
	setRightSibling(newNode);
	if (originalRight == NULL)//there is no node on the right
	{
		newNode->setRightSibling(NULL);
		newNode->setLeftSibling(this);
	}
	else//there is node on the right
	{
		newNode->setRightSibling(originalRight);
		newNode->setLeftSibling(this);
		originalRight->setLeftSibling(newNode);
	}
	//move numbers
	int max = values[leafSize - 1];
	if (max>value)
	{
		newNode->insert(max);
		values[leafSize - 1] = 0;
		count--;
		insert(value);
	}
	else
	{
		newNode->insert(value);
		//count--;
	}
	int center = (int)ceil(double(leafSize) / 2);
	for (int i = center; i < leafSize; i++)
	{
		newNode->insert(values[i]);
		values[i] = 0;//set the removed number to 0
		count--;
	}
	//
	return newNode;
	*/
}

//Sorted Array using Insertion Sort
void LeafNode::SortedArray() {
	for (int i = 1; i < count; i++)
	{
		int index = values[i]; int j = i;
		while (j > 0 && values[j - 1] > index)
		{
			values[j] = values[j - 1];
			j--;
		}
		values[j] = index;
	}
}

//Shift the Value to the Left Sibling
void LeafNode::ShiftValueToLeft(int value) {
	getLeftSibling()->insert(values[0]);
	DeleteFirstNodeFromLeaf();
	insert(value);
}

//Shift the Value to the Right Sibling
void LeafNode::ShiftValueToRight(int value) {
	DeleteLastNodeFromLeaf(value);
	insert(value);
}

//Delete the first value from the leaf
void LeafNode::DeleteFirstNodeFromLeaf() {
	for (int i = 0; i < count - 1; i++) {
		values[i] = values[i + 1];
	}
	count--;
}

//Delete the last value from the leaf
void LeafNode::DeleteLastNodeFromLeaf(int value) {
	if (values[leafSize - 1] < value) {
		getRightSibling()->insert(value);
	}
	else {
		getRightSibling()->insert(values[leafSize - 1]);
	}
	count--;
}