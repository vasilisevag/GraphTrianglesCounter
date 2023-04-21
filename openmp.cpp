//openMP implementation
#include "headers.h"
#include "containers.h"
#include "helper_functions.h"
using namespace std;

long int openMP_get_number_of_triangles(const adjacency_csc& adj_csc){
    vector<int> msked_prd(adj_csc.n_nnz, 0);
    vector<int> tr_inc_to_vert = vector<int>(adj_csc.dim, 0);
    unordered_map<int, int> s_elems; // a hash_map to improve time complexity
    for(int i = 0; i < adj_csc.dim+1; i++) s_elems[adj_csc.colstrelem[i]]=i; // pairs of <idx, colstrelem[idx]>

    int s_1, s_2, e_1, e_2, i;
    #pragma omp parallel for shared(adj_csc, s_elems, msked_prd) private(s_1, s_2, e_1, e_2, i) 
        for(i = 0; i < adj_csc.n_nnz; i++){ 
            s_1 = i; while(s_elems.find(s_1) == s_elems.end()) s_1--;
            e_1 = adj_csc.colstrelem[s_elems.find(s_1)->second + 1];
            s_2 = adj_csc.colstrelem[adj_csc.rowind[i]];
            e_2 = adj_csc.colstrelem[adj_csc.rowind[i] + 1];
            while(s_1 != e_1 && s_2 != e_2){
                if(adj_csc.rowind[s_1] > adj_csc.rowind[s_2]) s_2++;
                else if(adj_csc.rowind[s_1] < adj_csc.rowind[s_2]) s_1++;
                else { msked_prd[i]++; s_1++; s_2++; } 
            }
        }

    // our msked_prd got some zero values but in this way we reduce code complexity!
    // now we calculate the number of triangles
    int j;
    #pragma omp parallel for shared(adj_csc, tr_inc_to_vert, msked_prd) private(i, j)
        for(i = 0; i < adj_csc.dim; i++){
            for(j = adj_csc.colstrelem[i]; j < adj_csc.colstrelem[i + 1]; j++)
                tr_inc_to_vert[i] += msked_prd[j];
        }

    long int number_of_triangles = 0;
    for(int nft : tr_inc_to_vert)
        number_of_triangles += nft;

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
    cout << openMP_get_number_of_triangles(adj_csc) << endl;
    end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::duration<double>>(end - start).count() << endl;       
}