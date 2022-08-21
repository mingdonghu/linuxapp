#include <stdio.h>
extern int GetRobot1Number(void);
extern int GetRobot2Number(void);

int main(int argc, char** argv) {
  	printf("robot1:%d\n", GetRobot1Number());
	printf("robot2:%d\n", GetRobot2Number());
	return 0;
}
