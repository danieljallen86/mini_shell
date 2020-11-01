#include "smallsh.h"

int main(){
  char user_input[2048] = "\0";
  int process_status = 0;
  struct bg_child* cur_running = NULL;

  set_signals();

  while (strncmp(user_input, "quit", 4)){
    get_user_input(user_input);

    if(strncmp(user_input,"quit", 4)   // don't do if "quit"
    && user_input[0] != '#'            // don't do anything with comments
    && user_input[0] != '\n'){         // don't do anything with new lines
      // parse the command
      struct sh_command* command = parse_command(user_input);

      // test_get_command(command);
      run_process(command, &process_status, cur_running);
    }
  }
  //killall with process group
}
