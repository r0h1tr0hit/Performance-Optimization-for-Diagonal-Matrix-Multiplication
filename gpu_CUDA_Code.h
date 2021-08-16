#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
using namespace std;

#define TIME_NOW std::chrono::high_resolution_clock::now()
#define TIME_DIFF(gran, start, end) std::chrono::duration_cast<gran>(end - start).count()

__global__ void cuda_Version(int N,int *A, int *B, int *Out)
{
	
        int position = threadIdx.x + blockIdx.x * blockDim.x;
        //Calculating index of the current thread
        int rowA, colA, rowB, colB, terminate;

        if(position/N == 0)// Initializing the variables for first half of the output elements 
                rowA = 0 , colA = position, rowB = position , colB = N - 1, terminate = position + 1;
        else               // Initializing the variables for second half of the output elements 
                rowA = position + 1 - N, colA = N-1 , rowB = N-1 , colB = 2 * N - 2 - position, terminate = 2 * N - (position + 1);


        for(int j = 0; j < terminate; ++j)
                Out[position] += A[rowA++ * N + colA--] * B[rowB-- * N + colB--];
}




void gpuThread(int N, int *matA, int *matB, int *output)
{
        int *A,*B,*Out;//Pointers for the Device memory
        int input_Size = N*N*sizeof(int);
        int output_Size = (2*N-1)*sizeof(int);

        int total_Threads = 2*N-1;// Assigning one thread to each output element 
        int threads_perBlock = 32;// Assigning 32 threads per block
        int total_Blocks = (total_Threads+31)/threads_perBlock;

	// Allocating memory into Device 
        cudaMalloc((void **)&A , input_Size);
        cudaMalloc((void **)&B , input_Size);
        cudaMalloc((void **)&Out , output_Size);

        // Copying the input data from Host memory to Device memory 
        cudaMemcpy(A, matA, input_Size, cudaMemcpyHostToDevice);
        cudaMemcpy(B, matB, input_Size, cudaMemcpyHostToDevice);
	
	
	//Launching kernel and executing GPU program and calculating time for execution
        auto begin = TIME_NOW;
        cuda_Version<<< total_Blocks , threads_perBlock>>>(N, A, B, Out);
	auto end = TIME_NOW;
	cout << "Cuda Version execution time: " << (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n";

        //Copying the results from GPU memory to CPU memory
        cudaMemcpy(output, Out, output_Size, cudaMemcpyDeviceToHost);

	//Freeing the Device memory
        cudaFree(A);
        cudaFree(B);
        cudaFree(Out);
}
