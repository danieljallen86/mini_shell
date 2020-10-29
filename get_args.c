#include "smallsh.h"

void test_get_command(struct sh_command* command){
  for(int i = 0; i < command->arg_count; i++){
    printf("%s\n", command->arg_list[i]);
  }
  if(command->input_file)
    printf("input from %d\n", command->input_file);
  if(command->output_file)
    printf("output to %d\n", command->output_file);
  if(command->background)
    printf("to the back\n");
  else
    printf("in the front\n");
}

struct sh_command* command_init(char* user_input){
  struct sh_command* command = malloc(sizeof(struct sh_command));
  
  for(int i = 0; i < 512; i++)
    command->arg_list[i] = NULL;
  
  command->background = 0;
  command->input_file = 0;
  command->output_file = 0;
  command->arg_count = 0;

  return command;
}

struct sh_command* parse_command(char* user_input){
  struct sh_command* command = command_init(user_input);
  char *saveptr;

  // parse command
  char* token = strtok_r(user_input, " \n", &saveptr);
  char* arg = calloc(strlen(token) + 1, sizeof(char)); 
  strcpy(arg, token);
  command->arg_list[command->arg_count] = arg;
  command->arg_count++;

  // parse arguments
  do{
    token = strtok_r(NULL, " \n", &saveptr);
    if(token){
      // if < is parsed get the input file file descriptor 
      if(!strcmp(token, "<")){
        token = strtok_r(NULL, " \n", &saveptr);  // get the input file
        command->input_file = open(token, O_RDONLY, 444);
        if(command->input_file == -1){
          perror("input redirection failed\n");
        }
      
      // if > is parsed get the output file file descriptor
      }else if(!strcmp(token, ">")){
        token = strtok_r(NULL, " \n", &saveptr);  // get the input file
        command->output_file = open(token, O_CREAT | O_WRONLY, 222);
        if(command->output_file == -1){
          perror("output redirection failed\n");
        }
  
      // if & is parsed it runs in the background
      }else if(!strcmp(token, "&")){
        command->background = WNOHANG;
        command->output_file = open("/dev/null", O_RDWR, 222);
        if(command->output_file == -1){
          perror("/dev/null not opened\n");
        }  
      // otherwise its an argument
      }else{
        char* arg = calloc(strlen(token) + 1, sizeof(char)); 
        strcpy(arg, token);
        command->arg_list[command->arg_count] = arg;
        command->arg_count++;
      }
    }
  }while(token);

  return command;
}

void free_command(struct sh_command* command){
  for(int i = 0; i < command->arg_count; i++)
    free(command->arg_list[i]);

  free(command);
}
