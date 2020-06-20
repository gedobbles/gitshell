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

#define PIPE_READ 0
#define PIPE_WRITE 1

//#define DBG 1   //comment out to switch DBG-output off

/*
    Features to add:
     - history should be project dependant                            ✓
     - allow the colon for external cmds to be not seperated          ✓
     - add cd                                                         ✓
     - add better completion:
       * git command completion                                       ✓
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

       * after ':' program name completion on first level             ✓

*/

void   init(int);

int    start_proc(char**);
int    start_git(char**);
int    start_external(char**);

int    arr_elements(char**);

char** split_line(char*);

//readline completion
void initialize_readline();
int getCompletions(char***, char***);

char* completion_generator(const char *, int);
char** gitsh_completion(const char *, int, int);
char* dupstr(char*);

// constants
char* git = "git";

char* histfile;   //name of the history file

#include "completions.h"
char** gitsh_cmds;
char** gitsh_configs;
char** gitsh_ext_cmds;
char** branches;

char*** completion_set;

int main()
{
    char* input;
    char** args;
    int running = 1;
    int external = 0;   //Flag for external command

    printf("gitsh by gedobbles\ncd to change directory\n" \
           "cdp to switch to new history context\nHit Enter for git help.\n" \
           "Use a preceding colon for external commands.\n" \
           "Always escape spaces in parameters or filenames!\n\n");

    init(0);   //Read history file if existing, else create
    initialize_readline();

    while (running) {
      input = readline("(git) > ");

      //check for exit
      if (strncmp(input,"exit",4) == 0) {
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
  //escaped spaces to 1
  char* ptr = line;
  char* temp = dupstr(line);
  char* tptr = temp;
  ptr++;
  tptr++;
  while (*tptr) {
    if (*tptr == '\\' && *(tptr+1) == ' ') {
      *ptr = 1;
      tptr++;
    }else{
      *ptr = *tptr;
    }
    ptr++;
    tptr++;
  }
  *ptr = 0;
  free(temp);

  int bufsize = TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "gitsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    //rereplace '\1' with ' '
    ptr = token;
    while (*ptr) {
      if (*ptr == 1) {
        *ptr = ' ';
      }
      ptr++;
    }

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


void init(int reinit)
{
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

// readline completion



/* Tell the GNU Readline library how to complete.  We want to try to
   complete on command names if this is the first word in the line, or
   on filenames if not. */
void initialize_readline()
{
  gitsh_cmds = gitsh_cmds_arr;
  gitsh_configs = gitsh_configs_arr;
  //gitsh_ext_cmds
  char* bash    = "bash";
  char* _c      = "-c";
  char* compgen = "compgen -c";
  char** command = (char**)malloc(sizeof(char*)*5);
  command[0] = bash;
  command[1] = _c;
  command[2] = compgen;
  command[3] = _c;
  command[4] = NULL;
  getCompletions(&gitsh_ext_cmds, &command);
  free(command);
  rl_attempted_completion_function = gitsh_completion;
}


char** gitsh_completion (const char* text, int start, int end)
{
  char **matches;
  matches = (char **)NULL;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if (start == 0) {
    if (*text == ':') {
      completion_set = &gitsh_ext_cmds;
    }else{
      completion_set = &gitsh_cmds;
    }
    matches = rl_completion_matches(text, completion_generator);
  }else{
    char* so_far = rl_line_buffer;
    if (strncmp(so_far, "config ", 7) == 0) {
      completion_set = &gitsh_configs;
      matches = rl_completion_matches(text, completion_generator);
    }
  }
  //filename completion if we return NULL
  return (matches);
}

// generator for completions
// @param char*** completion_set
char* completion_generator(const char* text, int state)
{
  static int i, len;
  char *cmd;

  // If this is a new word to complete, initialize now.
  if (!state)
    {
      i = 0;
      len = strlen (text);
    }

  while (cmd = (*completion_set)[i])
    {
      i++;
      if (strncmp (cmd, text, len) == 0) {
        return (dupstr(cmd));
      }
    }
  return ((char *)NULL);
}


char* dupstr(char* s)
{
  char *r;

  r = (char*) malloc (strlen (s) + 1);
  strcpy (r, s);
  return (r);
}

int getCompletions(char*** result, char*** program)
{
  int aStdinPipe[2];
  int aStdoutPipe[2];
  int pidChild, childResult;
  int toChild, fromChild;

  if (pipe(aStdinPipe) < 0) {
    perror("allocating pipe for child input redirect");
    return -1;
  }
  if (pipe(aStdoutPipe) < 0) {
    close(aStdinPipe[PIPE_READ]);
    close(aStdinPipe[PIPE_WRITE]);
    perror("allocating pipe for child output redirect");
    return -1;
  }

  pidChild = fork();
  if (0 == pidChild) {
    // child continues here

    // redirect stdin
    if (dup2(aStdinPipe[PIPE_READ], STDIN_FILENO) == -1) {
      exit(-1);
    }

    // redirect stdout
    if (dup2(aStdoutPipe[PIPE_WRITE], STDOUT_FILENO) == -1) {
      exit(-1);
    }

    // redirect stderr
    if (dup2(aStdoutPipe[PIPE_WRITE], STDERR_FILENO) == -1) {
      exit(-1);
    }

    // all these are for use by parent only
    close(aStdinPipe[PIPE_READ]);
    close(aStdinPipe[PIPE_WRITE]);
    close(aStdoutPipe[PIPE_READ]);
    close(aStdoutPipe[PIPE_WRITE]);

    childResult = execvp(*program[0], *program);

    // if we get here at all, an error occurred, but we are in the child
    // process, so just exit
    exit(childResult);
  } else if (pidChild > 0) {
    // parent continues here

    // close unused file descriptors, these are for child only
    close(aStdinPipe[PIPE_READ]);
    close(aStdoutPipe[PIPE_WRITE]);

    // keep as easier names
    fromChild = aStdoutPipe[PIPE_READ];


    char c;
    char temp[50];
    int temp_i = 0;
    int cmd_i = 0;
    char* cmd_temp[5000];

    while(read(fromChild, &c, 1)!=0)
    {
      if (temp_i == 0) {
        *temp = ':';
        temp_i ++;
      }
      if (c == '\n') {
        temp[temp_i] = 0;
        cmd_temp[cmd_i] = dupstr(temp);
        cmd_i++;
        temp_i = 0;
      }else{
        temp[temp_i] = c;
        temp_i++;
      }
      //write(STDOUT_FILENO, &c, 1);    //DBG
    }
    //last command
    temp[temp_i] = 0;
    cmd_temp[cmd_i] = dupstr(temp);

    *result = (char**)malloc(sizeof(char*)*(cmd_i+1));
    for (int i = 0; i < (cmd_i+1); i++) {
      (*result)[i] = cmd_temp[i];
      //printf("%s\n", gitsh_ext_cmds[i]);  //debug
    }


  } else {
    // failed to create child
    close(aStdinPipe[PIPE_READ]);
    close(aStdinPipe[PIPE_WRITE]);
    close(aStdoutPipe[PIPE_READ]);
    close(aStdoutPipe[PIPE_WRITE]);
    return 1;
  }
  return 0;
}
