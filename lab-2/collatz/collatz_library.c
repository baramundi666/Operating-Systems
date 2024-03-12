#include <stdio.h>

int collatz_conjecture(int input) {
    if(input%2 == 0) return input/2;
    return 3*input+1;
}

int test_collatz_convergence(int input, int max_iter) {
    int current = input;
    int iter = 0;
    while(iter<max_iter && current!=1) {
        if(current == 1) break;
        current = collatz_conjecture(current);
        iter++;
    }
    return iter<max_iter ? iter : -1;
}
