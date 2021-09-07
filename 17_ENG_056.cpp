#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <Windows.h>
#include <map>
#include <vector>
#include "Header_class.h"

#define Empty_value 0

using namespace std;
using namespace std::chrono;
string directory;

//defined Sudoku Constructor getting sata from text file
Sudoku::Sudoku(string fileName) {
    Input_file = fileName;
    ifstream inputSudokuPuzzle(fileName);

    //checking whether file can be opened or not
    if (!inputSudokuPuzzle) {
        cout << "Could't open file!" << endl;
    }

    //reading the file
    string row;
    while (getline(inputSudokuPuzzle, row)) {
        stringstream rowstream(row);
        int currentValue;
        vector<int> rowValues;
        while (rowstream >> currentValue) {
            rowValues.push_back(currentValue);
        }
        //add elemnt to puzzle
        Sudoku_puzzle.push_back(rowValues);
    }
    //close resource file
    inputSudokuPuzzle.close();

    //getting puzzle size 
    N = Sudoku_puzzle.size();

    //check whether puzzle size is valid
    Sub_grid_size = sqrt(N); 
    if (Sub_grid_size * Sub_grid_size != N) {
        cout << "Puzzle size is invalid!" << endl;
        exit(0);
    }
    else {
        for (int val = 1; val <= N; val++) {
            Allow_values.push_back(val);
        }
        for (int row = 0; row < N; row++) {
            vector<vector<int>> allowedValuesRow;
            for (int col = 0; col < N; col++) {
                allowedValuesRow.push_back(Allow_values);
            }
            Possible_values.push_back(allowedValuesRow);
        }
    }
}

//update possible values of cells
void Sudoku::Update_possible_values() {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (Sudoku_puzzle[row][col] == Empty_value) {
                vector<int> currAllowedVals;
                for (int val : Possible_values[row][col]) {
                    if (Is_safe(row, col, val)) {
                        currAllowedVals.push_back(val);
                    }
                }
                Possible_values[row][col] = currAllowedVals;
            }
            else {
                Possible_values[row][col] = {};
            }
        }
    }
}

//check whether the assigned number violates any rule or not
bool Sudoku::Is_safe(int row, int column, const int num) {
    return Is_row_safe(row, num) && Is_column_safe(column, num) && Is_box_safe(row, column, num);
}

//check whether assigned number already in the row or not
bool Sudoku::Is_row_safe(int row, const int num) {
    for (int column = 0; column < N; column++) {
        if (Sudoku_puzzle[row][column] == num) {
            return false;
        }
    }
    return true;
}

//check whether assigned number already in the column or not
bool Sudoku::Is_column_safe(int column, const int num) {
    for (int row = 0; row < N; row++) {
        if (Sudoku_puzzle[row][column] == num) {
            return false;
        }
    }
    return true;
}

//check whether assigned number already in the sub grid or not
bool Sudoku::Is_box_safe(int row, int column, const int num) {
    int subGridRow;
    int subGridColumn;
    if (N == 9) {
        subGridRow = row - row % 3;
        subGridColumn = column - column % 3;
    }
    else if (N == 16) {
        subGridRow = row - row % 4;
        subGridColumn = column - column % 4;
    }

    for (int i = 0; i < Sub_grid_size; i++) {
        for (int j = 0; j < Sub_grid_size; j++) {
            if (Sudoku_puzzle[subGridRow + i][subGridColumn + j] == num) {
                return false;
            }
        }
    }
    return true;
}

//update single possible values to the puzzle
bool Sudoku::Update_puzzle_value() {
    bool terminator = true;
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (Sudoku_puzzle[row][col] == Empty_value && Possible_values[row][col].size() == 1) {
                Sudoku_puzzle[row][col] = Possible_values[row][col][0];
                Possible_values[row][col] = {};
                terminator = false;
            }
        }
    }
    return terminator;
}

//update single values to the possible value list
bool Sudoku::Update_possible_value_list() {
    bool terminator = true;
    for (int row = 0; row < N; row++) {
        map<int, vector<int>> possibleValueCounter;
        for (int col = 0; col < N; col++) {
            for (int val : Possible_values[row][col]) {
                possibleValueCounter[val].push_back(col);
            }
        }
        map<int, vector<int>>::iterator it;
        for (it = possibleValueCounter.begin(); it != possibleValueCounter.end(); it++) {
            if ((it->second).size() == 1) {
                int colmn = (it->second)[0];
                Sudoku_puzzle[row][colmn] = it->first;
                Possible_values[row][colmn] = {};
                terminator = false;
            }
        }
    }
    Update_possible_values();

    for (int col = 0; col < N; col++) {
        map<int, vector<int>> possibleValueCounter;
        for (int row = 0; row < N; row++) {
            for (int val : Possible_values[row][col]) {
                possibleValueCounter[val].push_back(row);
            }
        }
        map<int, vector<int>>::iterator it;
        for (it = possibleValueCounter.begin(); it != possibleValueCounter.end(); it++) {
            if ((it->second).size() == 1) {
                int rw = (it->second)[0];
                Sudoku_puzzle[rw][col] = it->first;
                Possible_values[rw][col] = {};
                terminator = false;
            }
        }
    }
    Update_possible_values();

    for (int row = 0; row < Sub_grid_size; row++) {
        for (int col = 0; col < Sub_grid_size; col++) {
            map<int, vector<vector<int>>> possibleValueCounter;
            for (int r = row * Sub_grid_size; r < (row + 1) * Sub_grid_size; r++) {
                for (int c = col * Sub_grid_size; c < (col + 1) * Sub_grid_size; c++) {
                    for (int val : Possible_values[r][c]) {
                        vector<int> tem = { r,c };
                        possibleValueCounter[val].push_back(tem);
                    }

                }
            }
            map<int, vector<vector<int>>>::iterator it;

            for (it = possibleValueCounter.begin(); it != possibleValueCounter.end(); it++)
            {
                if ((it->second).size() == 1) {
                    vector<int> tem = (it->second)[0];
                    Sudoku_puzzle[tem[0]][tem[1]] = it->first;
                    Possible_values[tem[0]][tem[1]] = {};
                    terminator = false;
                }
            }
        }
    }

    return terminator;
}

//optimize possible value list
void Sudoku::Optimize_possible_values() {
    bool terminator = false;
    while (!terminator) {
        Update_possible_values();
        terminator = Update_puzzle_value();
        Update_possible_values();
        terminator = Update_possible_value_list();
    }
}

//backtraking optimal values
bool Sudoku::Backtracking() {
    int row, col;

    if (!Is_empty(row, col)) {
        return true;
    }

    for (int val : Possible_values[row][col]) {
        if (Is_safe(row, col, val)) {
            Sudoku_puzzle[row][col] = val;

            if (Backtracking()) {
                return true;
            }
            Sudoku_puzzle[row][col] = Empty_value;
        }
    }
    return false;
}

//check whether empty cells exists or not
bool Sudoku::Is_empty(int& row, int& col) {
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (Sudoku_puzzle[row][col] == Empty_value)
                return true;
    return false;
}

//solving sudoku puzzle
bool Sudoku::Is_solve() {
    Update_possible_values();
    Optimize_possible_values();
    if (Backtracking()) {
        return true;
    }
    else {
        return false;
    }
}

//print puzzle
void Sudoku::Print_puzzle() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << Sudoku_puzzle[i][j] << " ";
        }
        cout << endl;
    }
}

//saving the solved puzzle to txt file
void Sudoku::Write_solution() {
    string namePrefix;
    for (char c : Input_file) {
        if (c == '.') {
            break;
        }
        namePrefix.push_back(c);
    }
    string outputFileName = namePrefix + "_output.txt";
    ofstream outputFile(outputFileName);
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            outputFile << Sudoku_puzzle[row][col] << " ";
        }
        outputFile << endl;
    }
    outputFile.close();
}

//saving errors to txt file
void Sudoku::write_massage() {
    string namePrefix;
    for (char c : Input_file) {
        if (c == '.') {
            break;
        }
        namePrefix.push_back(c);
    }
    string outputFileName = namePrefix + "_output.txt";
    ofstream outputFile(outputFileName);
    outputFile << "No solution exists!" << endl;
    outputFile.close();
}


string textRead()
{
    string userInput;
    cout << "File's path (ex:- D:\\input.txt) : ";
    cin >> userInput;
    ifstream gettingInput(userInput);
    while (!gettingInput)
    {
        cout << "Invalid Path! \n\nPlease enter correct File's path: ";
        cin >> userInput;
        gettingInput.open(userInput);
    }
    gettingInput.close();
    return userInput;
}


int main(int argc, char* argv[])
{
	//Getting input text files
	ifstream givenSudoku;
	if (argc != 2)
	{
		directory = textRead();
		givenSudoku.open(directory);
	}
	else
	{
		directory = argv[1];
		givenSudoku.open(directory);
		if (!givenSudoku)
		{
			directory = textRead();
			
			givenSudoku.open(directory);
		}
	}

    //creating sudolu puzzel object 
    Sudoku Sudoku_puzzel(directory);

    //print sudoku puzzle
    cout << "\nInitial Puzzle : " << endl;
    Sudoku_puzzel.Print_puzzle();
    cout << endl;

    //get the starting time point
    high_resolution_clock::time_point start = high_resolution_clock::now();

    //solving sudoku puzzle
    if (Sudoku_puzzel.Is_solve()) {
        cout << "\nSloved Puzzle : " << endl;
        Sudoku_puzzel.Print_puzzle();
        Sudoku_puzzel.Write_solution();
    }
    else {
        cout << "\nNo solution exists!" << endl;
        Sudoku_puzzel.write_massage();
    }

    //get the ending time point
    high_resolution_clock::time_point stop = high_resolution_clock::now();

    //calculating duration and printing time taken for solving
    cout << "\nTime taken for solving : ";
    auto countSec = duration_cast<seconds>(stop - start);
    cout << "\n\tSeconds     : " << countSec.count();
    auto countMilisec = duration_cast<milliseconds>(stop - start);
    cout << "\n\tMilliseconds: " << countMilisec.count();
    auto countMicrosec = duration_cast<microseconds>(stop - start);
    cout << "\n\tMicroseconds: " << countMicrosec.count();
    cout << endl << endl << endl;

	system("pause");
	return 0;
}
