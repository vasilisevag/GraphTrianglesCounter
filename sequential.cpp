// sequential implementation
#include "headers.h"
#include "containers.h"
#include "helper_functions.h"
using namespace std;

long int sequential_get_number_of_triangles(const adjacency_csc& adj_csc){
    int number_of_triangles = 0;
    unordered_map<int, int> s_elems; // a hash_map to improve time complexity
    for(int i = 0; i < adj_csc.dim+1; i++) s_elems[adj_csc.colstrelem[i]]=i; // pairs of <idx, colstrelem[idx]>

    int s_1, s_2, e_1, e_2;
    for(int i = 0; i < adj_csc.n_nnz; i++){ 
        s_1 = i; while(s_elems.find(s_1) == s_elems.end()) s_1--;
        e_1 = adj_csc.colstrelem[s_elems.find(s_1)->second + 1];
        s_2 = adj_csc.colstrelem[adj_csc.rowind[i]];
        e_2 = adj_csc.colstrelem[adj_csc.rowind[i] + 1];
        while(s_1 != e_1 && s_2 != e_2){
            if(adj_csc.rowind[s_1] > adj_csc.rowind[s_2]) s_2++;
            else if(adj_csc.rowind[s_1] < adj_csc.rowind[s_2]) s_1++;
            else { number_of_triangles++; s_1++; s_2++; } 
        }
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
    cout << sequential_get_number_of_triangles(adj_csc) << endl;
    end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::duration<double>>(end - start).count() << endl;       
}