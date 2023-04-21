#include "containers.h"

adjacency_csc::adjacency_csc(int dim, int n_nnz, int* rowind, int* colstrelem) : dim(dim), n_nnz(n_nnz){
    this->rowind = new int[n_nnz];
    this->colstrelem = new int[dim + 1];
    for(int i = 0; i < n_nnz; i++) this->rowind[i] = rowind[i];
    for(int i = 0; i < dim + 1; i++) this->colstrelem[i] = colstrelem[i];
}