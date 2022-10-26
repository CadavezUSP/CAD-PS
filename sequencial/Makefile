all:
	gcc sequencial.c funcoes_sequenciais.c -o sequencial -lm -fopenmp -Werror -g
opt:
	gcc sequencial.c funcoes_sequenciais.c -o sequencial -O3 -lm -fopenmp
run:
	./sequencial
clean:
	rm sequencial
zip:
	zip sequencial.zip sequencial.c funcoes_sequenciais.c Makefile