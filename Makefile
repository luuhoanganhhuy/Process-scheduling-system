allocate: allocate.c
	gcc -Wall -o allocate allocate.c -lm

.PHONY: clean

clean: 
		rm -f *.o
		rm -f allocate.exe
