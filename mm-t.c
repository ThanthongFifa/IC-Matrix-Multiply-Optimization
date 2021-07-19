#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include "mm-t.h"


// Task 1: Flush the cache so that we can do our measurement :)
void flush_all_caches(){
	// Your code goes here
	//printf("A: %ls\nB: %ls\nC: %ls\n", huge_matrixA,huge_matrixB,huge_matrixC);
	
	for (long i = 0; i < (row * col) ; i++){

        asm volatile ("clflush (%0)\n\t":: "r"(huge_matrixA + i) : "memory");

        asm volatile ("clflush (%0)\n\t" :: "r"(huge_matrixB + i) : "memory");

        asm volatile ("clflush (%0)\n\t" :: "r"(huge_matrixC + i) : "memory");

    }
	
    asm volatile ("sfence\n\t" ::: "memory");
}

void load_matrix_base()
{
	long i;
	huge_matrixA = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
	huge_matrixB = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
	huge_matrixC = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);

	//printf("A: %ls\nB: %ls\nC: %ls\n", huge_matrixA,huge_matrixB,huge_matrixC);

	// Load the input
	// Note: This is suboptimal because each of these loads can be done in parallel.
	for(i=0;i<((long)SIZEX*(long)SIZEY);i++)
	{
		fscanf(fin1,"%ld", (huge_matrixA+i)); 	
		fscanf(fin2,"%ld", (huge_matrixB+i)); 
		huge_matrixC[i] = 0;		
	}
}

void free_all()
{
	free(huge_matrixA);
	free(huge_matrixB);
	free(huge_matrixC);
}

void multiply_base(){
	for(long i = 0; i < row; i++){ //loop for SIZEX
		for(long j = 0; j < col; j++){ // loop for SIZEY

			long num = 0;

			for(long k = 0; k < col; k++){
				num += ( huge_matrixA[ (row * i) + k] * huge_matrixB[ (k * col) + col] );
			}

			huge_matrixC[ (i * row) + j ] = num;
		}
	}
}

void compare_results()
{
	fout = fopen("./out.in","r");
	ftest = fopen("./reference.in","r");

	long i;
	long temp1, temp2;
	for(i=0;i<((long)SIZEX*(long)SIZEY);i++)
	{
		fscanf(fout, "%ld", &temp1);
		fscanf(ftest, "%ld", &temp2);
		if(temp1!=temp2)
		{
			printf("Wrong solution!\n");
			exit(1);
		}
	}
	fclose(fout);
	fclose(ftest);
}

void write_results(){
	// Your code here
	//
	// Basically, make sure the result is written on fout
	// Each line represent value in the X-dimension of your matrix

	char result[BUF];
	fout = fopen("./out.in","w");

	// read from matrixC and write in fout
	for( long i = 0; i < row; i++){
		for( long j = 0; j < col; j++){

			memset(result, 0, BUF);
			long pos = (i * row) + j;

			sprintf(result, "%ld ", huge_matrixC[pos]);
			fwrite(result, sizeof(char), strlen(result), fout); //write a line to fout
			
		}
		char* end = "\n"; // end of line
		fwrite(end, sizeof(char), strlen(end), fout); //write new line
	}
}

void load_matrix() // copy of load_matrix_base() (for now)
{
	// Your code here
	long i;
	huge_matrixA = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
	huge_matrixB = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
	huge_matrixC = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
	// Load the input
	// Note: This is suboptimal because each of these loads can be done in parallel.
	for(i=0;i<((long)SIZEX*(long)SIZEY);i++)
	{
		fscanf(fin1,"%ld", (huge_matrixB+i)); 		
		fscanf(fin2,"%ld", (huge_matrixA+i)); 		
		huge_matrixC[i] = 0;		
	}
}

void multiply(){
	long blockSize = 200; // block size

	for(long bi = 0; bi < row; bi += blockSize){
		for(long bj = 0; bj < col; bj += blockSize){

			for(long bk = 0; bk < row; bk += blockSize){

				for(long i = bi; i < blockSize + bi; i++){
					for(long j = bj; j < blockSize + bj; j++){

						for(long k = bk; k < blockSize + bk; k++){
							huge_matrixC[(i * row) + j] += huge_matrixB[(i * row) + k] * huge_matrixA[(k * row) + j];
						}
					}
				} 
			} 
		} 
	}
}

void pm(long* m){
    for (long i = 0; i < row * col; i++)
    {
        if (i % (long)SIZEX == 0)
        {
            printf("\n");
        }
        printf("%ld ", m[i]);
    }
    printf("\n");
}

//===================== Thread Zone ======================

//========================================================
int main()
{
	
	clock_t s,t;
	double total_in_base = 0.0;
	double total_in_your = 0.0;
	double total_mul_base = 0.0;
	double total_mul_your = 0.0;
	fin1 = fopen("./input1.in","r");
	fin2 = fopen("./input2.in","r");
	fout = fopen("./out.in","w");
	ftest = fopen("./reference.in","r");


	//flush_all_caches();
	

	s = clock();
	load_matrix_base();
	//printf("%ld",huge_matrixA[1]);
	t = clock();
	total_in_base += ((double)t-(double)s) / CLOCKS_PER_SEC;
	printf("[Baseline] Total time taken during the load = %f seconds\n", total_in_base);

	s = clock();
	multiply_base();
	t = clock();
	total_mul_base += ((double)t-(double)s) / CLOCKS_PER_SEC;
	printf("[Baseline] Total time taken during the multiply = %f seconds\n", total_mul_base);
	//pm(huge_matrixC);
	fclose(fin1);
	fclose(fin2);
	fclose(fout);
	//free_all();

	flush_all_caches();
	//pm(huge_matrixA);
	free_all();
	//pm(huge_matrixA);

	fin1 = fopen("./input1.in","r");
	fin2 = fopen("./input2.in","r");
	fout = fopen("./out.in","w");
	ftest = fopen("./reference.in","r");
	

	s = clock();
	load_matrix();
	t = clock();
	total_in_your += ((double)t-(double)s) / CLOCKS_PER_SEC;
	printf("Total time taken during the load = %f seconds\n", total_in_your);

	s = clock();
	multiply();
	t = clock();
	total_mul_your += ((double)t-(double)s) / CLOCKS_PER_SEC;
	printf("Total time taken during the multiply = %f seconds\n", total_mul_your);
	//pm(huge_matrixC);
	write_results();
	fclose(fin1);
	fclose(fin2);
	fclose(fout);
	free_all();
	compare_results();

	return 0;
}


/*=============== collaborator ===============
- Vanessa Rujipatanakul 6280204

================= ref ===============
flush all caches
	https://stackoverflow.com/questions/11277984/how-to-flush-the-cpu-cache-in-linux-from-a-c-program
	https://stackoverflow.com/questions/39448276/how-to-use-clflush

Blocked Matrix Multiplication
	https://gist.github.com/metallurgix/8ee5262ed818730b5cc0
	https://www.youtube.com/watch?v=5MFWywYY9bE
	https://github.com/PatoBeltran/hpmmm/blob/master/matrixmultiply.c
	https://malithjayaweera.com/2020/07/blocked-matrix-multiplication/
	https://stackoverflow.com/questions/16115770/block-matrix-multiplication


*/