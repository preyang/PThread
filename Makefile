all: openmp pthread

openmp:
	gcc Openmp.c -std=gnu99 -fopenmp -lm -o openmp

pthread:
	gcc GaussEliminationWithPthread.c -std=gnu99 -lpthread -lm -o pthread

clean:
	rm openmp pthread
