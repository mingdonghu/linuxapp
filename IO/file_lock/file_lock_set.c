#include "file_lock_set.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int file_lock_set(int fd, int type) {
  struct flock lock;
  
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;
  lock.l_type = type;

  /** 1.读取锁，判断文件是否上锁 */
  if (fcntl(fd, F_GETLK, &lock) < 0) {
    // perror("fcntl error: F_GETLK");
    // return -1;
  }
  
  /** 2.判断文件不能上锁的原因*/
  if (lock.l_type != F_UNLCK) {
    if (lock.l_type == F_RDLCK) {
      printf("Read lock already set by %d\n", lock.l_pid);
    } else if (lock.l_type == F_WRLCK) {
      printf("Write lock already set by %d\n", lock.l_pid);
    }
  }

  /** 2.l_type 可能已被F_GETLK修改过，重新赋值*/
  lock.l_type = type;

  /** 3.设置文件锁(根据不同的type值进行阻塞式上锁或解锁) */
  if (fcntl(fd, F_SETLKW, &lock) < 0) {
    perror("fcntl error: F_SETLKW");
    return -2;
  }

  /** 4.打印上锁或解锁信息 */
  switch (lock.l_type) { 
    case F_RDLCK: {
      printf("Read lock set by %d\n", getpid());
      break;
    }
    case F_WRLCK: {
      printf("Write lock set by %d\n", getpid());
      break;
    }
    case F_UNLCK: {
      printf("Release lock by %d\n", getpid());
      return 1;
      break;
    }
    default:
      break;
  }
  
  return 0;
}