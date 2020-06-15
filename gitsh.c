#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

//#define DBG 1   //comment out to switch DBG-output off

int    start_proc(char**);
int    start_git(char**);
char** split_line(char*);

char* git = "git";

int main()
{
    char* input;
    char** args;
    int running = 1;

    printf("gitsh by gedobbles\nHit Enter for help.\n\n");

    while (running) {
      input = readline("(git) > ");
      if (strcmp(input,"exit")==0) {
        running = 0;
        continue;
      }
      add_history(input);
      start_git(split_line(input));
    }
    return 0;
}


int start_git(char** args)
{
  int elements = sizeof(args) / sizeof(char*);
  char** cmd = (char**)malloc((elements + 2)*sizeof(char*));

  cmd[0] = git;
  cmd[elements+1] = NULL;
  for (int i = 0; i < elements; i++) {
    cmd[i+1] = args[i];
  }
  free(args);
  start_proc(cmd);
  free(cmd);
}

int start_proc(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
#ifdef DBG
    printf("%s %s\n", args[0], args[1]);
#endif
    if (execvp(args[0], args) == -1) {
      perror("Error starting process");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("Error starting process (fork)");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}


char** split_line(char *line)
{
  int bufsize = TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "gitsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "gitsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}
