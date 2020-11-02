#include "smallsh.h"

int main(){
  char user_input[2048] = "\0";
  status = 0;
  pid_t running[512] = {0};
  set_signals();

  while (strncmp(user_input, "exit", 4)){
    get_user_input(user_input, running);
    
    if(strncmp(user_input,"exit", 4)){

      // parse the command
      struct sh_command* command = parse_command(user_input);

      run_process(command, running);
      fflush(stdout);
    }
  }
  close_bg(running);
  return 0;
}
