
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>

void insertionMatrix(int orderforMatrix, double** MatrixProblem, double* Outputofmatrix); /*This Method will generate the random values of Matrix*/
void PrintMatrix(int orderforMatrix, double** MatrixProblem, double* Outputofmatrix); /*This method will prints the matrix with the random numbers*/
void backSubstitution(int orderforMatrix, double** MatrixProblem, double* Outputofmatrix, double* coefficientVector, int numberOfThread); /*This method replaces the  coefficient with the solution vector*/
void OutputMatrix(double* coefficientVector, int orderforMatrix); /*This method gives the output of the given gaussian problem without pivoting*/

void *exclude(void *thread_argument); /*This function is used  for thread parallelism and is executed for Every thread*/

/*Structure defined for thread where all the thread data is stored*/
struct Thread_Info {
	double** MatrixProblem;
	double* Outputofmatrix;
	int orderforMatrix;
	int j;
	int numberOfThread;
} thread_info;

void main()
{
int orderforMatrix,i,j,threadIndex;
int numberOfThread,q;
struct timeval startTime, DestroyTime, SubstitutionTIme;
double destroyTime, substituteTime, Timeelapsed;

printf("Gaussian Elimination Without Pivoting\n");

    printf("Enter Order of Matrix Here:");
    scanf("%d",&orderforMatrix);

    printf("Enter Number of Thread Here:");
    scanf("%d",&numberOfThread);

    double **MatrixProblem = (double **)malloc(sizeof(double*)*orderforMatrix);
	for ( q=0; q < orderforMatrix; q++)
		MatrixProblem[q] = (double*)malloc(sizeof(double*)*orderforMatrix);

	/*Allocate Vector 'coefficientVector'*/
	double* Outputofmatrix = (double*) malloc(sizeof(double)*orderforMatrix);
	double* coefficientVector = (double*) malloc(sizeof(double)*orderforMatrix);
/*pthread intialization */
	pthread_t elimination_threads[numberOfThread];

insertionMatrix(orderforMatrix, MatrixProblem, Outputofmatrix);
/*To Print the matrix
int x,y;
		for ( x=0; x<orderforMatrix; x++){
			printf("| ");
			for( y=0; y<orderforMatrix; y++)
				printf("%7.2f ", MatrixProblem[x][y]);
			printf("| | %7.2f |\n", Outputofmatrix[x]);
		}
*/

    thread_info.MatrixProblem = MatrixProblem;
	thread_info.Outputofmatrix = Outputofmatrix;
	thread_info.orderforMatrix = orderforMatrix;
	thread_info.numberOfThread = numberOfThread;
	/*give index to thread*/
    int *index = malloc (sizeof(int)*numberOfThread);
	for( i = 0; i < numberOfThread; i++)
    {
    	index[i] = i;
    }
/*set time to check the executation time*/
	gettimeofday(&startTime, NULL);
for ( j=0; j < orderforMatrix-1; j++){

		thread_info.j = j;

		for ( threadIndex = 0; threadIndex < numberOfThread; threadIndex++){
			pthread_create(&elimination_threads[threadIndex], NULL, exclude, (void*)&index[threadIndex]);
		}

		for ( threadIndex = 0; threadIndex < numberOfThread; threadIndex++){
			pthread_join(elimination_threads[threadIndex], NULL);
		}
	}
gettimeofday(&DestroyTime, NULL);

backSubstitution(orderforMatrix, MatrixProblem, Outputofmatrix, coefficientVector, numberOfThread);
gettimeofday(&SubstitutionTIme, NULL);
destroyTime = ((DestroyTime.tv_sec  - startTime.tv_sec) * 1000000u + DestroyTime.tv_usec - startTime.tv_usec) / 1.e6;
substituteTime = ((SubstitutionTIme.tv_sec  - DestroyTime.tv_sec) * 1000000u + SubstitutionTIme.tv_usec - DestroyTime.tv_usec) / 1.e6;
Timeelapsed = ((SubstitutionTIme.tv_sec  - startTime.tv_sec) * 1000000u + SubstitutionTIme.tv_usec - startTime.tv_usec) / 1.e6;

OutputMatrix(coefficientVector, orderforMatrix);

printf("Destroy execution time: %.3f seconds.\n", destroyTime);
printf("Substitution execution time: %.3f seconds.\n", substituteTime);
printf("Total execution: \n%.3f seconds elapsed with %d threads used.\n\n", Timeelapsed, numberOfThread);

}
/*This method fill the matrix with random values.*/
void insertionMatrix(int orderforMatrix, double** MatrixProblem, double* Outputofmatrix){
	int i, j;
	for (i=0; i<orderforMatrix; i++){
		for (j=0; j<orderforMatrix; j++){
			MatrixProblem[i][j] = (rand()% 10000); /*Generates the random numbers between 0 to 1000*/
		}
		Outputofmatrix[i] = (rand()%10000);/* Generates the random numbers between 0 to 1000*/
	}
}

void backSubstitution(int orderforMatrix, double** MatrixProblem, double* Outputofmatrix, double* coefficientVector, int numberOfThread){
	int i,j;
	printf("Into the back Substitution");
	for (i=orderforMatrix-1; i >= 0; i--){
		coefficientVector[i] = Outputofmatrix[i];
		for (j=i+1; j<orderforMatrix; j++){
			coefficientVector[i] -= MatrixProblem[i][j]*coefficientVector[j];
		}
		coefficientVector[i] = coefficientVector[i] / MatrixProblem[i][i];
	}
}
void OutputMatrix(double* coefficientVector, int orderforMatrix){
int i;

		for ( i=0; i<orderforMatrix; i++){
			printf("%7.2f\n", coefficientVector[i]);
		}
}

void *exclude(void *thread_argument){
	int *threadIndex = (int*)thread_argument;
	double** MatrixProblem = thread_info.MatrixProblem;
	double* Outputofmatrix = thread_info.Outputofmatrix;
	int orderforMatrix = thread_info.orderforMatrix;
	int j = thread_info.j;
	int numberOfThread = thread_info.numberOfThread;
	int i, k;
	double m;

//	printf("Thread Index is %d\n",*threadIndex);
	for (k=j+1+ *threadIndex; k<orderforMatrix; k += numberOfThread){
		m = MatrixProblem[k][j]/MatrixProblem[j][j];
		for (i=j; i<orderforMatrix; i++){

			MatrixProblem[k][i] = MatrixProblem[k][i] - (m * MatrixProblem[j][i]);
		}
		Outputofmatrix[k] = Outputofmatrix[k] - (m * Outputofmatrix[j]);
	}
}


