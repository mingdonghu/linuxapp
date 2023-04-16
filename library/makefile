.PHONY:clean

CC = gcc
OBJ = test

${OBJ}:m_sort.o
	@echo "create static link library."
	@ar crs libm_sort.a $^

	#@echo "create dynamic link library."
	#@${CC} -shared $^ -o libm_sort.so 

	@echo "generate binary."
	@${CC} test.c -L . -lm_sort -I . -o $@

m_sort.o:m_sort.c
	@${CC} -fpic -c $^ -o $@
clean:
	@rm *.o *.a *.so ${OBJ}


