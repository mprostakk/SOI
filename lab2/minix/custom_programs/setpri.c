#include "/usr/include/lib.h"
#include "/usr/include/minix/type.h"
#include <stdio.h>
#include <stdlib.h>


int setproc(int nr_proc, int group) {
  message msg;
  msg.m1_i1 = nr_proc;
  msg.m2_l2 = group;
  return _syscall(MM, SETPRI, &msg);
}


int main(int argc, char* argv[]) {
  if (argc <= 2 || argc > 3) {
    printf("Run program using two arguments: proc_id, group (0 or 1) \n");
  }
  else {
    int proc_number = atoi(argv[1]);
    int group = atoi(argv[2]);
    
    int b = setproc(proc_number, group);
    printf("setpri.c - setproc: %d\n", b);
  }
  return 0;
}
