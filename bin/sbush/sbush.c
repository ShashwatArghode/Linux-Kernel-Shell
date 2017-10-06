#include <sbulib.h>
#define O_RDONLY 0x0000
#define true 1
#define false 0

void prepareCommand(char* command);

int totalNumberOfCommands = 0;

int shellStatus = true;

int background = false;

char **envp_global;

//char *PS1;

struct Command_struct {
	char argsArray[10][50];
	int background;
};

void write1(char *string);

void commandTokeniser(char *line, char **argv)
{
	while (*line != '\0')
	{
		while (*line == ' ' || *line == '&' || *line == '\t' || *line == '\n')
		{

			*line++ = '\0';
		}
		*argv++ = line;

		while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')

			line++;

	}

	argv = '\0';
}

void tokenize(char *line, char **argv, char * argvexec, char token) {
	if(0 == argvexec){
		while (*line != '\0')
		{
			while (*line == token)
			{
				*line++ = '\0';
			}
			*argv++ = line;
			while (*line != '\0'  && *line != '\t' && *line != '\n' && *line != token)
				line++;
		}
	}else {
		char *temp;
		temp = argvexec;
		*argv = temp;
		//		int i=0;
		while (*line != '\0')
		{
			while (*line == token)
			{
				*line = '\0';
				*temp = '\0';
				line++;
				argvexec += 100;
				temp = argvexec;
				argv++;
				*argv = temp;
			}
			//*argv++ = line;
			while (*line != '\0'  && *line != '\t' && *line != '\n' && *line != token){
				*temp = *line;
				line++;
				temp++;
			}
		}

	}
	argv = '\0';
}

int strLength(char* str) {
	int i;
	for (i = 0; str[i] != '\0' ; ++i);
	return i;
}
/*
int setPS1(char *str) {
	int cwd[1024];
	getcwd(cwd, sizeof(cwd);
	strCat(PS1, cwd);
	strCat(PS1, str);
	return 0;
}*/

void strCat(char* a, char* b) {

	while(*a)
		a++;
	while(*b){
		*a = *b;
		b++;
		a++;
	}
	*a= '\0';
}

int stringCompare(char *a, char *b) {
	while (*b && *a == *b) {
		a++;
		b++;
	}
	return *a - *b;
}

void strCpy(char *a , char *b){

	while (*b){
		*a = *b;
		a++;
		b++;
	}
	*a = '\0';	
}

/*
   void echo(char* variable) {
   if (variable[0] == '$') {
   char* var_no_dollar;
   var_no_dollar = &(variable[1]);
//printf("%s\n", getenv(var_no_dollar));
}
else {
//printf("%s\n", variable);

}
}


void exportPath(char* args[]) {
char* var;
char* val;
char *argv[64];
tokenize(args[1], argv , '=');

var = argv[0];
val = argv[1];

val = &(val[1]);
val[strLength(val) - 1] = '\0';
//setenv(var, val, 1);
}*/


int cd(char* dir) 
{
	return chdir(dir);
}

void executeCommand(char *argv[], int isBackground) {
	int status;

	char str[99] = "/bin/";
	//write1("               argv[0]              ");
	//write1(argv[0]);
	//write1("               argv[1]              ");
	//write1(argv[1]);
	//strCat(str, "/bin/");
	strCat(str, argv[0]);
	//strCat(str, "\0");
	//argv[0]= " ";
	strCpy(argv[0], str);
	//write1("     Here is replaced one argv[0]:    ");
	//write1(argv[0]);
	//write1("     Here is replaced one argv[1]:     ");
	//write1(argv[1]);


	int pid = fork();
	if (pid < 0) {
		//printf("Error: Unable to fork child process");
		exit(1);
	}
	if (pid == 0) {
		int ret = execve(argv[0], &argv[0], envp_global);
		//write1("fork in pid");
		if (ret < 0) {
			//printf("Error: command executation failed");
			exit(1);
		}
	}
	else
	{
		if (!isBackground)
		{
			//printf("waiting .... \n");
			while (wait(&status) != pid);
		}
	}
	//write1("end of fork");
}

void executePipe(char **argT){
	int i;

	for( i=1; i<totalNumberOfCommands; i++)
	{
		char *args[64] = {'\0'};
		char argsexec[6400] = {"\0"};
		tokenize(argT[i-1], args,(char *)argsexec, ' ');
		//commandTokeniser(argT[i-1], args);
		if (stringCompare(args[0], "cd") == 0) {
			cd(args[1]);
		}
		else{
			//printf("\nHandling command number %d\n", i);
			int pd[2];
			pipe(pd);
			write1("Here after pipe");
			if (!fork()) {
				dup2(pd[1], 1);
				write1(args[0]);	
				execve(args[0], &args[0], envp_global);
				//perror("exec");
				//abort();
			}
			dup2(pd[0], 0);
			close(pd[1]);
		}
	}
	char *args[64] = {'\0'};
	char argsexec1[6400] = {"\0"};
	tokenize(argT[i-1], args,(char *)argsexec1, ' ');
	//commandTokeniser(argT[i-1], args);

	if (stringCompare(args[0], "cd") == 0) {
		cd(args[1]);
	}
	else{
		write1("Here after pipe : in else");
		execve(args[0], &args[0], envp_global);

	}
}

int main(int argc, char *argv[], char *envp[]) {
	char *str = "Hello\n";
	write(1, str,6);
	envp_global = envp;
	//int status = true;
	if(argv[1] != '\0') {
		int fd;
		char buffer[30];
		int i=0;
		if((fd = open(argv[1], O_RDONLY))!=-1) {
			while(read(fd, &buffer[i], 1)==1) {
				if(buffer[i]!='\n') {
					i++;
				}
				else if(i!=0){
					if(buffer[i]=='#') {
						//printf("Found # \n");
					}
					buffer[i]='\0';
					//printf("Printing buffer :%s\n",buffer);
					i=0;
					prepareCommand(buffer);
				}
			}
		}
		else {
			//printf("Unable to execute command as there was problem in opening the requested file");
		}
		exit(0);
	}
	do {
		char *argT[64] = {"\0"};
		char argsexec[6400] = {"\0"};
		char input[1024];
		char cwd[1024];
		int i = getcwd(cwd, sizeof(cwd));
		if(i != -1){
			write1(cwd); 
			write1("@sbush> ");
		}
		else {
			write1("sbush> ");
			//scanf(" %[^\n]%*c",input);
		}
		int input_counter = 0;
		char c[1];
		do{
			read(0,c,1);
			if (c[0]=='&')
			{
				background = true;
				input[input_counter-1] = '\0';
			}
			else
			{
				input[input_counter] = c[0];
			}
			input_counter++;
		}while(input[input_counter-1]!=10);
		input[input_counter-1] = '\0';
		//write1(input);
		//done getting input
		tokenize(input, argT,(char *)argsexec, '|');
		int counter = 0;
		while (argT[counter])
		{
			counter++;
			//write1("arg++ : ");
			//write1(argT[counter]);
		}

		totalNumberOfCommands = counter;
		//counter -= 1;
		//write1("totalNumberOfCommands:");
		//write1(totalNumberOfCommands);
		if (totalNumberOfCommands < 2){
			prepareCommand(argT[0]);
		}

		else{
			write1("Here I am");
			int status = 0;
			int pid_parent = fork();
			if(pid_parent == 0)
			{
				write1("Here I am: in fork");
				executePipe(argT);
			}
			else{
				write1("Here I am: in else");
				while (wait(&status) != pid_parent);
			}
		}
		//write1("re-running");
	} while (shellStatus);
	return 0;
}

void prepareCommand(char* command) {
	char *args[64] = {"\0"};
	char argsexec[6400] = {"\0"};
	//write1("In command ");
	//write1(command);
	//write1("               ");
	tokenize(command, args,(char *)argsexec, ' ');
	//tokenize(command, args);
	//write1(args[0]);
	//write1(args[1]);	
	if (stringCompare(args[0], "cd") == 0) {
		cd(args[1]);
	}
	else if (stringCompare(args[0], "export") == 0) {
		//exportPath(args);
	}

	else if (stringCompare(args[0], "exit") == 0) {
		shellStatus = false;
	}

	else if (stringCompare(args[0], "echo") == 0) {
		//echo(args[1]);
	}
	else 
	{
		//printf("value of background : %d\n", background );
		//write1(args[0]);
		//write1("     Here       ");
		executeCommand(args, background);
	}
}

void write1(char *string)
{
	if(string){
		write(1, string, (strLength(string) +1));
	}
} 
