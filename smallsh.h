#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// signals
void handle_SIGTSTP(int);
void set_fg_only();
void handle_SIGINT(int);
void set_SIGINT_action();

// get arguments
void get_user_input(char*);
struct sh_command* command_init(char*);
struct sh_command* parse_command(char*);
void free_command(struct sh_command*);
void test_get_command(struct sh_command*);
void dollar_sign_swap(char*);

// run processes
void run_process(struct sh_command*, int*);
void change_dir(struct sh_command*, int*);
void get_status(int*);
void typed_process(struct sh_command*, int*);
void child_process(struct sh_command*);

struct sh_command{
  int arg_count;
  char* arg_list[512];
  int background;
  int input_file;
  int output_file;
};

int fg_only;
