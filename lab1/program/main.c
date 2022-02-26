#include "/usr/include/lib.h"
#include "/usr/include/minix/type.h"
#include <stdio.h>
#include <stdlib.h>

#define PROC_NUMBER_LEN 10


int getprocnr(int nr_proc) {
  message msg;
  msg.m2_l2 = nr_proc;
  return _syscall(MM, GETPROCNR, &msg);
}


int main(int argc, char* argv[]) {
  if (argc <= 0 || argc > 2) {
    printf("Run program using one argument\n");
  }
  else {
    int end_proc_number = atoi(argv[1]);
    int start_proc_number = end_proc_number - 10;

    int proc_number_iter;
    int proc_number;
    for (proc_number_iter = start_proc_number; proc_number_iter < end_proc_number; proc_number_iter++) {
      proc_number = getprocnr(proc_number_iter);
      if (proc_number >= 0) {
        printf("Proc pos: %d / Proc number: %d\n", proc_number_iter, proc_number);
      }
      else {
        printf("Error! Proc pos: %d / Errno: %d\n", proc_number_iter, errno);
      }
    }
  }

  return 0;
}
