#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>

#define NR_make_vcpu 333

void main(int argc, char **argv)
{
	int pid = atoi(argv[1]);
	syscall(NR_make_vcpu, pid); 	
}
