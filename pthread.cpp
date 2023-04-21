// pthreads implementation
#include "headers.h"
#include "containers.h"
#include "helper_functions.h"
using namespace std;

const int number_of_threads = 1000;

void* number_of_triangles_in_submatrix(void* args){
    long int number_of_triangles = 0;
    Container* arg = (Container*)args;
    int s_1, s_2, e_1, e_2; 
    for(int i = arg->start_end.first; i < arg->start_end.second; i++){
        s_1 = i; while((*(arg->s_elems)).find(s_1) == (*(arg->s_elems)).end()) s_1--;
        e_1 = (*(arg->adj_csc)).colstrelem[(*(arg->s_elems)).find(s_1)->second + 1];
        s_2 = (*(arg->adj_csc)).colstrelem[(*(arg->adj_csc)).rowind[i]];
        e_2 = (*(arg->adj_csc)).colstrelem[(*(arg->adj_csc)).rowind[i] + 1];
        while(s_1 != e_1 && s_2 != e_2){
            if((*(arg->adj_csc)).rowind[s_1] > (*(arg->adj_csc)).rowind[s_2]) s_2++;
            else if((*(arg->adj_csc)).rowind[s_1] < (*(arg->adj_csc)).rowind[s_2]) s_1++;
            else { number_of_triangles++; s_1++; s_2++; } 
        }
    }
    return (void*)number_of_triangles;
}

long int pthreads_get_number_of_triangles(adjacency_csc& adj_csc){
    long int number_of_triangles = 0;
    unordered_map<int, int> s_elems; // a hash_map to improve time complexity
    for(int i = 0; i < adj_csc.dim+1; i++) s_elems[adj_csc.colstrelem[i]]=i; // pairs of <idx, colstrelem[idx]>

    //construct the container
    Container *arg = new Container[number_of_threads];
    int block_size = adj_csc.n_nnz / number_of_threads;
    for(int i = 0; i < number_of_threads; i++){
        arg[i].adj_csc = &adj_csc;
        arg[i].s_elems = &s_elems;
        arg[i].start_end.first = i*block_size;
        arg[i].start_end.second = (i+1)*block_size;
    }
    arg[number_of_threads - 1].start_end.second = adj_csc.n_nnz; // block_size + (n_nnz % number_of_threads) 

    pthread_t t[number_of_threads];
    for(int i = 0; i < number_of_threads; i++)
        pthread_create(&t[i], NULL, number_of_triangles_in_submatrix, (void*)&arg[i]);
    void* result;
    for(int i = 0; i < number_of_threads; i++){
        pthread_join(t[i], &result);
        number_of_triangles += (long int)result;
    }

    return number_of_triangles;
}

int main(int argc, char** argv){
    if(argc != 2) exit(1);
    string filename = argv[1];

    srand(time_t(time(NULL)));
    chrono::high_resolution_clock::time_point start, end;

    int* dim = new int;
    int* n_nnz = new int;
    int* rowind;
    int* colstrelem;
    input_data(filename, colstrelem, rowind, dim, n_nnz);

    adjacency_csc adj_csc(*dim, *n_nnz, rowind, colstrelem);
        
    start = chrono::high_resolution_clock::now();
    cout << pthreads_get_number_of_triangles(adj_csc) << endl;
    end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::duration<double>>(end - start).count() << endl;       
}