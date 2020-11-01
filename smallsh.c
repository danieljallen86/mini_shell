#include "smallsh.h"

void handle_SIGTERM(){
  exit(0);
}

void quit_shell(){
  signal(SIGTERM, handle_SIGTERM);
  kill(0, SIGTERM);
}

int main(){
  char user_input[2048] = "\0";
  int process_status = 0;
  struct bg_child* cur_running = NULL;

  set_signals();

  while (strncmp(user_input, "quit", 4)){
    // check background processes
    //check_background(cur_running);
    printf("pid of first background process is %d", cur_running == NULL ? 000 : cur_running->pid);
      
    get_user_input(user_input);
    
    if(strncmp(user_input,"quit", 4)){

      // parse the command
      struct sh_command* command = parse_command(user_input);

      // test_get_command(command);
      run_process(command, &process_status, cur_running);
      fflush(stdout);
    }
  }
  quit_shell();
}
