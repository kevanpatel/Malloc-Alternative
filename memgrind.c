#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

int main(){
    clock_t t;
    double time_taken[5] = {0,0,0,0,0};

    // Test 1
    for(int x = 0; x < 50; x++){
        t = clock();
        for(int i = 0; i < 120; i++){
            char *p = malloc(1);
            free(p);
        }
        t = clock() - t;
        time_taken[0] += ((double)t)/CLOCKS_PER_SEC;
        
    
        
        // Test 2
        char *arr[120];
        t = clock();
        for(int i = 0; i < 120; i++){
            arr[i] = malloc(1);
        }
        for(int i = 0; i < 120; i++){
            free(arr[i]);
        }
        t = clock() - t;
        time_taken[1] += ((double)t)/CLOCKS_PER_SEC;

        // Test 3
        srand(time(NULL));
        int allocs = 0, frees = 0;
        t = clock();
        while(allocs < 120){
            if(rand() % 2 == 0){
                arr[allocs] = malloc(1);
                allocs++;
            } else {
                if(frees < allocs){
                    free(arr[frees]);
                    frees++;
                }
            }
        }
        for(int i = frees; i < 120; i++){
            free(arr[i]);
        }
        t = clock() - t;
        time_taken[2] += ((double)t)/CLOCKS_PER_SEC;

        // Test 4
        t = clock();
        for(int i = 0; i < 120; i++){
            char *ptr = malloc(MEMSIZE-3*sizeof(struct Header));
            free(ptr);
        }
        t = clock() - t;
        time_taken[3] += ((double)t)/CLOCKS_PER_SEC;

        // Test 5
        t = clock();
        for(int i = 0; i < 50; i++){
            arr[i] = malloc(rand() % 50 + 1);
        }
        for(int i = 0; i < 50; i++){
            free(arr[i]);
        }
        t = clock() - t;
        time_taken[4] += ((double)t)/CLOCKS_PER_SEC;
    }
    for(int i = 0; i < 5; i++){
        printf("Test %d took %lf seconds on average\n", i+1, time_taken[i]/50.0);
    }
}