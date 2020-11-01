#include "smallsh.h"


void quit_shell(pid_t* running){
  for(int i = 0; i < 512; i++){
    if(running[i] != 0)
      kill(running[i], 15);
  }
}

int main(){
  char user_input[2048] = "\0";
  int process_status = 0;
  pid_t running[512] = {0};

  set_signals();

  while (strncmp(user_input, "exit", 4)){
    get_user_input(user_input, running);
    
    if(strncmp(user_input,"exit", 4)){

      // parse the command
      struct sh_command* command = parse_command(user_input);

      // test_get_command(command);
      run_process(command, &process_status, running);
      fflush(stdout);
    }
  }
  quit_shell(running);
  return 0;
}
