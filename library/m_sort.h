#ifndef __M_SORT_H__
#define __M_SORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef enum m_sort_type_enum {
  BIG_TO_SMALL,
  SMALL_TO_BIG,
} m_sort_t;


int m_sort(m_sort_t type, unsigned char* src, int size);



#ifdef __cplusplus
}
#endif





#endif
