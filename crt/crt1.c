#include <sbulib.h>

long long int argc,argv,env;

void _start(void) {
  	//call main() and exit() here
	__asm__ (
		"mov %%rsp, %0"
		:"=r" (argc)
		:
		:"%rsp");
	argc += 0x8;
	argv = argc + 0x8;
	argc = *(long long int *)argc;
	env = argv + (argc * 0x8)+ 0x8;
	main(argc,(char**)argv,(char**)env);
	syscall1(60,0);
}

