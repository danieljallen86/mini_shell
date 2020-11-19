#include "smallsh.h"

/************************************************************
* Runs the process 
*   Parameters: sh_command struct, a poiinter to the satus, 
*     and an array of background pids
*   Returns: None
*************************************************************/
void run_process(struct sh_command* command, pid_t* running){
  // if there was no command parsed or the first charcter was a #, do nothing
  if(command->arg_list[0] == NULL || command->arg_list[0][0] == '#'){
    ;

  // if cd command was given
  }else if(!strcmp(command->arg_list[0], "cd")){
    change_dir(command);

  // if status built in was given
  }else if(!strcmp(command->arg_list[0], "status")){
    get_status();

  // for all other commands
  }else{
    typed_process(command, running);
  }
  
  fflush(stdout);
  free_command(command);
}

/****************************************************
* Executes the child process from a successful fork
*   Parameters: sh_command, list of background pids
*   Returns: None
*****************************************************/
void child_process(struct sh_command* command, pid_t* running){
  int result;
  // children ignor STGTSTP
  signal(SIGTSTP, SIG_IGN);
  
  // foreground processes ignore CTRL-Z
  if(command->background != WNOHANG){
    signal(SIGINT, SIG_DFL);
  }

  // set output
  result = dup2(command->output_file, 1);
  if(result == -1){
    exit(EXIT_FAILURE);
  }

  // set input
  result = dup2(command->input_file, 0);
  if(result == -1){
    exit(EXIT_FAILURE);
  }

  // exectute the command
  execvp(command->arg_list[0],command->arg_list);

  // exec only returns if an error
  perror(command->arg_list[0]);
  exit(EXIT_FAILURE);
}

/***********************************************************************
* Handles non-built-in processes
*   Parameters: sh_struct, pointer to status, list of background pids
*   Returns: None
***********************************************************************/
void typed_process(struct sh_command* command, pid_t* running){
  pid_t child_pid = fork();

  switch(child_pid){
    // if spawning a child fails
    case -1:
      perror("fork() failed\n");
      exit(EXIT_FAILURE);
      break;

    // fork successful
    case 0:
      child_process(command, running);
      break;

    default:
      set_SIGINT_action();
      // if background process, print pid
      if(command->background != 0){
        printf("background pid is %d\n", child_pid);
        track_bg_procs(running, child_pid);
      }

      child_pid = waitpid(child_pid, &status, command->background);
      
      if(WIFEXITED(status)){
        status = WEXITSTATUS(status);
        
      }else if(WIFSIGNALED(status)){
        status = WTERMSIG(status);
      }

      // call a second time to see if background process finished
      child_pid = waitpid(-1, &status, WNOHANG);
      
      // reset sigint ignore
      signal(SIGINT, SIG_IGN);
      break;
  }
}
