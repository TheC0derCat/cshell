#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void loop(void);
void tokenize(char *input, char **parts, int parts_len);
void async_part(char *input, char **parts);

int main(void)
{
    for (;;) {
        loop();
    }
}

void loop(void)
{
    char *input = NULL;
    size_t len = 0;
    printf("$_");
    getline(&input, &len, stdin);
    char **parts = malloc(1 * sizeof(char *));
    int parts_len = 0;

    tokenize(input, parts, parts_len);
    async_part(input, parts);
}

void tokenize(char *input, char **parts, int parts_len)
{
    char *token = strtok(input, " \n");

    while (token != NULL) {
        parts[parts_len++] = token;
        parts = realloc(parts, (parts_len + 1) * sizeof(char *));
        token = strtok(NULL, " \n");
    }

    parts[parts_len] = NULL;
    if (!strcmp(parts[0], "exit"))
        exit(0);
    if (!strcmp(parts[0], "cd"))
        chdir(parts[1]);
    for (int i = 0; i < parts_len; i++) {
        if (!strcmp(parts[i], "$SHELL"))
            parts[i] = "cshell";
        if (!strcmp(parts[i], "$USER")) {
            struct passwd *pw = getpwuid(getuid());
            parts[i] = pw->pw_name;
        }
    }
}

void async_part(char *input, char **parts)
{
    int pid = fork();
    if (pid == 0) {
        char *path;
        asprintf(&path, "/bin/%s", parts[0]);
        execv(path, parts);
        free(path);
    } else {
        wait(NULL);
    }
    free(input);
    free(parts);
}
