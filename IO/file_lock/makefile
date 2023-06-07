.PHONY:clean

CC = gcc
#CFLAGS = -Wall -g
RM = rm -rf


all:
	@echo "make all"
	@${CC} file_read_lock.c file_lock_set.c -o read_lock
	@${CC} file_write_lock.c file_lock_set.c -o write_lock
clean:
	@echo "make clean"
	@${RM} read_lock write_lock