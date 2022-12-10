${flags} = -Wall -Wextra -Werror

all:
	gcc pvc-seq.c -o pvc-seq ${flags}
	mpicc pvc-par.c -o pvc-par ${flags}
run_seq:
	./pvc-seq
run_par:
	mpirun -np 4 ./par
zip:
	zip -r trabalho.zip Makefile seq.c par.c pcam-pvc.pdf resultados-pvc-mpi.pdf
clean:
	rm -f pvc-seq pvc-par
run:
	./pvc-seq ${nodes}
	mpirun -np ${clusters} ./pvc-par ${nodes}