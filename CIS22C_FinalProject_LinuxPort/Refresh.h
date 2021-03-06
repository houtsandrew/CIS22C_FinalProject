#ifndef REFRESH_H
#define REFRESH_H
#include "Sheet.h"


class Refresh
{
private:
	Sheet* sheet;
public:
	Refresh(Sheet *sheet);
	void findfunctions();
	double sum(int x1, int x2, int y1, int y2);
	double add(int x1, int x2, int y1, int y2);
	double subtract(int x1, int x2, int y1, int y2);
	double multiply(int x1, int x2, int y1, int y2);
	double divide(int x1, int x2, int y1, int y2);
	void sortRow(int x1, int x2, int y);
	void sortCol(int y1, int y2, int x);
	double average(int x1, int x2, int y1, int y2);
};

#endif