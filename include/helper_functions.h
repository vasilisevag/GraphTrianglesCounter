#pragma once
#include <fstream>
using namespace std;
void read_and_discard_comments(ifstream& fin);
void input_data(const string& filename, int*& colstrelem, int*& rowind, int* dim, int* n_nnz);