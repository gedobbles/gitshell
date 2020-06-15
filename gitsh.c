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
int    start_external(char**);

int    arr_elements(char**);

char** split_line(char*);

char* git = "git";

int main()
{
    char* input;
    char** args;
    int running = 1;

    printf("gitsh by gedobbles\nHit Enter for help.\n" \
           "Use a colon seperated by a space for external commands.\n\n");

    while (running) {
      input = readline("(git) > ");
      if (strcmp(input,"exit") == 0) {
        running = 0;
        continue;
      }
      if (strcmp(input,"") == 0) {
        continue;
      }
      add_history(input);
      args = split_line(input);       //Remember to free args !!!
      if (strcmp(args[0],":")== 0) {
        start_external(args);         //frees args :-)
      }else{
        start_git(args);              //frees args :-)
      }
    }
    return 0;
}


int start_external(char** args)
{
  int elements = arr_elements(args);
  char** cmd = (char**)malloc((elements + 1)*sizeof(char*));

  cmd[elements] = NULL;
  for (int i = 0; i < elements; i++) {    //Strip off colon
    cmd[i] = args[i+1];
  }
  free(args);
  start_proc(cmd);
  free(cmd);
}

int start_git(char** args)
{
  int elements = arr_elements(args);
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
    printf("%s %s %s\n", args[0], args[1], args[2]);
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

int arr_elements(char** args)
{
  int e = 0;
  while (args[e] != NULL) {
    e++;
  }
  return e++;
}
