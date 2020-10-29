#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// get arguments
struct sh_command* command_init(char*);
struct sh_command* parse_command(char*);
void free_command(struct sh_command*);
void test_get_command(struct sh_command*);

// run processes
void run_process(struct sh_command*, int*);
void change_dir(struct sh_command*, int*);
void get_status(int*);
void typed_process(struct sh_command*, int*);

struct sh_command{
  int arg_count;
  char* arg_list[512];
  int background;
  int input_file;
  int output_file;
};
