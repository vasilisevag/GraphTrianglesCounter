#include <fstream>
#include <vector>
#include "helper_functions.h"
using namespace std;

void read_and_discard_comments(ifstream& fin){
    char first_char_of_a_line;
    char line[100];
    fin.get(first_char_of_a_line);
    while(first_char_of_a_line == '%'){
        fin.getline(line, 100);
        fin.get(first_char_of_a_line);
    }
    fin.putback(first_char_of_a_line); // put back the first digit of the dimension :)
}

void input_data(const string& filename, int*& colstrelem, int*& rowind, int* dim, int* n_nnz){
    ifstream fin(filename.c_str());

    read_and_discard_comments(fin);

    fin >> *dim >> *dim; // it's a square matrix, so I read the same value twice!
    fin >> *n_nnz;
    colstrelem = new int[*dim + 1];
    rowind = new int[*n_nnz];
    int row, col;
    int prev_col = -1, elem_idx = 0;
    fin >> row >> col;
    while(!fin.fail()){
        row--; col--; // from 1-indexed to 0-indexed!
        rowind[elem_idx] = row;
        if(col != prev_col){
            for(int i = prev_col + 1; i <= col; i++)
                colstrelem[i] = elem_idx;
            prev_col = col;
        }
        elem_idx++;
        fin >> row >> col;
    }
    for(int i = prev_col + 1; i <= *dim; i++) colstrelem[i] = *n_nnz;
    fin.close();
}
