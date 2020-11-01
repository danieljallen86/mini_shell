#include "smallsh.h"

void run_process(struct sh_command* command, int* status, struct bg_child* cur_running){
  if(!strcmp(command->arg_list[0], "cd")){
    change_dir(command, status);
  }else if(!strcmp(command->arg_list[0], "status")){
    get_status(status);
  }else{
    typed_process(command, status, cur_running);
  }
  fflush(stdout);
  free_command(command);
}

void change_dir(struct sh_command* command, int* status){
  if(command->arg_count == 1){
    chdir(getenv("HOME"));
  }else{
    chdir(command->arg_list[1]);
  }
}

void get_status(int* status){
  printf("exit value %d\n", *status);
}

void child_process(struct sh_command* command){
  int result;
  
  set_SIGINT_action();

  // foreground processes ignore CTRL-Z
  if(command->background == 0){
    signal(SIGTSTP, SIG_IGN);

  // otherwise track it as still running
  }else{
    printf("track this!\n");
  }
      
  result = dup2(command->output_file, 1);
  if(result == -1){
    perror("dup2");
    exit(2);
  }

  result = dup2(command->input_file, 0);
  if(result == -1){
    perror("dup2");
    exit(2);
  }
  execvp(command->arg_list[0],command->arg_list);
  // exec only returns if an error
  perror(command->arg_list[0]);
  exit(2);
}

void typed_process(struct sh_command* command, int* status, struct bg_child* cur_running){
  int result;
  pid_t child_pid = fork();

  switch(child_pid){
    // if spawning a child fails
    case -1:
      perror("fork() failed\n");
      exit(1);
      break;

    // fork successful
    case 0:
      child_process(command);
      break;

    default:
      // if background process, print pid
      if(command->background != 0){
        printf("background pid is %d\n", getpid());
      }
      child_pid = waitpid(child_pid, status, command->background);
      *status = (*status != 0) ? 1 : 0;

      // reset sigint ignore
      signal(SIGINT, SIG_IGN);
      break;
  }
}
