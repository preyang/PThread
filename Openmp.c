#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

void insertionMatrix(int orderforMatrix, double** MatrixProblem, double* Outputofmatrix); /*This Method will generate the random values of Matrix*/
void printMatrix(int orderforMatrix, double** MatrixProblem, double* Outputofmatrix); /*This method will prints the matrix with the random numbers*/
void backSubstitution(int orderforMatrix, double** MatrixProblem, double* Outputofmatrix, double* coefficientVector, int numThreads); /*This method replaces the  coefficient with the solution vector*/
void OutputMatrix(double* coefficientVector, int orderforMatrix); /*This method gives the output of the given gaussian problem without pivoting*/

void main(){
	int orderforMatrix,q;
	int numThreads;
	struct timeval startTime, DestroyTime, SubstitutionTIme;
	double eliminationTime, substitutionTime, totalTime;

	 printf("Enter the order of the matrix here");
	    scanf("%d",&orderforMatrix);

	    printf("Enter the number of thread here");
	    scanf("%d",&numThreads);

	   /*thread_t elimination_threads[numThreads];*/

	    double **MatrixProblem = (double **)malloc(sizeof(double*)*orderforMatrix);
	    	for (q=0; q < orderforMatrix; q++)
	    		MatrixProblem[q] = (double*)calloc(orderforMatrix,sizeof(double*));

	    	/*Allocate Vector 'coefficientVector'*/
	    	double* Outputofmatrix = (double*) malloc(sizeof(double)*orderforMatrix);
	    	double* coefficientVector = (double*) malloc(sizeof(double)*orderforMatrix);

	    	insertionMatrix(orderforMatrix, MatrixProblem, Outputofmatrix);
	    	printf("\nPerforming gaussian elimination with the following matrix (MatrixProblem) and vector (Outputofmatrix):\n\n");
	    //	printMatrix(orderforMatrix, MatrixProblem, Outputofmatrix);

	    	gettimeofday(&startTime, NULL);
	    	int i, k,j;
	    	double m;

	    		/*Gaussian Elimination using OpenMP*/

	    		for (j=0; j < orderforMatrix-1; j++){

					#pragma omp parallel default(none) num_threads(numThreads) shared(orderforMatrix,MatrixProblem,Outputofmatrix,j) private(i,k,m)
	    			{

						#pragma omp for schedule(static)
						for (k=j+1; k<orderforMatrix; k++)
						{
							m = MatrixProblem[k][j]/MatrixProblem[j][j];
							for (i=j; i<orderforMatrix; i++)
							{
								MatrixProblem[k][i] = MatrixProblem[k][i] - (m * MatrixProblem[j][i]);
							}
							Outputofmatrix[k] = Outputofmatrix[k] - (m * Outputofmatrix[j]);
						}
					}

	    		}
				/*Doing the time calculation to */
	    		gettimeofday(&DestroyTime, NULL);

	    		printf("\n-------Gaussian Elimination -------\n");
	    		backSubstitution(orderforMatrix, MatrixProblem, Outputofmatrix, coefficientVector, numThreads);
	    		printf("\n--------Back Substitution ---------\n");
	    		gettimeofday(&SubstitutionTIme, NULL);

	    		eliminationTime = ((DestroyTime.tv_sec  - startTime.tv_sec) * 1000000u + DestroyTime.tv_usec - startTime.tv_usec) / 1.e6;
    			substitutionTime = ((SubstitutionTIme.tv_sec  - DestroyTime.tv_sec) * 1000000u + SubstitutionTIme.tv_usec - DestroyTime.tv_usec) / 1.e6;
    			totalTime = ((SubstitutionTIme.tv_sec  - startTime.tv_sec) * 1000000u + SubstitutionTIme.tv_usec - startTime.tv_usec) / 1.e6;

	    		OutputMatrix(coefficientVector, orderforMatrix);

	    		printf("Substitution execution time: %.3f seconds.\n", eliminationTime);
	    		printf("Substitution execution time: %.3f seconds.\n", substitutionTime);
	    		printf("Total execution: \n%.3f seconds elapsed with %d threads used.\n\n", totalTime, numThreads);

}

void insertionMatrix(int orderforMatrix, double** MatrixProblem, double* Outputofmatrix){
	int i, j;
	for (i=0; i<orderforMatrix; i++){
		for (j=0; j<orderforMatrix; j++){
			MatrixProblem[i][j] = (rand()*1000);
		}
		Outputofmatrix[i] = (rand()*1000);
	}
}

void printMatrix(int orderforMatrix, double** MatrixProblem, double* Outputofmatrix){

	int coefficientVector,y;
		for (coefficientVector=0; coefficientVector<orderforMatrix; coefficientVector++){
			printf("| ");
			for(y=0; y<orderforMatrix; y++)
				printf("%7.2f ", MatrixProblem[coefficientVector][y]);
			printf("| | %7.2f |\n", Outputofmatrix[coefficientVector]);
		}

}


void backSubstitution(int orderforMatrix, double** MatrixProblem, double* Outputofmatrix, double* coefficientVector, int numThreads){
	int i,j;
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
		printf("\nSolution Vector (coefficientVector):\n\n");
		for (i=0; i<orderforMatrix; i++){
			printf("|%11.6f|\n", coefficientVector[i]);
		}

}


