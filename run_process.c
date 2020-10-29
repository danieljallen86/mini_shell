#include "smallsh.h"

void run_process(struct sh_command* command, int* status){
  if(!strcmp(command->arg_list[0], "cd")){
    change_dir(command, status);
  }else if(!strcmp(command->arg_list[0], "status")){
    get_status(status);
  }else{
    typed_process(command, status);
  }
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

void typed_process(struct sh_command* command, int* status){

  pid_t child_pid = fork();

  switch(child_pid){
    // if spawning a child fails
    case -1:
      perror("fork() failed\n");
      exit(1);
      break;

    // fork successful
    case 0:
      execvp(command->arg_list[0],command->arg_list);
      // exec only returns if an error
      perror("execution of process failed\n");
      exit(2);
      break;

    default:
      child_pid = waitpid(child_pid, status, command->background);
      break;
  }
}
