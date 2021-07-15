#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "mm.h"



// Task 1: Flush the cache so that we can do our measurement :)
void flush_all_caches(){
	// Your code goes here
	
	// from stack overflow link 1 in ref; not sure if this work
	for (long k = 0; k < (row * col) ; k++){
        asm volatile ("clflush (%0)\n\t"
                :
                : "r"(huge_matrixA + k)
                : "memory");
        asm volatile ("clflush (%0)\n\t"
                :
                : "r"(huge_matrixB + k)
                : "memory");
        asm volatile ("clflush (%0)\n\t"
                :
                : "r"(huge_matrixC + k)
                : "memory");
    }
    asm volatile ("sfence\n\t"
            :
            :
            : "memory");

}


void load_matrix_base()
{
	long i;
	huge_matrixA = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
	huge_matrixB = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
	huge_matrixC = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
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

void multiply_base()
{
	// Your code here
	//s
	// Implement your baseline matrix multiply here.
	for(long n = 0; n < row; n++){
		for(long m = 0; m < col; m++){
			long sum = 0;

			for(long i = 0; i < col; i++){
				sum += ( huge_matrixA[ (row * n) + i] * huge_matrixB[ (i * col) + col] );
			}
			huge_matrixC[ (n * row) + m ] = sum;
		}
	}

	
}

void compare_results()
{
	fout = fopen("./out.in","r");
	long i;
	long temp1, temp2;
	for(i=0;i<((long)SIZEX*(long)SIZEY);i++)
	{
		fscanf(fout, "%ld", &temp1);
		fscanf(fout, "%ld", &temp2);
		if(temp1!=temp2)
		{
			printf("Wrong solution!");
			exit(1);
		}
	}
	fclose(fout);
	fclose(ftest);
}

void write_results()
{
	// Your code here
	//
	// Basically, make sure the result is written on fout
	// Each line represent value in the X-dimension of your matrix
}

void load_matrix()
{
	// Your code here
}



void multiply()
{
	// Your code here
}

int main()
{
	printf("1\n");
	clock_t s,t;
	double total_in_base = 0.0;
	double total_in_your = 0.0;
	double total_mul_base = 0.0;
	double total_mul_your = 0.0;
	fin1 = fopen("./input1.in","r");
	fin2 = fopen("./input2.in","r");
	fout = fopen("./out.in","w");
	ftest = fopen("./reference.in","r");
	printf("2\n");
	

	flush_all_caches();
	printf("3\n");

	s = clock();
	load_matrix_base();
	t = clock();
	total_in_base += ((double)t-(double)s) / CLOCKS_PER_SEC;
	printf("[Baseline] Total time taken during the load = %f seconds\n", total_in_base);

	s = clock();
	multiply_base();
	t = clock();
	total_mul_base += ((double)t-(double)s) / CLOCKS_PER_SEC;
	printf("[Baseline] Total time taken during the multiply = %f seconds\n", total_mul_base);
	fclose(fin1);
	fclose(fin2);
	fclose(fout);
	free_all();

	flush_all_caches();

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


*/