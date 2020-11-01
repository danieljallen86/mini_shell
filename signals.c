#include "smallsh.h"

void handle_SIGTSTP(int signum){
  if(fg_only){
    char* mode_off = "\nExiting foreground-only mode\n: ";
    write(STDOUT_FILENO, mode_off, 33);
    fg_only = 0;

  }else{
    char* mode_on = "\nEntering foreground-only mode (& will be ignored)\n: ";
    write(STDOUT_FILENO, mode_on, 53);
    fg_only = 1;
  }
}

void set_fg_only(){
  struct sigaction SIGTSTP_action = {0};

  // register handler
  SIGTSTP_action.sa_handler = handle_SIGTSTP;
  // block all catchable signals while handle_SIGINT is running
  sigfillset(&SIGTSTP_action.sa_mask);
  // No flags set
  SIGTSTP_action.sa_flags = 0;
  // install signal handler
  sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}

void handle_SIGINT(int signum){
  char* message = "\nTerminated by signal 2\n";
  write(STDOUT_FILENO, message, 24);
  exit(0);
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

void set_signals(){
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, handle_SIGTSTP);
  //set_fg_only();
}  
