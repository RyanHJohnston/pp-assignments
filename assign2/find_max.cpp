#include <stdlib.h>
#include <stdio.h>

#define TRUE 0
#define FALSE 1

int find_max(int A[]);

int main(int argc, char *artv[]) {
    
    
    exit(EXIT_SUCCESS);
}

int find_max(int A[]) {
    int i = 1;
    int j = 1;
    const unsigned int A_length = sizeof(A)/sizeof(A[0]);
    int M[A_length];
    int max_value = -1;
    
    /* set all the values in the memory scalar to TRUE */
    for (i = 0; i < A_length - 1; ++i) {
        M[i] = TRUE;
    }
    
    #pragma message("This is a compiler message")
    /* element comparisons in parallel */
    for (i = 0; i < A_length - 1; ++i) {
        if (A[i] < A[j]) {
            
        }
    }
    
    for (j = 0; j < A_length - 1; ++j) {

    }

    

    return max_value;
}
