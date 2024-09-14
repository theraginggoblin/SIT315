#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <fstream>
#include <mpi.h>
#include <cmath>
#include <array>
#include <CL/cl.h>

using namespace std::chrono;
using namespace std;

// root rank is rank that will send/receive data and write outputs. aka the master rank
const int rootRank = 0;
const int matrixSize = 1000;
// tag for sending data out and then data in - used with MPI_Send and MPI_Recv
const int dataOutTag = 1; // used when sending parts of data to worker processors
const int dataInTag = 2; // used to receive from worker processors
// going global as hitting max stack size - because not using heap
long arrayA[matrixSize][matrixSize];
long arrayB[matrixSize][matrixSize];
long arrayC[matrixSize][matrixSize];
// long *resultArray;
// used a few times so may as well calculate once
long matrixSizeSquared = matrixSize * matrixSize;

// opencl variables
// first 2 buffers are for v1 and v2. v3 is for holding the result
cl_mem bufferA;
cl_mem bufferB;
cl_mem bufferC;
// this is a device id variable where the result of create_device is later stored
cl_device_id device_id;
// context object for this host program
cl_context context;
// variable for program that's built on 32
cl_program program;
// variable for the kernel function
cl_kernel kernel;
// command queue variable used by device
cl_command_queue queue;
cl_event event = NULL;
int err;

// open cl function declarations
// this function tries to get a GPU device if available, otherwise tries to get a CPU device
cl_device_id create_device();
// this function contains code to build the host program, the kernel function, context and set device
void setup_openCL_device_context_queue_kernel(char *filename, char *kernelname);
// this function builds the program including kernel
cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename);
// creates buffer to write to device then enqueues command to load data to it
void setup_kernel_memory();
// uses clSetKernelArg to copy arguments to kernel function
void copy_kernel_args();
// releases the command queue, kernel, program and queue
void free_memory();

void free_memory() {
    //free the buffers
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);

    //free opencl objects
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);
}

void copy_kernel_args(int startRow) {
    // kernel needs matrix size for inner most for loop so its the first arg
    // 
    clSetKernelArg(kernel, 0, sizeof(int), (void *)&matrixSize);
    clSetKernelArg(kernel, 1, sizeof(int), (void *)&startRow);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bufferA);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&bufferB);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&bufferC);

    if (err < 0)
    {
        perror("Couldn't create a kernel argument");
        printf("error = %d", err);
        exit(1);
    }
}

// setup buffer memory on device
void setup_kernel_memory(int partitionSize, int startRow) {
    // copying matrices across
    bufferA = clCreateBuffer(context, CL_MEM_READ_WRITE, matrixSizeSquared * sizeof(long), NULL, NULL);
    bufferB = clCreateBuffer(context, CL_MEM_READ_WRITE, matrixSizeSquared * sizeof(long), NULL, NULL);
    bufferC = clCreateBuffer(context, CL_MEM_READ_WRITE, partitionSize * matrixSize * sizeof(long), NULL, NULL);

    // Copy matrices
    clEnqueueWriteBuffer(queue, bufferA, CL_TRUE, 0, matrixSizeSquared * sizeof(long), &arrayA[0], 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferB, CL_TRUE, 0, matrixSizeSquared * sizeof(long), &arrayB[0], 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferC, CL_TRUE, 0, partitionSize * matrixSize * sizeof(long), &arrayC[startRow][0], 0, NULL, NULL);
}

// set up device, program, command queue and kernel
void setup_openCL_device_context_queue_kernel(char *filename, char *kernelname) {
    device_id = create_device();
    cl_int err;

    // creates contrxt for the device specified by device_id
    // can be to multiple devices but it's only one as specifed by the variable
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    if (err < 0)
    {
        perror("Couldn't create a context");
        exit(1);
    }

    program = build_program(context, device_id, filename);

    // crate queue for context on device_id. 0 is properties and last is pointer to error variable to mutate if error occurs
    queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);
    if (err < 0)
    {
        perror("Couldn't create a command queue");
        exit(1);
    };


    kernel = clCreateKernel(program, kernelname, &err);
    if (err < 0)
    {
        perror("Couldn't create a kernel");
        printf("error =%d", err);
        exit(1);
    };
}

// loads and builds program from file
cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename) {

    cl_program program;
    FILE *program_handle;
    char *program_buffer, *program_log;
    size_t program_size, log_size;

    /* Read program file and place content into buffer */
    program_handle = fopen(filename, "r");
    if (program_handle == NULL)
    {
        perror("Couldn't find the program file");
        exit(1);
    }
    fseek(program_handle, 0, SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char *)malloc(program_size + 1);
    program_buffer[program_size] = '\0';
    fread(program_buffer, sizeof(char), program_size, program_handle);
    fclose(program_handle);

    // creates program with the source code from vector_ops.cl
    // each of the arguments is a pointer
    // program_buffer is the char array that was read and is the source code
    // program_size is size of the char array
    // err variable is int used to represent any errors that occur during program creation
    program = clCreateProgramWithSource(ctx, 1,
                                        (const char **)&program_buffer, &program_size, &err);
    if (err < 0)
    {
        perror("Couldn't create the program");
        exit(1);
    }
    free(program_buffer);

    /* Build program 

   The fourth parameter accepts options that configure the compilation. 
   These are similar to the flags used by gcc. For example, you can 
   define a macro with the option -DMACRO=VALUE and turn off optimization 
   with -cl-opt-disable.
   */
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err < 0)
    {

        /* Find size of log and print to std output */
        clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
                              0, NULL, &log_size);
        program_log = (char *)malloc(log_size + 1);
        program_log[log_size] = '\0';
        clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
                              log_size + 1, program_log, NULL);
        printf("%s\n", program_log);
        free(program_log);
        exit(1);
    }

    return program;
}

// get a cl device - gpu if available, else gets cpu object. also includes error raising if a device is not available
cl_device_id create_device() {
   cl_platform_id platform;
   cl_device_id dev;
   int err;

   /* Identify a platform */
   err = clGetPlatformIDs(1, &platform, NULL);
   if(err < 0) {
      perror("Couldn't identify a platform");
      exit(1);
   } 

   // Access a device
   // GPU
   err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
   if(err == CL_DEVICE_NOT_FOUND) {
      // CPU
      printf("GPU not found\n");
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
   }
   if(err < 0) {
      perror("Couldn't access any devices");
      exit(1);   
   }

   return dev;
}

void openclMatrixMultiply(int partitionSize, int startRow) {
    // uncomment to troubleshoot
    //printf("rank %d partition size %d", startRow, partitionSize);

    // using 2 dimenions - partition size as we only work on this partition from Array A
    // 2nd dimension is matrixSize as we need entire size from arrayB - columns
    size_t global[2] = {(size_t)partitionSize, (size_t)matrixSize};
    // perform setup
    setup_openCL_device_context_queue_kernel((char *)"./matrix_multiply.cl", (char *)"matrix_multiply");
    setup_kernel_memory(partitionSize, startRow);
    copy_kernel_args(startRow);

    // enqueue kernel command and wait on it, then read return
    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global, NULL, 0, NULL, &event);
    clWaitForEvents(1, &event);
    clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, partitionSize * matrixSize * sizeof(long), &arrayC[startRow][0], 0, NULL, NULL);
}

void randomMatrix(long matrix[][matrixSize]) {
    for (int row = 0; row < matrixSize; row++) {
        for (int column = 0; column < matrixSize; column++) {
        // get random number modulo 100 and set at location row-column in matrix
            matrix[row][column] = rand() % 100;
        }
    }
}

/*
    Returns an array of ints containing a number of elements equalling numPartitions with
    each element being the length of a partition. Ensures remainder is as evenly distributed as
    possible.

    IE pass in 10 and 3
    return is [4, 4, 3]
*/
int* getPartitions(int length, int numPartitions) {
    int partition_size = floor(length / numPartitions);
    int remainder = length % numPartitions;
    int *arr = (int *) malloc(numPartitions * sizeof(int *));

    for (int i = 0; i < numPartitions; i++) {
        arr[i] = partition_size;
        if (remainder > 0) {
            arr[i]++;
            remainder--;
        }
    }

    return arr;
}

int getStartRow(int* partitions, int rank) {
    // lets say partitions is 4 4 3 for 3 ranks
    // ranks are 0 1 2
    // rows would be 0 to 3, 4 to 7, 8 to 11
    // starts are 0, 4, 8
    // rank 0 for loop doesn't trigger, so get 0
    // rank 1 for loop runs once meaning 0 + partition[0] = 4
    // rank 2 for loop run twice meaning 0 + partition[0] + partition[1]
    int start = 0;
    for (int i = 0; i < rank; i++) {
        start += partitions[i];
    }
    return start;
}

int getEndRow(int* partitions, int rank) {
    // ends are start of the next row - we'll be checking we're below them
    // lets say partitions is 4 4 3 for 3 ranks
    // so if ranks are 0 1 2
    // ends are 4, 8, 11
    int end = 0;
    for (int i = 0; i <= rank; i++) {
        end += partitions[i];
    }
    return end;
}

// create an output file with the result matrix. column values are separated by commas and rows by newlines
void createOutputFile(string fileName, long matrix[][matrixSize]) {
    ofstream outputFileStream(fileName);
    if (outputFileStream.is_open()) {
        int row, column;
    
        for (row = 0; row < matrixSize; row++) {
            for (column = 0; column < matrixSize; column++) {
                outputFileStream << matrix[row][column];
            
                if (column != matrixSize - 1) {
                    outputFileStream << ",";
                }
            }
            if (row != matrixSize - 1) {
                outputFileStream << "\n";
            }
        }
        outputFileStream.close();
    }
}

int main() {
    // initialisation/setup actions
    srand(time(0));

    // init mpi environment
    MPI_Status status;
    MPI_Init(NULL, NULL);

    char processorName[MPI_MAX_PROCESSOR_NAME];
    int nameLength;
    // get name of current processor
    MPI_Get_processor_name(processorName, &nameLength);

    // world is all processors / num of processors
    int worldSize;
    // MPI_COMM_WORLD is default communicator containing all processes available to use
    // MPI_Comm_size gets number of processes in communicator and will set worldSize
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    // rank is world/global rank of processor
    int rank;
    // get rank of calling process
    // takes in default communicator. will set rank to id of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // treating all non root ranks as workers
    int numProcessors = worldSize;
    int numWorkerProcessors = worldSize - 1;

    // if matrix size is 4
    // and num processors = 5
    // then we more processors than partitions
    // means when partitioning a worker will end up with no work so syntax error
    // reduce num workers to avoid that
    if (numProcessors > matrixSize) {
        numProcessors = matrixSize;
        numWorkerProcessors = matrixSize - 1;
    }

    // processor says hello
    printf("Hello world from processor %s, rank %d out of %d processors\n", processorName, rank, worldSize);
    double startTime = MPI_Wtime();

    // partitions is rows per processor/rank
    int* partitions = getPartitions(matrixSize, numProcessors);
    // startRow and endRow are local processor/rank start and end rows
    int startRow = getStartRow(partitions, rank);
    int endRow = getEndRow(partitions, rank);

    if (rank == rootRank) {
        printf("Application is opencl mpi matrix multiplication.\n");
        // if root rank setup the two random matrices containing the values for matrix multiplication
        randomMatrix(arrayA);
        randomMatrix(arrayB);

        int destinationStartRow;

        // iterate through all of the workers and send each the matrices
        for (int destinationProcessor = 1; destinationProcessor <= numWorkerProcessors; destinationProcessor++) {
            // send entire matrices to the other processors
            // printf("sending data to rank %d", destinationProcessor);
            MPI_Send(&arrayA, matrixSizeSquared, MPI_LONG, destinationProcessor, dataOutTag, MPI_COMM_WORLD);
            MPI_Send(&arrayB, matrixSizeSquared, MPI_LONG, destinationProcessor, dataOutTag, MPI_COMM_WORLD);
            // printf("done sending data to rank %d", destinationProcessor);
        }
    }

    if (rank > rootRank) {
        MPI_Recv(&arrayA, matrixSizeSquared, MPI_LONG, rootRank, dataOutTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&arrayB, matrixSizeSquared, MPI_LONG, rootRank, dataOutTag, MPI_COMM_WORLD, &status);
    }

    // sync all processes
    MPI_Barrier(MPI_COMM_WORLD);

    // performance matrix multiplication on partition for rank - uses opencl
    openclMatrixMultiply(partitions[rank], startRow);

    // sync all processes
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank > rootRank) {
        // printf("sending from rank %d, size %d", rank, partitions[rank] * matrixSize);
        MPI_Send(&arrayC[startRow][0], partitions[rank] * matrixSize, MPI_LONG, rootRank, dataInTag, MPI_COMM_WORLD);
        // printf("sent from rank %d", rank);
    }

    if (rank == rootRank) {
        // retrieve results from workers
        for (int workerProcessor = 1; workerProcessor <= numWorkerProcessors; workerProcessor++) {
            startRow = getStartRow(partitions, workerProcessor);
            MPI_Recv(&arrayC[startRow][0], partitions[workerProcessor] * matrixSize, MPI_LONG, workerProcessor, dataInTag, MPI_COMM_WORLD, &status);
        }
    
        double duration = MPI_Wtime() - startTime;
        printf("Program took %f seconds\n", duration);
        createOutputFile("output_matrixC_mpiopencl.txt", arrayC);
    }

    // sync all processes
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    delete partitions;
    return 0;
}
