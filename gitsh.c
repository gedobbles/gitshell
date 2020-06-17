#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

#define MAX_HISTORY 128
#define HISTORY_FILE "/.gitsh"
#define HISTORY_FILE_CHRS 8   //make sure this is correct! (len+1 for \0)

//#define DBG 1   //comment out to switch DBG-output off

/*
    Features to add:
     - history should be project dependant                            ✓
     - allow the colon for external cmds to be not seperated          ✓
     - add cd                                                         ✓
     - add better completion:
       * git command completion
          ~ branch: branches
          ~ checkout: branches
          ~ config: (some) variables
          ~ fetch: remotes
          ~ help
          ~ log: branches
          ~ merge: branches
          ~ pull: remotes
          ~ push: remotes
          ~ rebase: branches
          ~ remote:
             # add get-url prune remove rename set-branches
             # set-head set-url show update
          ~ reset: branches
          ~ show: branches

       * after ':' program name completion on first level

*/

void   init(int);

int    start_proc(char**);
int    start_git(char**);
int    start_external(char**);

int    arr_elements(char**);

char** split_line(char*);

// constants
char* git = "git";

char* histfile;   //name of the history file

int main()
{
    char* input;
    char** args;
    int running = 1;
    int external = 0;   //Flag for external command

    printf("gitsh by gedobbles\ncd to change directory\n" \
           "cdp to switch to new history context\nHit Enter for git help.\n" \
           "Use a preceding colon for external commands.\n\n");

    init(0);   //Read history file if existing, else create

    while (running) {
      input = readline("(git) > ");

      //check for exit
      if (strcmp(input,"exit") == 0) {
        running = 0;
        continue;
      }

      add_history(input);
      stifle_history(MAX_HISTORY);

      //check for external command (':')
      external = 0;
      if (input[0] == ':') {
        input[0] = ' ';
        external ++;
      }

      args = split_line(input);       //Remember to free args !!!

      //check for internals
      if (args[0] != NULL && strcmp(args[0], "cd") == 0) {
        chdir(args[1]);
        free(args);                   //frees args :-)
        continue;
      }

      if (args[0] != NULL && strcmp(args[0], "cdp") == 0) {
        if (chdir(args[1]) == 0) {
          init(1);
        }
        free(args);                   //frees args :-)
        continue;
      }

      if (external != 0) {
        start_external(args);         //frees args :-)
      }else{
        start_git(args);              //frees args :-)
      }
      free(input);
    }

    write_history(histfile);
    free(histfile);
    return 0;
}


int start_external(char** args)
{
  int elements = arr_elements(args);
  char** cmd = (char**)malloc((elements + 1)*sizeof(char*));

  cmd[elements] = NULL;
  for (int i = 0; i < elements; i++) {
    cmd[i] = args[i];
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


void init(int reinit) {
  char* dir = getcwd(NULL, 0);
  int dirlen = 0;
  while (dir[dirlen] != 0) {
    dirlen++;
  }
  dirlen++;
  dirlen += HISTORY_FILE_CHRS;
  if (reinit) {
    write_history(histfile);
    clear_history();
    free(histfile);
  }
  histfile = (char*)malloc(dirlen*sizeof(char));
  histfile[0] = 0;
  strcat(histfile, dir);
  strcat(histfile, HISTORY_FILE);
  free(dir);

  int res = read_history(histfile);
  if (res == 2) {
    char* touch = "touch";
    char ** a = malloc(sizeof(touch)*3);
    a[0] = touch;
    a[1] = histfile;
    a[2] = NULL;
    start_proc(a);
    free(a);
  }
}
