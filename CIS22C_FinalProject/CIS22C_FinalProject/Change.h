
#ifndef CHANGE_H
#define CHANGE_H

#include <string>
#include "Sheet.h"
using namespace std;

class Change
{
private:
	Change *prev;
	Change *next;
	int row;
	int col;
	string prevData;
	string newData;
	bool isOpenParen;
	bool isCloseParen;
	bool isHead;		// if it is the head Change, don't undo it
public:
	// This creates the head Change that will be used in other classes
	Change();
	// true = openParen, false = closeParen
	Change(bool openOrClose);
	Change(int row, int col, string prevData, string newData, Change *prev, Change *next = nullptr);
	void undo(Sheet *sheet);
	// we assume that this will only be called on the head Change
	Change* pushBack(int row, int col, string prevData, string newData);
};

#endif