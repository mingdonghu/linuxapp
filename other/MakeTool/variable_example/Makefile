.PHONY:clean
CC = gcc
RM = rm
OBJ = simple
OBJS = main.o robot1.o robot2.o

${OBJ}:${OBJS}
	${CC} -o ${OBJ} ${OBJS}
main.o:main.c
	${CC} -c main.c -o main.o
robot1.o:robot1.c
	${CC} -c robot1.c -o robot1.o
robot2.o:robot2.c
	${CC} -c robot2.c -o robot2.o
clean:
	${RM} ${OBJS} ${OBJ}
