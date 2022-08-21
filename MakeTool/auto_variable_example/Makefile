.PHONY:clean
CC = gcc
RM = rm
OBJ = simple
OBJS = main.o robot1.o robot2.o

${OBJ}:${OBJS}
	${CC} -o $@  $^
main.o:main.c
	${CC} -c $^ -o $@
robot1.o:robot1.c
	${CC} -c $^ -o $@
robot2.o:robot2.c
	${CC} -c $^ -o $@
clean:
	${RM} ${OBJS} ${OBJ}
