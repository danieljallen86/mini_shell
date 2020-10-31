#include "smallsh.h"

int main(){
  char user_input[2048] = "\0";
  int process_status = 0;
  int* pstatus = &process_status;

//  signal(SIGINT, SIG_IGN);

  while (strncmp(user_input, "quit", 4)){
    // show prompt and get input
    printf(": ");
    fgets(user_input, 2048, stdin);

    if(strncmp(user_input,"quit", 4) 
    && user_input[0] != '#' 
    && user_input[0] != '\n'){
      // parse args into linked list
      struct sh_command* command = parse_command(user_input);

      //test_get_command(command);
      run_process(command, pstatus);
      
      fflush(stdout);
      free_command(command);
    }
  }
}
