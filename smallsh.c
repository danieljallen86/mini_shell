#include "smallsh.h"

int main(){
  printf("\n\n");
  char user_input[2048];

  while (strcmp(user_input, "quit\n")){
    // show prompt and get input
    printf(": ");
    fgets(user_input, 2048, stdin);

    printf("%s\n", user_input); // test line
    if(!strcmp(user_input, "cd\n")){
      char* lui[] = {"cd"};
      execlp("cd","cd","~", NULL);
      system("pwd");
    }

    // parse args into linked list
    char* prog_arg = get_arg_list(user_input);

    // execvp(
  }
}
