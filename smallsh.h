#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct sh_command{
  int arg_count;
  char* arg_list[512];
  int background;
  int input_file;
  int output_file;
};

struct bg_child{
  int pid;
  struct bg_child* next;
};

// signals
void handle_SIGTSTP(int);
void set_fg_only();
void handle_SIGINT(int);
void set_SIGINT_action();
void set_signals();
void handle_SIGTERM();
void quit_shell(pid_t*);

// get arguments
void get_user_input(char* user_input, pid_t*);
struct sh_command* command_init();
struct sh_command* parse_command(char*);
void free_command(struct sh_command*);
void test_get_command(struct sh_command*);
void dollar_sign_swap(char*);

// run processes
void run_process(struct sh_command*, int*, pid_t*);
void change_dir(struct sh_command*);
void get_status(int*);
void typed_process(struct sh_command*, int*, pid_t*);
void track_bg_procs(pid_t* running, pid_t pid);
void check_background(pid_t*);

int fg_only;
