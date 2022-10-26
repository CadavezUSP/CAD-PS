all:
	gcc paralelo.c funcoes_sequenciais.c -o paralelo -fopenmp -lm -Werror -g
run:
	./paralelo
clean:
	rm paralelo
zip:
	zip paralelo.zip paralelo.c funcoes_sequenciais.c Makefile