struct temp_dirent{
        char name[1024];
        unsigned short len;
};
int main(int argc, char *argv[], char *envp[]);
int write(int fd, const char *buf, int count);
int syscall1(long long int n, long long int arg1);
int execve(char *str, char **buf, char **envp);
int getpid();
int exit(int status);
int getcwd(const char *buf, int count);
int read(int fd, const char *buf, int count);
int fork();
int getdents(unsigned int a,struct temp_dirent *p , unsigned int b);
int open(char *path, int a);
int close(int pid);
int pipe(int *a);
int dup2(int a, int b);
int wait(int *a);
int strLength(char* str);
int chdir(char *a);
