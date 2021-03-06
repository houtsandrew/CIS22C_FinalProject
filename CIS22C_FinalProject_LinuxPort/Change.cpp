
#include "Change.h"
#include "Sheet.h"		// this is actually required

// This creates the head Change that will be used in other classes
Change::Change() {
	isOpenParen = false;
	isCloseParen = false;
	isHead = true;
	row = -1;
	col = -1;
	prev = NULL;
	next = NULL;
	prevData = "NULL";
	newData = "NULL";

}
Change::~Change()
{

}
;
// true = openParen, false = closeParen
Change::Change(bool blockStart, bool blockEnd) {
	isOpenParen = blockStart;
	isCloseParen = blockEnd;
	row = -1;
	col = -1;
	prevData = "NULL";
	newData = "NULL";
	isHead = false;
	prev = NULL;
	next = NULL;
};
Change::Change(int nRow, int nCol, string nPrevData, string nNewData) {
	col = nCol;
	row = nRow;
	prevData = nPrevData;
	newData = nNewData;
	prev = NULL;
	next = NULL;
	isHead = false;
	isCloseParen = false;
	isOpenParen = false;
};
void Change::undo(Sheet *sheet) { // we assume that this will only be called on the head Change
	if (isHead == false && isOpenParen == false && isCloseParen == false) {
		sheet->setCellData(row, col, prevData);
		deleteChange();
	}
	if (isHead == true) {
		if (next != NULL) {
			next->undo(sheet);
		} else {
			char error[] = "Empty Change Log";
			throw error;

		}
	}
	if (isCloseParen == true) {
		while (next->getOpenParen() != true && next != NULL) {
			next->undo(sheet);
		}
		if (next != NULL) {
			next->deleteChange();
		}
		deleteChange();
	}
	if (isOpenParen == true) {
		char error[] = "Lose open Paren found";
		throw error;
	}
};
Change* Change::getNext() {
	return next;
};
Change* Change::getPrev() {
	return prev;
};
void Change::setNext(Change* newNext) {
	next = newNext;
};
void Change::setPrev(Change* newPrev) {
	prev = newPrev;
};
void Change::pushBack(Change* newChange) {
	newChange->setPrev(this);
	newChange->setNext(this->next);
	if (next != nullptr)
	{
		this->next->setPrev(newChange);
	}
	this->next = newChange;
};
void Change::deleteChange() {
	if (prev != NULL) {
		prev->setNext(next);
	}
	if (next != NULL) {
		next->setPrev(prev);
	}
	delete this;
};
bool Change::getOpenParen() {
	return isOpenParen;
};
void Change::pushBack(int nRow, int nCol, string nPrevData, string nNewData) {
	Change* disChange;
	disChange = new Change(nRow, nCol, nPrevData, nNewData);
	pushBack(disChange);
};
void Change::deleteStack() {
	while (next != NULL) {
		next->deleteChange();
	}
	while (prev != NULL) {
		prev->deleteChange();
	}
};
void Change::pushBack(bool blockStart, bool blockEnd) {
	Change* disChange = new Change(blockStart, blockEnd);
	pushBack(disChange);
};