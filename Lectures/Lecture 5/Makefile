queue: cclass.o queue.o 
	gcc cclass.o queue.o -o queue

cclass.o: cclass.c queue.h
	gcc -Wall -Werror -ggdb cclass.c -c

queue.o: queue.c queue.h
	gcc -Wall -Werror -ggdb queue.c -c

clean:
	rm -f queue.o cclass.o queue
