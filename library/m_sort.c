#include "m_sort.h"

int m_sort(m_sort_t type, unsigned char* src, int size) {
  if ((src == NULL) || (size <= 1)) {
    return -1;
  }
  
  if (type == BIG_TO_SMALL) {
    for (int i = 0; i < (size - 1); i++) {
      for (int j = 0; j < (size - 1 - i); j++) {
        if (src[j] < src[j + 1]) {
          unsigned char tmp = src[j];
          src[j] = src[j + 1];
          src[j + 1] = tmp;
        }
      }
    }
  } else if (type == SMALL_TO_BIG) {
    for (int i = 0; i < (size - 1); i++) {
      for (int j = 0; j < (size - 1 - i); j++) {
        if (src[j] > src[j + 1]) {
          unsigned char tmp = src[j];
          src[j] = src[j + 1];
          src[j + 1] = tmp;
        }
      }
    }
  } else {
    return -2;
  }

  return 0;
}
