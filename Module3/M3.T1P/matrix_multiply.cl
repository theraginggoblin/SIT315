// this kernel runs the inner most for loop of traditional matrix multiply. arrayA is entire first array to be multiplied. arrayB is entire 2nd matrix to be multiplied. arrayC is result array/buffer
__kernel void matrix_multiply(const int matrixSize, const int startRow, const __global long* arrayA, const __global long* arrayB, __global long* arrayC) {
    // Thread identifiers
    const int globalIndexDim1 = get_global_id(0); // dim 1 id
    const int globalIndexRow = globalIndexDim1 + startRow; // is row index
    const int globalIndexColumn = get_global_id(1); // dim 2 id is column index
    // index for placement in return buffer
    int resultIndex = (globalIndexDim1 * matrixSize) + globalIndexColumn;

    //uncomment to troubleshoot
    //printf("Kernel process index row  :(%d)  index column : (%d)\n ", globalIndexDim1, globalIndexColumn);
    //printf("matrixSize: %d", matrixSize);
    //printf("start row: %d", startRow);

    // using private variable to minimise global variable access as it's faster
    // this is the inner most loop of matrix multiplication
    long result = 0;
    for (int offset = 0; offset < matrixSize; offset++) {
        result += arrayA[globalIndexColumn + (offset * matrixSize)] * arrayB[(globalIndexRow * matrixSize) + offset];
    }
    arrayC[resultIndex] = result;

    // uncomment to troubleshoot
    //printf("from row: %d index %d result %d \n", startRow, resultIndex, arrayC[resultIndex]);
}
