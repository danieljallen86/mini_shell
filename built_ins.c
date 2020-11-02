#include "smallsh.h"

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

/******************************************************
* Shows a message about the exit status of the process
*   Parameters: None - uses global variable
*   Returns: None
******************************************************/
void get_status(){
  if(status == 0 || status == 1){
    printf("exited value %d\n", status);
  
  // if terminated by a kill signal
  }else{
    printf("terminated by signal %d\n", status);
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
      get_status();
      running[i] = 0; // clear the spot in the array 
    }
  }
}

/**********************************************************
* Kills all existing background processes
*   Parameters: poiner to an array of background pids
*   Returns: None
**********************************************************/
void close_bg(pid_t* running){
  for(int i = 0; i < 512; i++){
    if(running[i] != 0)
      kill(running[i], 15);
  }
}
