#include "m_sort.h"

#include <stdio.h>

int main(int argc, char const *argv[]) {
  unsigned char number[10] = {1,2,3,4,5,6,7,8,9,10};
  int ret;
  
  ret = m_sort(BIG_TO_SMALL, &number[0], sizeof(number) / sizeof(number[0]));
  if (ret < 0) {
    perror("m_sort() fail.");
    return -1;
  }
  printf("big to small.\n");
  for (int i = 0; i < (sizeof(number)/sizeof(number[0])); i++) {
    printf("%d\n", number[i]);
  }

  ret = m_sort(SMALL_TO_BIG, &number[0], sizeof(number) / sizeof(number[0]));
  if (ret < 0) {
    perror("m_sort() fail.");
    return -1;
  }
  printf("small to big.\n");
  for (int i = 0; i < (sizeof(number)/sizeof(number[0])); i++) {
    printf("%d\n", number[i]);
  }

  return 0;
}