// this kernel function takes in vectors 1 and 2 which have values, then adds them and puts result in v3. which element this kernel works on is determined by global id
__kernel void square_magnitude(__global int* v1, __global int* v2, __global int* v3) {
    // Thread identifiers
    const int globalIndex = get_global_id(0);   
 
    //uncomment to see the index each PE works on
    printf("Kernel process index :(%d)\n ", globalIndex);

    v3[globalIndex] = v1[globalIndex] + v2[globalIndex];
}
