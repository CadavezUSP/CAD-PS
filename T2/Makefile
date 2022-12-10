flags = -Wall -Wextra -Werror

all:
	gcc pvc-seq.c -o pvc-seq ${flags}
	mpicc pvc-par.c -o pvc-par ${flags}
run_seq:
	./pvc-seq 11
run_par:
	mpirun -np 8 ./pvc-par 11
zip:
	zip -r trabalho.zip Makefile seq.c par.c pcam-pvc.pdf resultados-pvc-mpi.pdf
clean:
	rm -f pvc-seq pvc-par
run:
	./pvc-seq ${nodes}
	mpirun -np ${processes} ./pvc-par ${nodes}