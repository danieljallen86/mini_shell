#include "smallsh.h"

int main(){
  char user_input[2048] = "\0";
  int process_status = 0;
  int* pstatus = &process_status;
//  fg_only = 0;

  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, handle_SIGTSTP);
  //set_fg_only();

  while (strncmp(user_input, "quit", 4)){
    get_user_input(user_input);

    if(strncmp(user_input,"quit", 4)   // don't do if "quit"
    && user_input[0] != '#'            // don't do anything with comments
    && user_input[0] != '\n'){         // don't do anything with new lines
      // parse the command
      struct sh_command* command = parse_command(user_input);

      // test_get_command(command);
      run_process(command, pstatus);
    }
  }
}
