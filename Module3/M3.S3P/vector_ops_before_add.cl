// This function takes in arguments of size and the vector v. it gets the global id and uses it as index in the vectors to do the multiplication
__kernel void square_magnitude(const int size,
                      __global int* v) {
    
    // Thread identifiers
    const int globalIndex = get_global_id(0);   
 
    //uncomment to see the index each PE works on
    //printf("Kernel process index :(%d)\n ", globalIndex);

    v[globalIndex] = v[globalIndex] * v[globalIndex];
}
