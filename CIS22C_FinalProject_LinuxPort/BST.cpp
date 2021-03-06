#include "BST.h"
;
void BST::rotateLeft(Node* target) 
{
	Node* holder = target;
	target = target->getRight();
	holder->setRight(target->getLeft());
	target->setLeft(holder);
}

void BST::rotateRight(Node* target) 
{
	Node* holder = target;
	target = target->getLeft();
	holder->setLeft(target->getRight());
	target->setRight(holder);
}

void BST::generateTree(Sheet &sheet)
{
	Node* temp;
	clearTree(headNode);
	for (int j = 0; j < sheet.getYSize(); j++)
	{
		for (int i = 0; i < sheet.getXSize(); i++)
		{
			temp = new Node();
			temp->setMe(sheet(i, j));
			addNode(temp, headNode);
		}
	}
}

/*void BST::clearTree()
{
	if (headNode != NULL) {
		if (headNode->getLeft() != NULL) {
			clearTree(headNode->getLeft());
		}
		if (headNode->getRight() != NULL) {
			clearTree(headNode->getRight());
		}
		if (headNode->getRight() == NULL && headNode->getLeft() == NULL) {
			delete headNode;
		}
	}
}*/

void BST::removeNode(Node *targetNode) //moves all branches from node to left, assigns value of leftChild to targetNode, balances tree
{
	Node* holder = targetNode;
	Node *holder1 = NULL, *holder2 = NULL;
	if (targetNode->getRight() != NULL) {
		holder1= targetNode -> getRight();
	}
	if (targetNode->getLeft() != NULL) {
		holder2 = targetNode->getLeft();
	}
	delete holder;
	if (holder1 != NULL) {
		addNode(holder1, headNode);
	}
	if(holder2 != NULL) {
		addNode(holder2, headNode);
	}
}

Node* BST::search(string goal, Node* target, Sheet *sht) //compares value of goal to values in me and children, if not found, calls search recursively until no children found
{
	if (target != NULL) {
		if (target->getMe() == goal) {
			return target;
		}
		if (goal <= target->getMe()) {
			if (target->getLeft() != NULL) {
				return search(goal, target->getLeft(), sht);
			}
		}
		if (goal > target->getMe()) {
			if (target->getRight() != NULL) {
				return search(goal, target->getRight(), sht);
			}
		}
	}
	else {
		return NULL;
	}
	return NULL;
}

void BST::balance(Node* target)//completely untested
{
	if (target->getLeft() != NULL) {
		if (target->getLeft()->getLeft() != NULL || target->getRight() == NULL) {
			rotateRight(target);
		}
	}
	balance(target->getLeft());
	if (target->getRight() != NULL) {
		if (target->getRight()->getRight() != NULL || target->getLeft() == NULL) {
			rotateLeft(target);
		}
	}
	balance(target->getRight());
}

Node* BST::getHead()
{
	return headNode;
}

void BST::addNode(Node* newNode, Node* target) {
	if (target == nullptr) {
		headNode = newNode;
	}else if (target->getLeft() != NULL && target->getRight() != NULL) {
		if (newNode->getMe() <= target->getMe()) {
			addNode(newNode, target->getLeft());
		}
		else {
			addNode(newNode, target->getRight());
		}
	}else if (target->getLeft() == NULL && target->getRight() == NULL) {
		if (newNode->getMe() <= target->getMe()) {
			target->setLeft(newNode);
			newNode->setParent(target);
		}
		else {
			target->setRight(newNode);
			newNode->setParent(target);
		}
	}else if (target->getLeft() != NULL && target->getRight() ==NULL) {
		if (newNode->getMe() <= target->getMe()) {
			addNode(newNode, target->getLeft());
		}
		else {
			target->setRight(newNode);
			newNode->setParent(target);
		}
	}else if (target->getRight() !=NULL && target->getLeft() ==NULL) {
		if (newNode->getMe() > target->getMe()) {
			addNode(newNode, target->getRight());
		}
		else {
			target->setLeft(newNode);
			newNode->setParent(target);
		}
	}
}
void BST::clearTree(Node* target) {
	if (target != NULL) {
		if (target->getLeft() != NULL) {
			clearTree(target->getLeft());
		}
		if (target->getRight() != NULL) {
			clearTree(target->getRight());
		}
		if (target->getLeft() == NULL && target->getRight() == NULL) {
			delete target;
		}
	}
	headNode = NULL;
}
