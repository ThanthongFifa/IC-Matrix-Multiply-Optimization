#define SIZEX 1000
#define SIZEY 1000

long * huge_matrixA;
long * huge_matrixB;
long * huge_matrixC;

FILE *fin1, *fin2, *fout, *ftest;

void flush_all_caches();
void load_matrix_base();
void free_all();
void multiply_base();
void compare_results();

// Your job is to finish these three functions in the way that it is fast
void write_results();
void load_matrix();
void multiply();

// Any additional variables needed go here
#define BUF 256
#define INDEX(r, c) ((c) + (r) * SIZEX)
long row = (long)SIZEX; // they are here because i dont waht to write them over and over
long col = (long)SIZEY;

// Any helper functions go here
void pm(); //print matrix

