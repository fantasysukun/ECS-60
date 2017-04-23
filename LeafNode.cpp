#include <iostream>
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
	if (count < sizeof(values)) {
		values[count] = value;
		count++;
		SortedArray();
	}
	else {
		//Checking LeftSibling
		//Checking RightSibling
		//Split
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

//Checking LeftSibling.
void LeafNode::CheckingLeftSibling() {

}
//Checking RightSibling
void LeafNode::CheckingRightSibling() {

}
//Split
void LeafNode::Split() {

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


