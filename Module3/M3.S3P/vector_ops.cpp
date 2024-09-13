#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <time.h>
#include <chrono>
#include <iostream>
using namespace std::chrono;
using namespace std;

#define PRINT 1

int size = 10000;
// v1 and v2 are initalised with random values. v3 is results of vector add
int *v1, *v2, *v3;

// first 2 buffers are for v1 and v2. v3 is for holding the result
cl_mem bufferV1;
cl_mem bufferV2;
cl_mem bufferV3;


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

void init(int *&A, int size);
void print(int *A, int size);

void randomVector(int vector[], int size)
{
    for (int i = 0; i < size; i++)
    {
        // get random number modulo 100 and set at location i in array
        vector[i] = rand() % 100;
    }
}

int main(int argc, char **argv) {
    if (argc > 1)
        size = atoi(argv[1]);

    // allocate heap memory to the vectors
    v1 = (int *) malloc(size * sizeof(int *));
    v2 = (int *) malloc(size * sizeof(int *));
    v3 = (int *) malloc(size * sizeof(int *));
    // init v1 and v2 as random vectors
    randomVector(v1, size);
    randomVector(v2, size);

    // start timer
    auto start = high_resolution_clock::now();
    //  global size to size of size's type - which is int
    size_t global[1] = {(size_t)size};
    setup_openCL_device_context_queue_kernel((char *)"./vector_ops.cl", (char *)"square_magnitude");

    setup_kernel_memory();
    copy_kernel_args();

    // enqueue command to execute kernel.
    // queue is command queue. kernel is kernel to run. 1 is work_dim which is number of dimensions used.
    // first NULL is global work offset. this is an offset used to calculate global id of a work item
    // global is the global work size
    // 2nd null is local_work_size. has a default it uses. can be explicit set when not using NULL
    // 0 is num_events_in_wait_list which specify evenst that need to complete before running this command
    // null is for event_wait_list - these are the events to wait on but since is null none to wait on
    // event is event object updated by this command
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global, NULL, 0, NULL, &event);
    clWaitForEvents(1, &event);

    // enqueues a command to read from buffer to host
    // queue is the command queue to add command to
    // bufferV3 is buffer being read from
    // CL_TRUE is for blocking_read or not - blocks until done
    // 0 is offset
    // size * sizeof(int) is size of data being read from bufV
    // &v3[0] is for ptr - pointer to be written to - this is where the data from bufferV3 lands
    // 0 is num_events_in_wait_list
    // null event_wait_list so no event wait list
    // last null is event, no event is being set
    clEnqueueReadBuffer(queue, bufferV3, CL_TRUE, 0, size * sizeof(int), &v3[0], 0, NULL, NULL);

    auto stop = high_resolution_clock::now();
    // time taken to run = stop time minus start time, then cast to microseconds
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by opencl function: " << duration.count() << " microseconds" << endl;
    cout << "Aka time taken by opencl function: " << duration.count() / 1000 << " milliseconds" << endl;

    //result vector
    //print(v3, size);

    //frees memory for device, kernel, queue, etc.
    //you will need to modify this to free your own buffers
    free_memory();
}

void init(int *&A, int size)
{
    A = (int *)malloc(sizeof(int) * size);

    for (long i = 0; i < size; i++)
    {
        A[i] = rand() % 100; // any number less than 100
    }
}

void print(int *A, int size)
{
    if (PRINT == 0)
    {
        return;
    }

    if (PRINT == 1 && size > 15)
    {
        for (long i = 0; i < 5; i++)
        {                        //rows
            printf("%d ", A[i]); // print the cell value
        }
        printf(" ..... ");
        for (long i = size - 5; i < size; i++)
        {                        //rows
            printf("%d ", A[i]); // print the cell value
        }
    }
    else
    {
        for (long i = 0; i < size; i++)
        {                        //rows
            printf("%d ", A[i]); // print the cell value
        }
    }
    printf("\n----------------------------\n");
}

void free_memory()
{
    //free the buffers
    clReleaseMemObject(bufferV1);
    clReleaseMemObject(bufferV2);
    clReleaseMemObject(bufferV3);

    //free opencl objects
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);

    delete v1;
    delete v2;
    delete v3;
}


void copy_kernel_args() {
    // sets value for argument of a kernel
    // kernel is the kernel to setup argument for
    // 0 / 1 is location of argument
    // next is size of argument
    // final is the value to put in the argument
    //clSetKernelArg(kernel, 0, sizeof(int), (void *)&size);
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufferV1);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufferV2);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bufferV3);

    if (err < 0)
    {
        perror("Couldn't create a kernel argument");
        printf("error = %d", err);
        exit(1);
    }
}

void setup_kernel_memory() {
    // creates data buffer. context is context, CL_MEM_READ_WRITE means memory region is for read and write (it's also the default)
    // size * sizeof(int) is size of the buffer
    // first null is for the host_ptr arg which is pointer to memory that may already be allocated to application
    // final null is errcode_ret which is variable to put error code into
    bufferV1 = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(int), NULL, NULL);
    bufferV2 = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(int), NULL, NULL);
    bufferV3 = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(int), NULL, NULL);

    // Copy vectors
    clEnqueueWriteBuffer(queue, bufferV1, CL_TRUE, 0, size * sizeof(int), &v1[0], 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferV2, CL_TRUE, 0, size * sizeof(int), &v2[0], 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferV3, CL_TRUE, 0, size * sizeof(int), &v3[0], 0, NULL, NULL);
}

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

cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename)
{

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