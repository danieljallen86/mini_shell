#include "smallsh.h"

/************************************************************
* Runs the process 
*   Parameters: sh_command struct, a poiinter to the satus, 
*     and an array of background pids
*   Returns: None
*************************************************************/
void run_process(struct sh_command* command, int* status, pid_t* running){
  // if there was no command parsed or the first charcter was a #, do nothing
  if(command->arg_list[0] == NULL || command->arg_list[0][0] == '#'){
    ;

  // if cd command was given
  }else if(!strcmp(command->arg_list[0], "cd")){
    change_dir(command);

  // if status built in was given
  }else if(!strcmp(command->arg_list[0], "status")){
    get_status(status);

  // for all other commands
  }else{
    typed_process(command, status, running);
  }
  
  fflush(stdout);
  free_command(command);
}

/*****************************************
* Change Directory (cd) built in command
*   Parameters: sh_command struct
*   Returns: None
******************************************/
void change_dir(struct sh_command* command){
  // if no directory is given, navigate to the home dir
  if(command->arg_count == 1){
    chdir(getenv("HOME"));

  // otherwise navigate to the directory specified
  }else{
    chdir(command->arg_list[1]);
  }
}

/************************************************
* IN PROGRESS
************************************************/
void get_status(int* status){
  if(WIFEXITED(*status)){
    printf("exit value %d\n", WEXITSTATUS(*status));
  }else if(WIFSIGNALED(*status)){
    printf("I was killed!\n");
  }
}

/****************************************************
* Executes the child process from a successful fork
*   Parameters: sh_command, list of background pids
*   Returns: None
*****************************************************/
void child_process(struct sh_command* command, pid_t* running){
  int result;
  
  //set_SIGINT_action();

  // foreground processes ignore CTRL-Z
  if(command->background == 0){
    signal(SIGTSTP, SIG_IGN);
    signal(SIGINT, SIG_DFL);
  }
      
  // set output
  result = dup2(command->output_file, 1);
  if(result == -1){
    exit(2);
  }

  // set input
  result = dup2(command->input_file, 0);
  if(result == -1){
    exit(2);
  }

  // exectute the command
  execvp(command->arg_list[0],command->arg_list);
  // exec only returns if an error
  perror(command->arg_list[0]);
  exit(1);
}

/***********************************************************************
* Handles non-built-in processes
*   Parameters: sh_struct, pointer to status, list of background pids
*   Returns: None
***********************************************************************/
void typed_process(struct sh_command* command, int* status, pid_t* running){
  pid_t child_pid = fork();

  switch(child_pid){
    // if spawning a child fails
    case -1:
      perror("fork() failed\n");
      exit(1);
      break;

    // fork successful
    case 0:
      child_process(command, running);
      break;

    default:
      // if background process, print pid
      if(command->background != 0){
        printf("background pid is %d\n", child_pid);
        track_bg_procs(running, child_pid);
      }
      child_pid = waitpid(child_pid, status, command->background);

      // reset sigint ignore
      signal(SIGINT, SIG_IGN);
      break;
  }
}

/************************************************************************
* Adds a new background process to the list
*   Parameters: pointer to list of background pids, pid to add to list
*   Returns: None
************************************************************************/
void track_bg_procs(pid_t* running, pid_t pid){
  // look through array
  for(int i = 0; i < 512; i++){
    // put new pid in first empty spot
    if(running[i] == 0){
      running[i] = pid;
      break;
    }
  }
}

/**************************************************
* Checks all backgroud processes and cleans list
*   Parameters: pointer to list of background pids
*   Returns: None
***************************************************/
void check_background(pid_t* running){
  // look through array
  for(int i = 0; i < 512; i++){
    // if it's not empty and it is no longer running
    if(running[i] != 0 && kill(running[i], 0)){
      // Let user know it was completed
      printf("background pid %d is done: ", running[i]);
      running[i] = 0; // clear the spot in the array 
    }
  }
}
