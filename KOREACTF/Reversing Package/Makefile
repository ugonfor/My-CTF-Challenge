all: prob2

prob2: crc32.o main.o
	gcc -o prob2 crc32.o main.o -no-pie
	strip prob2
	python convert.py
	mv prob_replaced Reversing_Package

crc32.o : crc32.h crc32.c
	gcc -c -o crc32.o crc32.c

main.o : prob2.c crc32.h
	gcc -c -o main.o prob2.c

clean : 
	rm prob2 *.o
	rm prob_replaced
	rm Reversing_Package
	rm .gdb_history