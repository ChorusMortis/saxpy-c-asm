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

extern void saxpyAsm(int, float, float*, float*, float*);
void saxpyC(int, float, float*, float*, float*);
int compareFloats(float, float, float);
void compareResults(int, float*, float*);
void initVector(int, float*, float*);

int main() {
	float sampleX[] = { 1.0, 2.0, 3.0, 4.0 };
	float sampleY[] = { 5.0, 6.0, 7.0, 8.0 };
	int sampleSize = sizeof(sampleX) / sizeof(sampleX[0]);

	int n = N;
	float A = 15.0;
	float* X = (float*)malloc(N * sizeof(float));
	float* Y = (float*)malloc(N * sizeof(float));
	float* Z_C = (float*)malloc(N * sizeof(float));
	float* Z_Asm = (float*)malloc(N * sizeof(float));
	initVector(sampleSize, sampleX, X);
	initVector(sampleSize, sampleY, Y);

	clock_t start, diff;
	long msec;
	
	start = clock();
	saxpyAsm(n, A, X, Y, Z_Asm);
	diff = clock() - start;
	msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Assembly :: Time taken: %d seconds %d milliseconds\n\n", msec / 1000, msec % 1000);

	start = clock();
	saxpyC(n, A, X, Y, Z_C);
	diff = clock() - start;
	msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("C :: Time taken: %d seconds %d milliseconds\n\n", msec / 1000, msec % 1000);

	compareResults(10, Z_Asm, Z_C);

	free(X);
	free(Y);
	free(Z_C);
	free(Z_Asm);

	return 0;
}

void initVector(int len, float* sampleV, float* V) {
	for (int i = 0; i < N; i++) {
		V[i] = sampleV[i % len];
	}
}

void saxpyC(int n, float A, float* X, float* Y, float* Z) {
	for (int i = 0; i < n; i++) {
		Z[i] = A * X[i] + Y[i];
	}
}

// floats are imprecise, should allow a little bit of inaccuracy/error
// https://stackoverflow.com/questions/5989191/compare-two-floats
int compareFloats(float a, float b, float epsilon) {
	return fabs(a - b) < epsilon;
}

void compareResults(int n, float* Z, float* answers) {
	float eps = (float)0.1;
	printf("Answers comparison (margin of error = %f)\n\n", eps);
	for (int i = 0; i < n; i++) {
		printf("[%d] expected %f, got %f => ", i, answers[i], Z[i]);
		if (compareFloats(Z[i], answers[i], eps) == 1) {
			printf("Correct\n");
		}
		else {
			printf("Incorrect\n");
		}
	}
}
