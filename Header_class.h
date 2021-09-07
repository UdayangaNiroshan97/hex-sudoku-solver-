#include<iostream>
using namespace std;

//Sudoku  Class
class Sudoku {
private:
    vector<vector<int>> Sudoku_puzzle;
    int N;
    vector<int> Allow_values;
    vector<vector<vector<int>>> Possible_values;
    int Sub_grid_size;
    string Input_file;

public:
    Sudoku(string fileName);
    void Update_possible_values();
    bool Is_safe(int row, int column, const int num);
    bool Is_row_safe(int row, const int num);
    bool Is_column_safe(int column, const int num);
    bool Is_box_safe(int row, int column, const int num);
    bool Update_puzzle_value();
    bool Update_possible_value_list();
    void Optimize_possible_values();
    bool Is_solve();
    void Print_puzzle();
    bool Is_empty(int& row, int& col);
    bool Backtracking();
    void Write_solution();
    void write_massage();
};