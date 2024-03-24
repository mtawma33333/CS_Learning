#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
	int months;
	double contribution;
	double rate_of_return;
};
typedef struct _retire_info retire_info;


void compute_balance(int *startAge, double *initial, retire_info info) {
    for (int i = 0; i < info.months; i++) {
        // Your common operations here
	printf("Age %3d month %2d you have $%.2lf\n", *startAge/12, *startAge%12, *initial);
        *initial *= 1 + info.rate_of_return;
        *initial += info.contribution;  
        (*startAge)++;
    }
}
void retirement (int startAge, double initial, retire_info working, retire_info retired) {
	// compute balance
	compute_balance(&startAge, &initial, working);
	compute_balance(&startAge, &initial, retired);
}

int main (void) {
	retire_info working = {.months=489, .contribution=1000.0, .rate_of_return=0.045/12};

	retire_info retired = {.months=384, .contribution=-4000.0, .rate_of_return=0.01/12};

	retirement(327, 21345.0, working, retired);
	return 0;
}
