#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>

// #define N (1073741824) // 2^30
// #define N (536870912) // 2^29
// #define N (268435456) // 2^28
// #define N (134217728) // 2^27
#define N (67108864) // 2^26
// #define N (16777216) // 2^24
// #define N (1048576) // 2^20
// #define N (16)

extern void saxpyAsm(int, float, float*, float*, float*);
void saxpyC(int, float, float*, float*, float*);
float floatRand(float, float);
void initVector(float, float, float*);
int compareFloats(float, float, float);
void compareResults(float*, float*);

int main() {
	float minValue = -100.0;
	float maxValue = 100.0;

	int n = N;
	float A = floatRand(minValue, maxValue);
	float* X = (float*)malloc(N * sizeof(float));
	float* Y = (float*)malloc(N * sizeof(float));
	float* Z_C = (float*)malloc(N * sizeof(float));
	float* Z_Asm = (float*)malloc(N * sizeof(float));

	// abort if not all vectors were properly initialized
	if (!X || !Y || !Z_C || !Z_Asm) {
		fprintf(stderr, "Memory allocation for vectors failed. Use a lower value for N or free up memory.\n");
		free(X);
		free(Y);
		free(Z_C);
		free(Z_Asm);
		return EXIT_FAILURE;
	}

	initVector(minValue, maxValue, X);
	initVector(minValue, maxValue, Y);
	
	clock_t Asm_start = clock();
	saxpyAsm(n, A, X, Y, Z_Asm);
	clock_t Asm_end = clock();
	clock_t Asm_diff = Asm_end - Asm_start;
	long Asm_msec = Asm_diff * 1000 / CLOCKS_PER_SEC;

	// separate Assembly results from C results
	printf("\n\n");

	clock_t C_start = clock();
	saxpyC(n, A, X, Y, Z_C);
	clock_t C_end = clock();
	clock_t C_diff = C_end - C_start;
	long C_msec = C_diff * 1000 / CLOCKS_PER_SEC;

	printf("\n");
	printf("Assembly :: Time taken: %d seconds %d milliseconds\n", Asm_msec / 1000, Asm_msec % 1000);
	printf("C        :: Time taken: %d seconds %d milliseconds\n\n", C_msec / 1000, C_msec % 1000);

	compareResults(Z_Asm, Z_C);

	free(X);
	free(Y);
	free(Z_C);
	free(Z_Asm);

	return 0;
}

void saxpyC(int n, float A, float* X, float* Y, float* Z) {
	for (int i = 0; i < n; i++) {
		Z[i] = A * X[i] + Y[i];
	}
	// assume there are at least 10 elements to print out
	printf("SAXPY results in C (first 10)\n\n");
	for (int i = 0; i < 10; i++) {
		printf("Z_C[%d] = %f\n", i, Z[i]);
	}
}

// creates random float between two values, both inclusive
// https://stackoverflow.com/a/44105089
float floatRand(float min, float max) {
	float scale = rand() / (float)RAND_MAX;
	return min + scale * (max - min);
}

void initVector(float min, float max, float* V) {
	for (int i = 0; i < N; i++) {
		V[i] = floatRand(min, max);
	}
}

// floats are imprecise, should allow a little bit of inaccuracy/error
// https://stackoverflow.com/a/5989243
int compareFloats(float a, float b, float epsilon) {
	return fabs(a - b) < epsilon;
}

void compareResults(float* Z, float* answers) {
	float eps = (float)0.1;
	int allCorrect = 1;
	printf("Answers comparison (margin of error = %f)\n\n", eps);
	for (int i = 0; i < N && allCorrect == 1; i++) {
		if (compareFloats(Z[i], answers[i], eps) != 1) {
			allCorrect = 0;
		}
	}
	if (allCorrect == 1) {
		printf("The x86-64 kernel output is correct.\n");
	}
	else {
		printf("The x86-64 kernel output is incorrect.\n");
	}
}
