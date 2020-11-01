#include "smallsh.h"

void handle_SIGTSTP(int signum){
//    char mode_on[] = "\nEntering foreground-only mode (& will be ignored)\n";
//    char mode_off[] = "\nExiting foreground-only mode\n";
//  if(fg_only){
//    write(STDOUT_FILENO, mode_off, 32);
//    fg_only = 0;
//
//  }else{
//    write(STDOUT_FILENO, mode_on, 53);
//    fg_only = 1;
//  }
char* message = "fuck you!\n";
write(STDOUT_FILENO, message, 10);
}

void set_SIGTSTP_action(){
  struct sigaction SIGTSTP_action = {0};

  // register handler handle_SIGSTP
  SIGTSTP_action.sa_handler = handle_SIGTSTP;
  // block catchable signals
  sigfillset(&SIGTSTP_action.sa_mask);
  // no flags set
  SIGTSTP_action.sa_flags = 0;

  // install handler
  sigaction(SIGTSTP, &SIGTSTP_action, NULL);
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

void set_signals(){
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, handle_SIGTSTP);
  set_SIGTSTP_action();
}  
