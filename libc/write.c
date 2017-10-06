#include <sbulib.h>

int syscall0(long long int n) {
	long long int res = -1;
	__asm__(
		"movq %1, %%rax\n\t"
		"syscall\n\t"
		"mov %%rax, %0\n\t"
		: "=r" (res)
		: "r" (n));
		return res;
}

int syscall1(long long int n, long long int arg1) {
	long long int res = -1;
	__asm__(
		"movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"syscall\n\t"
		"mov %%rax, %0\n\t"
		: "=r" (res)
		: "r" (n),"r" (arg1)
		: "%rax","%rdi");

		return res;
}

int syscall2(long long int n, long long int arg1, long long arg2) {
	long long int res = -1;
        __asm__(
                "movq %1, %%rax\n\t"
                "movq %2, %%rdi\n\t"
		"movq %3, %%rsi\n\t"
                "syscall\n\t"
                "mov %%rax, %0\n\t"
                : "=r" (res)
                : "r" (n),"r" (arg1),"r" (arg2)
		: "%rax","%rdi","%rsi");

                return res;

}

int syscall3(long long int n, long long int arg1, long long int arg2, long long int arg3) {
	long long int res = -1;
	__asm__(
		"movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"movq %3, %%rsi\n\n"
		"movq %4, %%rdx\n\t"
		"syscall\n\t"
		
		"mov %%rax, %0\n\t"
		: "=r" (res)
		: "r" (n),"r" (arg1),"r" (arg2),"r" (arg3)
		: "%rax", "%rdi", "%rsi","%rdx");

		return res;
}

int syscall4(long long int n, long long int arg1, long long int arg2, long long int arg3, long long int arg4) {
        long long int res = -1;
        __asm__(
                "movq %1, %%rax\n\t"
                "movq %2, %%rdi\n\t"
                "movq %3, %%rsi\n\n"
                "movq %4, %%rdx\n\t"
		"movq %5, %%r10\n\t"
                "syscall\n\t"

                "mov %%rax, %0\n\t"
                : "=r" (res)
                : "r" (n),"r" (arg1),"r" (arg2),"r" (arg3),"r" (arg4)
                : "%rax", "%rdi", "%rsi","%rdx","%r10");

                return res;
}


int write(int fd, const char *buf, int count) {
	return syscall3(1, fd, (long) buf, count);
}

int exit(int status) {
	return syscall1(60,status);
}

int getpid() {
	return syscall0(39);
}

int read(int fd, const char *buf, int count) {
	return syscall3(0,fd,(long) buf,count);
}

int getcwd(const char *buf, int count) {
	return syscall2(79,(long) buf,count);
}

int execve(char *str, char **buf, char **envp) {
        return syscall3(59,(long long)str,(long long) buf,(long long) envp);
}

int fork() {
	return syscall0(57);
}

int getdents(unsigned int a, struct temp_dirent *p, unsigned int b) {
	return syscall3(78,(long long) a,(long long) p,(long long) b);
}

int open(char *path, int a) {
	return syscall2(2,(long long) path,(long long) a);
}

int close(int pid) {
	return syscall1(3,(long long) pid);
}

int pipe(int *a) {
	return syscall1(22,(long long) a);
}

int dup2(int a, int b) {
	return syscall2(33,(long long) a,(long long) b);
}

int wait(int *a) {
	return syscall4(61,0,(long long) a,0,0);
}

int chdir(char *a) {
        return syscall1(80,(long long)a);
}

/*
int main() {
	int val=0;
	//syscall1(60,0);			//exit
	//val = syscall0(39); 			//pid
	//char *str = "Hello\n";
	//char buff[1024];
	//val = read(0,buff,6);			//read
	//printf("%d",val);
	//val = write(1, buff,6); 		//write
	//val = getcwd(buff,sizeof(buff));	//getcwd
	//printf("\n%s@sbush>\n ", buff);
	//char *args[]     = { "ls", NULL };
   	//char *env_args[] = { "PATH=/bin/ls", "USER=shashwat", NULL };
    	//val = execve(args[0], args, env_args);
	printf("\n%d\n",val);
	return 0;
}*/
