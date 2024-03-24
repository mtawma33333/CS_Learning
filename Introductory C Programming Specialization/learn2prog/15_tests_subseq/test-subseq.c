#include<stdlib.h>
#include<stdio.h>

size_t maxSeq(int * array, size_t n);

int main(void) {
    // case 0
    if (maxSeq(NULL, 0) != 0 ) {
        return EXIT_FAILURE;
    }
    // case 1
    size_t x1 = 10;
    int arr1[] = {1, 2, 1, 3, 5, 7, 2, 4, 6, 9};
    if (maxSeq(arr1, x1) != 4) {
        return EXIT_FAILURE;
    }
    // case 2
    size_t x2 = 1;
    int arr2[] = {1};
    if (maxSeq(arr2, x2) != 1) {
        return EXIT_FAILURE;
    }
    // case 3
    size_t x3 = 11;
    int arr3[] = {2,3,4,5,4,3,4,5,6,7,0};
    if (maxSeq(arr3, x3) != 5) {
        return EXIT_FAILURE;
    }
    // case 4
    size_t x4 = 4;
    int arr4[] = {-4, -2, 0, -1};
    if (maxSeq(arr4, x4) != 3) {
	    return EXIT_FAILURE;
    }
    // case 5
    size_t x5 = 3;
    int arr5[] = {5,4,3};
    if (maxSeq(arr5, x5) != 1) {
	    return EXIT_FAILURE;
    }
    // case 6
    size_t x6 = 6;
    int arr6[] = {1, 2, -3, 4, 5, 6};
    if (maxSeq(arr6, x6) != 4) {
	    return EXIT_FAILURE;
    }
    size_t x7 = 7;
    int arr7[] = {1, 2, 2, 3, 4, 5, 6};
    if (maxSeq(arr7, x7) != 5) {
	    return EXIT_FAILURE;
    }
}
