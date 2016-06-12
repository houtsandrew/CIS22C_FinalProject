#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include "Change.h"
#include "Sheet.h"
#include "Refresh.h"
#include "Cell.h"
#include "BST.h"
using namespace std;

class CommandLine
{
private:
	BST BST;
	Sheet *sheet;
	Change change;
	Refresh refresh;
	BST bst;
public:
	CommandLine(Sheet *sheet);
	void modifyCell(int xCoord, int yCoord, string data);
	void drawSheet(ostream& out);
	void mainLoop();
};

#endif