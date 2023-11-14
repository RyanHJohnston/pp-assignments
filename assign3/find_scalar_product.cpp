#include <cstdio>
#include <cstdlib>

int main() {
    
    int i = 0;
    int a[4] = {0,1,2,3}; 
    int b[4] = {4,5,6,7};
    int scalar_product = 0;
    
    // execute the iteration of both vectors in parallel
    #pragma omp parallel for reduction(+: scalar_product)
    for (i = 0; i < 4; ++i) {
        scalar_product += a[i]*b[i]; // multipliy bits a[i]*b[i] and add it to the scalar_product
    }
    
    // print out the scalar product
    fprintf(stdout, "Final scalar_product is %d\n", scalar_product);
    
    return 0;
}
