.PHONY:clean
simple:main.o robot1.o robot2.o
	gcc -o simple main.o robot1.o robot2.o
main.o:main.c
	gcc -c main.c -o main.o
robot1.o:robot1.c
	gcc -c robot1.c -o robot1.o
robot2.o:robot2.c
	gcc -c robot2.c -o robot2.o
clean:
	rm *.o simple
