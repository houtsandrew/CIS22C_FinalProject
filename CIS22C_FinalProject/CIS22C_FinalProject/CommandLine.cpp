#include "CommandLine.h"


void CommandLine::modifyCell(int xCoord, int yCoord, string data)
{
	sheet->setCellData(xCoord, yCoord, data);
}
void CommandLine::drawSheet(ostream& out) 
{
	for (int x = 0; x < sheet->getXSize; x++)
	{
		for (int y = 0; y < sheet->getYSize; y++)
		{
			out << sheet->getCellData(x, y);
		}
	}
}

int CommandLine::multiply(int xCoord, int yCoord, int xCoord1, int yCoord1)
{
	int total = 1;
	if (xCoord < xCoord1 && yCoord < yCoord1)
	{
		for (int m = yCoord; m <= yCoord1; m++)
		{
			for (int i = xCoord; i <= xCoord1; i++)
			{
				if (stoi(sheet->getCellData(i, yCoord)))
				{
					int a = stoi(sheet->getCellData(i, yCoord));
					total *= a;

				}
			}
		}
		return total;
	}

	else if (xCoord > xCoord1 && yCoord < yCoord1)
	{
		for (int m = yCoord; m <= yCoord1; m++)
		{
			for (int i = xCoord; i <= xCoord1; i--)
			{
				if (stoi(sheet->getCellData(i, yCoord)))
				{
					int a = stoi(sheet->getCellData(i, yCoord));
					total *= a;

				}
			}
		}
		return total;
	}

	else if (xCoord < xCoord1 && yCoord > yCoord1)
	{
		for (int m = yCoord; m <= yCoord1; m--)
		{
			for (int i = xCoord; i <= xCoord1; i++)
			{
				if (stoi(sheet->getCellData(i, yCoord)))
				{
					int a = stoi(sheet->getCellData(i, yCoord));
					total *= a;

				}
			}
		}
		return total;
	}

	else if (xCoord > xCoord1 && yCoord > yCoord1)
	{
		for (int m = yCoord; m <= yCoord1; m--)
		{
			for (int i = xCoord; i <= xCoord1; i--)
			{
				if (stoi(sheet->getCellData(i, yCoord)))
				{
					int a = stoi(sheet->getCellData(i, yCoord));
					total *= a;

				}
			}
		}
		return total;
	}
}
void CommandLine::mainLoop()
{
	
	drawSheet(cout);
	cout << "enter the command you wish to do next: ";
	string masterString;
	getline(cin, masterString);

	stringstream iss(masterString);

	while (iss)
	{
		string word[100];
		for (int i = 0; i < masterString.length(); i++)
		{
			iss >> word[i];
		}

		// || SET VALUE ||
		if (word[0] == "set")
		{
			if (stoi(word[1]))
			{
				int a = stoi(word[1]);
				if (stoi(word[2]))
				{
					int b = stoi(word[2]);
					if (word[3] == "multiply")
					{
						if (stoi(word[4]))
						{
							int c = stoi(word[4]);
							if (stoi(word[5]))
							{
								int d = stoi(word[5]);
								if (stoi(word[6]))
								{
									int e = stoi(word[6]);
									if (stoi(word[7]))
									{
										int f = stoi(word[7]);
										multiply(c, d, e, f);
										modifyCell(a, b, /*convert multiply() to string and put here)*/);
									}
								}
							}
						}
					}
					else
					{
						modifyCell(a, b, word[3]);
					}
					}
				else
				{
					cout << "invalid entry";
				}
			}
			else
			{
				cout << "invalid entry";
			}
		}
		else
		{
			cout << "invalid entry";
		}
		
	}
}