all:test_mnist mnist2pgm

test_mnist: mnist.c test_mnist.c
	gcc -g -Wall -pedantic -std=c11 -o test_mnist mnist.c test_mnist.c -lm -lcunit -lgmp

mnist2pgm:mnist.c mnist2pgm.c 
	gcc -Wall -pedantic -std=c11 -o mnist2pgm mnist.c mnist2pgm.c -g -lm

clean:
	rm -f mnist2pgm test_mnist