#include <pthread.h>

int block_size = 16;
int total_threads =64;
int *A;
int *B;
int size;
int Bn;


struct arg_struct // Creating the structure for passing the arguments in threads
{
    int thread_ID;
    int* out;
};

void* multi(void* arg){
    struct arg_struct *args = (struct arg_struct*) arg;
    (args->out) = (int*)malloc((2*size-1)*sizeof(int)); 
    
    for(int i = 0,j=Bn-1 ; i <Bn && j>=0 ;i++,j--)
   { 
     int rowA = i*block_size, colB = j*block_size;
     for(int m= 0,n=0; m<Bn && n <Bn;m++,n++)
     { 
     	if((m%total_threads) == args->thread_ID && (n%total_threads) == args->thread_ID)
     	{
     		int rowB = n*block_size, colA = m*block_size;
     		for(int a=0,d=block_size-1; a<block_size && d>=0 ;a++,d--){    	
           		for(int b =0 , c=0;b<block_size;b++,c++){
             			
             			*((args->out)+rowA+a+colA+b) += A[(rowA+a) * size + (colA+b)] * B[(rowB+c) * size + (colB+d)];
        		}	
       	}
       }
     }
    
   }
   pthread_exit(0);
}



void multiThread(int N, int *matA, int *matB, int *output)
{
   for(int i = 0;i < 2*N-1;i++){
       output[i] = 0;
   }
   size = N;
   Bn = size/block_size;
   A = matA;
   B = matB;
   struct arg_struct args[total_threads];// Creating the structure args array for storing the arguments for multiple threads.
   
   pthread_t threads[total_threads]; // Creating an array of threads
   for(int i = 0 ; i < total_threads;i++)
   {
      args[i].thread_ID = i;
      pthread_create(&threads[i],NULL,multi,&args[i]); // Creating thread i and calling it with multi function
   }     
   for(int i = 0; i<total_threads;i++){
    	pthread_join(threads[i],NULL); // waiting for other threads to come so that synchronization is possible.
   	for(int j = 0;j<2*N-1;j++){
      		output[j] += args[i].out[j];
      	}
   }  
}
