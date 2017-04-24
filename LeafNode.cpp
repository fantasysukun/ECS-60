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

	//Normal case
	newNode->setLeftSibling(this);
	setLeftSibling(newNode);


	if (value > values[leafSize - 1]) { 
		newNode->insert(value);
		for (int i = (int)ceil(double(leafSize) / 2); i < leafSize; i++) {
			newNode->insert(values[i]);
			count--;
		}
	}
	else {
		for (int i = leafSize; i >= (int)ceil(double(leafSize) / 2); i--) {
			if (value < values[i]) {
				newNode->insert(values[i]);
				count--;
			}
			else {
				newNode->insert(value);
			}
		}
		insert(value);
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
	DeleteFirstNodeFromLeaf();
	insert(value);
}

//Delete the first value from the leaf
void LeafNode::DeleteFirstNodeFromLeaf() {
	for (int i = 0; i < count; i++) {
		values[i] = values[i + 1];
	}
	count--;
}

//Delete the last value from the leaf
void LeafNode::DeleteLastNodeFromLeaf(int value) {
	if (values[count] < value) {
		getRightSibling()->insert(value);
	}
	else {
		getRightSibling()->insert(count);
	}
	count--;
}