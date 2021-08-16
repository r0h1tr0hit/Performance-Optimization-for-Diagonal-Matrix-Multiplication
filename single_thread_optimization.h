void singleThread(int N, int *matA, int *matB, int *output)
{
    int block_size = 64;
    int Bn = N/64;// Dividing the matrix A and B into multiple blocks of size block_size * block_size
    
   for(int i = 0,j=Bn-1 ; i <Bn && j>=0 ;i++,j--)//Iterating for each block of matrix A with the corresponding 
                                                 //block of matrix B
   { 
     int rowA = i*block_size, colB = j*block_size;
     for(int m= 0,n=0; m<Bn && n <Bn;m++,n++)
     { 
     	int rowB = n*block_size, colA = m*block_size;
     	for(int a=0,d=block_size-1; a<block_size && d>=0 ;a++,d--)// Performing the multiplication within the elements of the block B(i,m)
     	{    	                                                  // of matrix A and block B(n,j) of matrix B
           for(int b =0 , c=0;b<block_size;b++,c++)
           {
             output[rowA + a + colA + b]+= matA[(rowA+a )* N + colA + b] * matB[(rowB+c)*N + colB + d];
           } 
       }
     }
   }
}
