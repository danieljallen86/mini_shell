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

void handle_SIGINT(int signum){
  char* message = "\nTerminated by signal 2\n";
  write(STDOUT_FILENO, message, 24);
}

void set_SIGINT_action(){
  struct sigaction SIGINT_action = {0};

  // register handler
  SIGINT_action.sa_handler = handle_SIGINT;
  // block all catchable signals while handle_SIGINT is running
  sigfillset(&SIGINT_action.sa_mask);
  // No flags set
  SIGINT_action.sa_flags = 0;
  // install signal handler
  sigaction(SIGINT, &SIGINT_action, NULL);
}

void child_process(struct sh_command* command){
  int result;

  // foreground processes ignore CTRL-Z
  if(command->background == 0)
    signal(SIGSTOP, SIG_IGN);

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
//      set_SIGINT_action();
      // if background process, print pid
      if(command->background != 0){
        printf("background pid is %d\n", getpid());
      }
      child_pid = waitpid(child_pid, status, command->background);

      // reset sigint ignore
//      signal(SIGINT, SIG_IGN);
      break;
  }
}
