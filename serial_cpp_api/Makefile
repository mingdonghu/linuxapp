.PHONY:clean

target:
	@g++ test.cpp serial_interface_linux.cpp serial_linux_api.c -lpthread -std=c++11
	@echo "build finish. please ./a.out"

clean:
	@rm a.out
	@echo "clear build"