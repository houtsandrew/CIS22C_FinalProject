#include "CommandLine.h"

CommandLine::CommandLine(Sheet *sheet) : refresh(sheet)
{
	this->sheet = sheet;
	change = Change();
	bst = BST();
}

void CommandLine::modifyCell(int xCoord, int yCoord, string data)
{
	sheet->setCellData(xCoord, yCoord, data);
}
void CommandLine::drawSheet(ostream& out)
{
	system("CLS");
	for (int y = 0; y < sheet->getYSize(); y++)
	{
		for (int x = 0; x < sheet->getXSize(); x++)
		{
			out << right << setw(8) << sheet->getCellData(x, y).substr(0, 8);
			out << "|";
		}
		out << endl;
	}
}

void CommandLine::mainLoop(ostream &out, istream &in)
{
	refresh.findfunctions();
	string word1, string1;
	while (word1 != "exit")
	{
		drawSheet(cout);
		out << "enter the command you wish to do next: ";
		cin >> word1;

		if (word1 == "set")
		{
			string word2, word3;
			cin >> word2 >> word3;
			if (stoi(word2) || stoi(word2) == 0)   //if the second word is convertable to an int, do so, and continue, otherwise gve an error
			{
				int a = stoi(word2);
				if (stoi(word3) || stoi(word3) == 0)    //if the third word is convertable to an int, do so, and continue, otherwise gve an error
				{
					int b = stoi(word3);

					cin.ignore();
					getline(cin, string1);  //takes a string of what to add into the correct cell



					if (a >= sheet->getXSize() || b >= sheet->getYSize())
					{
						cout << "one or more values are out of bounds.";
						system("pause");
					}
					else
					{
						change.pushBack(a, b, sheet->getCellData(a, b), string1);
						sheet->setCellData(a, b, string1);
					}
				}
			}
		}
		else if (word1 == "undo")
		{
			try
			{
				change.undo(sheet);
				// successfully undone
			}
			catch (char e[])
			{
				// changelog is empty
			}
		}
		else if (word1 == "search")
		{
			string searchstring;

			cin >> searchstring;
			bst.search(searchstring, &bst.getHead(), sheet);
		}
		else if (word1 == "save")
		{
			sheet->toFile();
		}
		else if (word1 == "exit")
		{
			// do nothing
		}
		else
		{
			cout << "\ninvalid entry\n";
			cin.ignore(1000, '\n');
			getline(cin, string1);
		}
	}
	change.deleteStack();
}
