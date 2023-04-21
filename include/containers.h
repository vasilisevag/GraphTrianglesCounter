#pragma once
#include "headers.h"
using namespace std;

struct adjacency_csc{
    int dim; // adjacency dimension
    int n_nnz; // number of nnzvals

    // all values are equal to 1 so we don't store them
    int* rowind; // row indices
    int* colstrelem;  // number of column's starting element

    adjacency_csc(int dim, int n_nnz, int* rowind, int* colstrelem);
};

struct Container{
    adjacency_csc* adj_csc;
    unordered_map<int, int>* s_elems;
    pair<int, int> start_end;
};