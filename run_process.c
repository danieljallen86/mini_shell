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

void child_process(struct sh_command* command){
  int result;

  // set output stream if different than stdout
  result = dup2(command->output_file, 1); 
  if(result == -1){
    perror("dup2");
    exit(2);
  }

  // set input stream if different than stdin
  result = dup2(command->input_file, 0);
  if(result == -1){
    perror("dup2");
    exit(2);
  }

  // run the command
  execvp(command->arg_list[0],command->arg_list);
  // exec only returns if an error
  perror(command->arg_list[0]);
  exit(2);
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
      child_process(command);
      break;

    default:
      child_pid = waitpid(child_pid, status, command->background);
      break;
  }
}
