#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pwd.h>
#define DEFLEN 50
int main(int argc, char ** argv){
	while(1){
		char * input = NULL;
		size_t len = 0;
		printf("$_");
		getline(&input, &len, stdin);
		if(!strcmp(input, "exit\n"))
			return 0;
		char ** parts = malloc(1 * sizeof(char *));
		int parts_len = 0;
		// tokenize part
		char* token = strtok(input, " \n");
		while(token != NULL) {
			parts[parts_len++] = token;
			parts = realloc(parts, (parts_len + 1) * sizeof(char *));
			token = strtok(NULL, " \n");
		}
		parts[parts_len] = NULL;
		for(int i = 0; i < parts_len; i++){
			if(!strcmp(parts[i], "$SHELL") && (argc != 0))
				parts[i] = argv[0];
			if(!strcmp(parts[i], "$USER")){
				struct passwd *pw = getpwuid(getuid());
				parts[i] = pw->pw_name;
			}

			//printf("%s\n", parts[i]);
		}
		// ansyc part
		int pid = fork();
		if(pid == 0){
			char * path;
			asprintf(&path, "/bin/%s", parts[0]);
			execv(path, parts);
			free(path);
		}
		else{
			wait(NULL);
		}
		free(input);
		free(parts);
	}
}
