#include<stdlib.h>

// function prototype
unsigned power (unsigned x, unsigned y);

void run_check(unsigned x, unsigned y, unsigned expected_ans) {
    unsigned tmp = power(x, y);
    if (tmp != expected_ans) {
        exit(EXIT_FAILURE);
    }
    return ;
}

int main(void) {
    run_check(0, 0, 1);
    run_check(1, 0, 1);
    run_check(0, 2, 0);
    run_check(2, 11, 2048);
    run_check(2, 8, 256);
    run_check(10, 5, 100000);

    return EXIT_SUCCESS;
}
