
#include "Sheet.h"

Sheet::Sheet(int xSize, int ySize, Change *change)
{
	hashTableSizeMultiplier = 30;  // at this value we can have a max resolution of 1 at every combination of x and y size from 1-20 by 1-100
	maxResolutionAttempts = 5;
	initializeSheet(xSize, ySize); // initalize all of the cells for a sheet of this size
	generateHashTable();
	filePath = "./spreadsheet.dat";
	this->change = change;
};

Sheet::~Sheet()
{
	wipeSheet();			// delete all of the cells in the sheet
	delete[] hashTable;
};

Cell * Sheet::operator()(int x, int y)
{
	if (x >= xSize || x < 0)
	{
		char error[] = "Index x is out of bounds.\n";
		throw error;
	}
	if (y >= ySize || y < 0)
	{
		char error[] = "Index y is out of bounds.\n";
		throw error;
	}
	int index = getHashIndex(x, y, hashTableMultiplier, hashTableAddition, hashTableSize);		// get first possible index of cell
	Cell *data = hashTable[index];
	while (!(data->getXCoord() == x && data->getYCoord() == y))		// find the correct cell by quadratic resolution
	{
		index = quadraticResolution(index, hashTableSize);
		data = hashTable[index];
	}

	return data;
};

void Sheet::generateHashTable()
{
	if (hashTable != nullptr)
	{
		delete[] hashTable;
	}

	hashTableSize = getPrimeGreaterThan(xSize * ySize * hashTableSizeMultiplier);		// choose a hashTableSize proportional to the hash table volume
	hashTableMultiplier = getPrimeGreaterThan(100);		// these values happen to be quite good
	hashTableAddition = getPrimeGreaterThan(130);

	hashTable = new Cell*[hashTableSize];		// create a hash table to store cell pointers in

	for (int i = 0; i < hashTableSize; i++)			// set all pointers in hash table to null
	{
		hashTable[i] = nullptr;
	}

	for (int j = 0; j < ySize; j++)		// for each y coord
	{
		for (int i = 0; i < xSize; i++)		// for each x coord
		{
			int cellX = nonHashSearch(i, j)->getXCoord();		// get the cells coordinates
			int cellY = nonHashSearch(i, j)->getYCoord();
			int index = getHashIndex(cellX, cellY, hashTableMultiplier, hashTableAddition, hashTableSize);		// see if the spot is available
			if (hashTable[index] == nullptr)		// if the spot is available 
			{
				hashTable[index] = nonHashSearch(i, j);		// set its spot to that location
			}
			else
			{
				bool done = false;
				int count = 0;
				while (!done && count < maxResolutionAttempts)		// otherwise use quadratic resolution to find the first empty spot
				{
					index = quadraticResolution(index, hashTableSize);
 					if (hashTable[index] == nullptr)
					{
						hashTable[index] = nonHashSearch(i, j);
						done = true;
					}
					else
					{
						count++;
					}
				}
				if (count == maxResolutionAttempts)		// if it took too many cycles, throw error
				{
					char error[] = "Hash resolution took too many cycles.\n";
					throw error;
				}
			}
		}
	}
};

Cell * Sheet::nonHashSearch(int x, int y)
{
	if (x >= xSize || x < 0)
	{
		char error[] = "Sheet passed an index x that is out of bounds.\n";
		throw error;
	}
	if (y >= ySize || y < 0)
	{
		char error[] = "Sheet passed an index y that is out of bounds.\n";
		throw error;
	}
	Cell *temp = headerCell;		// slow search for use before hashTable is generated
	for (int i = 0; i < x; i++)
	{
		temp = temp->getRight();
	}
	for (int i = 0; i < y; i++)
	{
		temp = temp->getBelow();
	}
	return temp;
};


void Sheet::setCellData(int x, int y, string str)
{
	change->pushBack(x, y, operator()(x, y)->getData(), str);
	operator()(x, y)->setData(str);
};

string Sheet::getCellData(int x, int y)
{
	return operator()(x, y)->getData();
};

void Sheet::toFile()
{
	ofstream fout = ofstream();
	fout.open(filePath);
	if (fout.fail())
	{
		char error[] = "Failed to open file for writing.\n";
		throw error;
	}

	fout << xSize << endl;		// write the size to the file
	fout << ySize << endl;

	for (int i = 0; i < hashTableSize; i++)
	{
		if (hashTable[i] != nullptr)		// write everything that isn't nothing
		{
			Cell *temp = hashTable[i];
			fout << temp->getXCoord() << " ";
			fout << temp->getYCoord() << " ";
			fout << temp->getData() << endl;
		}
		else
		{
			fout << endl;	// write nothing
		}
	}

	fout.close();
};

void Sheet::fromFile()
{
	ifstream fin = ifstream();
	fin.open(filePath);
	if (fin.fail())
	{
		char error[] = "Failed to open file for reading.\n\0";
		throw error;
	}
	int newXSize;
	int newYSize;
	try
	{
		fin >> newXSize;
		fin >> newYSize;
	}
	catch (...)
	{
		char error[] = "Invalid file given.\n\0";
		throw error;
	}

	resizeSheet(newXSize, newYSize);		// set sheet size to be the same as the sheet that is being loaded

	string *fileData = new string[hashTableSize];

	for (int i = 0; i < hashTableSize; i++)		// load file into memory
	{
		string line;
		getline(fin, line);
		fileData[i] = line;
	}

	for (int i = 0; i < hashTableSize; i++)
	{
		string *answer = new string[2];

		for (int i = 0; i < 2; i++)
		{
			answer[i] = "";
		}

		string line = fileData[i];
		stringstream ssin(line);
		int count = 0;
		while (ssin.good() && count < 2)	// put the file data into an easier to work with format
		{
			ssin >> answer[count];
			count++;
		}

		ssin.ignore(1);		// ignore the space after the second index

		string data;
		getline(ssin, data);		// get the cell data

		if (count == 2 && (stoi(answer[0]) || stoi(answer[0]) == 0) && (stoi(answer[1]) || stoi(answer[1]) == 0))
		{
			operator()(stoi(answer[0]), stoi(answer[1]))->setData(data);	// set cell data if all parameters are met
		}
		delete[] answer;
	}

	generateHashTable();		// generate a hashTable of the correct size
};

void Sheet::swapRow(int y1, int y2)
{
	string *row = new string[xSize];	// create a temporary row

	Cell *temp = operator()(0, y2);
	for (int i = 0; i < xSize; i++)
	{
		row[i] = temp->getData();	// copy row y2 into temp row
		temp = temp->getRight();
	}

	temp = operator()(0, y1);
	Cell *temp2 = operator()(0, y2);
	for (int i = 0; i < xSize; i++)		// copy row y1 into y2
	{
		setCellData(temp2->getXCoord(), temp2->getYCoord(), temp->getData());
		temp = temp->getRight();
		temp2 = temp2->getRight();
	}

	temp = operator()(0, y1);
	for (int i = 0; i < xSize; i++)		// copy temp row into y1
	{
		setCellData(temp->getXCoord(), temp->getYCoord(), row[i]);
		temp = temp->getRight();
	}

	delete[] row;
	delete temp;
	delete temp2;
};

void Sheet::swapCol(int x1, int x2)
{
	string *col = new string[ySize];	// create a temporary col

	Cell *temp = operator()(0, x2);
	for (int i = 0; i < ySize; i++)		// copy col x2 into temp col
	{
		col[i] = temp->getData();
		temp = temp->getBelow();
	}

	temp = operator()(0, x1);
	Cell *temp2 = operator()(0, x2);
	for (int i = 0; i < ySize; i++)		// copy col x1 into col x2
	{
		setCellData(temp2->getXCoord(), temp2->getYCoord(), temp->getData());
		temp = temp->getBelow();
		temp2 = temp2->getBelow();
	}

	temp = operator()(0, x1);
	for (int i = 0; i < ySize; i++)		// copy temp col into x1
	{
		setCellData(temp->getXCoord(), temp->getYCoord(), col[i]);
		temp = temp->getBelow();
	}

	delete[] col;
	delete temp;
	delete temp2;
};

int Sheet::getXSize()
{
	return xSize;
};

int Sheet::getYSize()
{
	return ySize;
};

string Sheet::getFilePath()
{
	return filePath;
};

void Sheet::setFilePath(string filePath)
{
	this->filePath = filePath;
}
void Sheet::changeOpenParen()
{
	change->pushBack(true, false);
}

void Sheet::changeCloseParen()
{
	change->pushBack(false, true);
}

void Sheet::printHashTable(ostream &out)
{
	for (int i = 0; i < hashTableSize; i++)
	{
		if (hashTable[i] != nullptr)
		{
			out << "[" << i << "]" << "  (" << hashTable[i]->getXCoord() << "," << hashTable[i]->getYCoord() << ") " << "=  " << hashTable[i]->getData() << endl;
		}
	}
}


void Sheet::resizeSheet(int xSize, int ySize)
{
	wipeSheet();					// delete all cells in sheet
	initializeSheet(xSize, ySize);	// create sheet of cells of the right size
	generateHashTable();
};

void Sheet::initializeSheet(int xSize, int ySize)
{
	if (xSize <= 0 || ySize <= 0)
	{
		char error[] = "Invalid size chosen for Sheet.\n";
		throw error;
	}
	try
	{
		this->xSize = xSize;
		this->ySize = ySize;
	}
	catch (...)
	{
		char error[] = "Error initalizing sheet.\n\0";
		throw error;
	}

	// create a dynamically allocated array of pointers
	Cell **arr = new Cell*[xSize * ySize];

	// initialize all cells
	for (int i = 0; i < xSize * ySize; i++)
	{
		arr[i] = new Cell();
	}

	for (int j = 0; j < ySize; j++)
	{
		for (int i = 0; i < xSize; i++)
		{
			// go through and form 2 way linkage between every cell above and to the left of the current cell
			if (i > 0 && j > 0)
			{
				arr[index(i, j, xSize)]->setLeft(arr[index(i - 1, j, xSize)]);
				arr[index(i - 1, j, xSize)]->setRight(arr[index(i, j, xSize)]);
				arr[index(i, j, xSize)]->setAbove(arr[index(i, j - 1, xSize)]);
				arr[index(i, j - 1, xSize)]->setBelow(arr[index(i, j, xSize)]);
			}
			else if (i > 0)
			{
				arr[index(i, j, xSize)]->setLeft(arr[index(i - 1, j, xSize)]);
				arr[index(i - 1, j, xSize)]->setRight(arr[index(i, j, xSize)]);
			}
			else if (j > 0)
			{
				arr[index(i, j, xSize)]->setAbove(arr[index(i, j - 1, xSize)]);
				arr[index(i, j - 1, xSize)]->setBelow(arr[index(i, j, xSize)]);
			}
			else
			{

			}
		}
	}
	headerCell = arr[0];
	// delete the array of pointers
	delete[] arr;
};

void Sheet::wipeSheet()
{
	Cell *currentRow = headerCell;
	Cell *nextRow = headerCell;
	for (int j = 0; j < ySize; j++)		// for each row in the sheet
	{
		if (j < ySize - 1)
		{
			nextRow = currentRow->getBelow();	// find the row beneath
		}
		Cell *nextCell = currentRow;
		for (int i = 0; i < xSize; i++)		// delete all cells in the row
		{
			Cell *currentCell = nextCell;
			nextCell = nextCell->getRight();
			delete currentCell;
		}
		currentRow = nextRow;
	}
};

int Sheet::index(int x, int y, int width)
{
	return x + width * y;		// used in initialize sheet
};

bool Sheet::isPrime(int number)
{
	bool flag = false;
	for (int i = 2; i < number / 2; i++)
	{
		if (number % i == 0)
		{
			flag = true;
		}
	}
	return !flag;
};

int Sheet::getPrimeGreaterThan(int number)
{
	if (number <= 1)
	{
		char error[] = "Invalid number passed into getPrimeGreaterThan().\n";
		throw error;
	}
	int count = number + 1;
	while (!isPrime(count))
	{
		count++;
	}
	return count;
};

int Sheet::getHashIndex(int cellXIndex, int cellYIndex , int multiplier, int addition, int hashTableSize)
{
	int temp = abs((cellXIndex + addition * cellYIndex - multiplier) * multiplier + addition);		// arbitrary algorithm that is relatively good
	return temp % hashTableSize;
};

int Sheet::quadraticResolution(int index, int hashTableSize)		// standard quadratic resolution
{
	int temp = abs((double)index * (double)index);		// use a double to prevent overflow (sometimes)
	return temp % hashTableSize;
};
