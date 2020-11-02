#include "smallsh.h"

/*********************************************
* Signal handler for SIGTSTP
*   uses global variable to determine if all 
*   processes are run in the foreground
**********************************************/
void handle_SIGTSTP(int signum){
  if(fg_only){
    char* mode_off = "\nExiting foreground-only mode\n: ";
    write(STDOUT_FILENO, mode_off, 32);
    fg_only = 0;

  }else{
    char* mode_on = "\nEntering foreground-only mode (& will be ignored)\n: ";
    write(STDOUT_FILENO, mode_on, 53);
    fg_only = 1;
  }
  fflush(stdout);
}

/****************************************************
* Signal handler fro SIGINT
*   dipslays a message that process has been eneded
****************************************************/
void handle_SIGINT(int signum){
  status = 2;
  char* new_line = "\n";
  write(STDOUT_FILENO, new_line, 1);
  get_status();
}

/*********************************
* Installs the handler for SIGINT
**********************************/
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

/****************************
* Signal handler for SIGCHLD
*   Prevents zombies
*****************************/
void handle_SIGCHLD(int signum){
  while(waitpid(-1, &status, WNOHANG) > 0){
    }
}

/*********************************
* Used in main to set signals
*********************************/
void set_signals(){
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, handle_SIGTSTP);
  signal(SIGCHLD, handle_SIGCHLD);
}  
