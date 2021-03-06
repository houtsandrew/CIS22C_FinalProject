#include "CommandLine.h"

void ClearScreen()
{
    cout << "\033[2J" << "\033[1;1H";
}

CommandLine::CommandLine(Sheet *sheet, Change *change) : refresh(sheet) {
    //initializes all the variables
    this->sheet = sheet;
    this->change = change;
    bst = BST();
    CELL_WIDTH = 12;
    COUNT_WIDTH = 4;
    CELLS_TO_DISPLAY_X = 12;
    CELLS_TO_DISPLAY_Y = 40;
}

void CommandLine::modifyCell(int xCoord, int yCoord, string data) {
    //called when the data in a node needs to be changed
    sheet->setCellData(xCoord, yCoord, data);
}

void CommandLine::drawSheet(ostream &out) {

    ClearScreen();

    if (CELLS_TO_DISPLAY_X > sheet->getXSize()) {
        CELLS_TO_DISPLAY_X = sheet->getXSize();
    }

    if (CELLS_TO_DISPLAY_Y > sheet->getYSize()) {
        CELLS_TO_DISPLAY_Y = sheet->getYSize();
    }

    for (int countWidth = 0; countWidth <= COUNT_WIDTH; countWidth++) {
        out << " ";
    }

    for (int i = 0; i < CELLS_TO_DISPLAY_X; i++) {
        out << right << setw(CELL_WIDTH) << fixed << setprecision(3) << i << "|";
    }
    out << endl;

    for (int countWidth = 0; countWidth <= COUNT_WIDTH; countWidth++) {
        out << " ";
    }

    // goes thorugh both x and y values and plots the sheet for the user to see
    for (int i = 0; i < CELLS_TO_DISPLAY_X; i++) {
        for (int cellWidth = 0; cellWidth <= CELL_WIDTH; cellWidth++) {
            out << "-";
        }
    }
    out << endl;

    for (int y = 0; y < CELLS_TO_DISPLAY_Y; y++) {
        out << left << setw(COUNT_WIDTH) << fixed << y << "|";
        for (int x = 0; x < CELLS_TO_DISPLAY_X; x++) {
            out << right << setw(CELL_WIDTH) << fixed << setprecision(3)
                << sheet->getCellData(x, y).substr(0, CELL_WIDTH);
            out << "|";
        }
        out << endl;
    }
}

void CommandLine::mainLoop(ostream &out, istream &in) {
    CELL_WIDTH = 12;
    COUNT_WIDTH = 4;
    CELLS_TO_DISPLAY_X = 12;
    CELLS_TO_DISPLAY_Y = 40;

    string word1, string1;
    while (word1 != "exit") // runs out of loop when the user enters "exit"
    {
        refresh.findfunctions();
        drawSheet(cout);
        out << "=> ";
        cin >> word1;

        if (word1 == "set") {
            string word2, word3;
            cin >> word2 >> word3;
            try {
                if (stoi(word2) || stoi(word2) ==
                                   0)//if the second word is convertable to an int, do so, and continue, otherwise gve an error
                {
                    int a = stoi(word2);
                    try     //tries to run stoi and catches it if it doesnt work.
                    {
                        if (stoi(word3) || stoi(word3) ==
                                           0)    //if the third word is convertable to an int, do so, and continue, otherwise gve an error
                        {
                            int b = stoi(word3);

                            cin.ignore();
                            getline(cin, string1);  //takes a string of what to add into the correct cell



                            if (a >= sheet->getXSize() || b >= sheet->getYSize() || a < 0 || b < 0) {
                                cout << "one or more values are out of bounds.";
                                std::cout << "Press enter to continue ...";
                                std::cin.clear();
                                std::cin.get();
                            }
                            else {
                                sheet->setCellData(a, b, string1);
                            }

                        }
                    }
                    catch (std::invalid_argument) {
                        cout << "invalid 'Y' value entered\n";
                        cin.ignore(1000, '\n');
                        std::cout << "Press enter to continue ...";
                        std::cin.clear();
                        std::cin.get();
                    }
                }
            }
            catch (std::invalid_argument) {
                cout << "invalid 'X' value entered\n";
                cin.ignore(1000, '\n');
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
        }
        else if (word1 == "undo") {
            try {
                change->undo(sheet);
                // successfully undone
            }
            catch (char e[]) {
                cout << "\nChange Log Empty\n";
                cin.ignore(1000, '\n');
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
        }
        else if (word1 == "search") {
            string searchstring;
            cin.ignore(1);
            getline(cin, searchstring);

            BST1 bst = BST1(sheet);
            bst.loadFromSheet();

            CellContainer *data = bst.search(searchstring);

            if (data->numCells == 0) {
                cout << "String not Found.\n";
            }
            else {
                cout << "String Found at Indexes:\n\n";
                for (int i = 0; i < data->numCells; i++) {
                    Cell *cell = data->cells[i];
                    cout << "(" << cell->getXCoord() << "," << cell->getYCoord() << ")" << endl;
                }
            }

            delete data;
            std::cout << "Press enter to continue ...";
            std::cin.clear();
            std::cin.get();
        }
        else if (word1 == "setfilepath") {
            string newFileName;

            cin >> newFileName;
            try {
                sheet->setFilePath(newFileName);
                cout << "File path changed successfully.\n";
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
            catch (...) {
                cout << "Unable to change file path.\n";
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
        }
        else if (word1 == "cellstodisplayx") {
            unsigned int toDisplay;
            cin >> toDisplay;
            if (toDisplay <= sheet->getXSize() && toDisplay > 0) {
                CELLS_TO_DISPLAY_X = toDisplay;
                cout << "X Axis cells to display set to " << toDisplay << ".\n";
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
            else {
                cout << "An invalid number of cells to display was given.\n";
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
        }
        else if (word1 == "cellstodisplayy") {
            unsigned int toDisplay;
            cin >> toDisplay;
            if (toDisplay <= sheet->getYSize() && toDisplay > 0) {
                CELLS_TO_DISPLAY_Y = toDisplay;
                cout << "Y Axis cells to display set to " << toDisplay << ".\n";
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
            else {
                cout << "An invalid number of cells to display was given.\n";
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
        }
        else if (word1 == "charstodisplay") {
            unsigned int toDisplay;
            cin >> toDisplay;
            if (toDisplay <= 40 && toDisplay > 1) {
                CELL_WIDTH = toDisplay;
                cout << "Now displaying " << toDisplay << " characters for each cell.\n";
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
            else {
                cout << "An invalid number of characters was provided.\n";
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
        }
        else if (word1 == "save") {
            try {
                sheet->toFile();
                cout << "Save of file " << sheet->getFilePath() << " successfully completed.\n";
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
            catch (...) {
                cout << "Failed to save file.\n";
                std::cout << "Press enter to continue ...";
                std::cin.clear();
                std::cin.get();
            }
        }
        else if (word1 == "exit") {
            // do nothing
        }
        else if (word1 == "sortrow") {
            int x1, x2, y;
            cin >> x1 >> x2 >> y;
            refresh.sortRow(x1, x2, y);
        }
        else if (word1 == "sortcol") {
            int x1, x2, y;
            cin >> x1 >> x2 >> y;
            refresh.sortCol(x1, x2, y);
        }
        else if (word1 == "printtree") {
            BST1 bst = BST1(sheet);
            bst.loadFromSheet();
            bst.printTree(cout);
            std::cout << "Press enter to continue ...";
            std::cin.clear();
            std::cin.get();
        }
        else if (word1 == "displayhashtable") {
            ClearScreen();
            sheet->printHashTable(cout);
            std::cout << "Press enter to continue ...";
            std::cin.clear();
            std::cin.get();
        }
        else if (word1 == "help") {
            ClearScreen();
            cout << "Commands:\n";
            cout << "set [int] [int] [string/int/double]\n";
            cout << "search [string/int/double]\n";
            cout << "sortrow [xCoord1] [xCoord2] [yCoord]\n";
            cout << "sortcol [yCoord1] [yCoord2] [xCoord]\n";
            cout << "undo\n";
            cout << "setfilepath [newFilePath]\n";
            cout << "cellstodisplayx [int]\n";
            cout << "cellstodisplayy [int]\n";
            cout << "charstodisplay [int]\n";
            cout << "printtree\n";
            cout << "displayhashtable\n";
            cout << "save\n";
            cout << "exit\n";
            std::cout << "Press enter to continue ...";
            std::cin.clear();
            std::cin.get();
        }
        else {
            cout << "\nInvalid entry (type help for a list of commands)\n";
            cin.ignore(1000, '\n');
            std::cout << "Press enter to continue ...";
            std::cin.clear();
            std::cin.get();
        }
    }
    change->deleteStack();
}
