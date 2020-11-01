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

void get_user_input(char* user_input){
  // show prompt and get input
  printf(": ");
  fgets(user_input, 2048, stdin);

  dollar_sign_swap(user_input);
}

struct sh_command* command_init(char* user_input){
  struct sh_command* command = malloc(sizeof(struct sh_command));
  
  for(int i = 0; i < 512; i++)
    command->arg_list[i] = NULL;
  
  command->background = 0;
  command->input_file = 0;
  command->output_file = 1;
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
        command->input_file = open(token, O_RDONLY, 0666);
        if(command->input_file == -1){
          perror("input redirection failed\n");
        }
      
      // if > is parsed get the output file file descriptor
      }else if(!strcmp(token, ">")){
        token = strtok_r(NULL, " \n", &saveptr);  // get the input file
        command->output_file = open(token, O_RDWR | O_CREAT, 0666);
        if(command->output_file == -1){
          perror("output redirection failed\n");
        }
  
      // if & is parsed it runs in the background
      }else if(!strcmp(token, "&")){
        // if forground only mode is set, no background running
        if(!fg_only){
          command->background = WNOHANG;
          command->output_file = open("/dev/null", O_RDWR, 222);
          if(command->output_file == -1){
            perror("/dev/null not opened\n");
          }  
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

  // if output not set, set to standard out
  if(command->output_file == 0)
    command->output_file = 1;

  return command;
}

void dollar_sign_swap(char* user_input){
  int i = 0;
  char new_input[2048] = "\0";
  char* to_replace = "$$";
  char new_str[10];
  sprintf(new_str,"%d", (int)getpid());
  int old_len = 2;
  int new_len = strlen(new_str);
  char* to_compare = user_input;

  while(*to_compare){
    // if the substring matches, put new string in the new_input
    if(strstr(to_compare, to_replace) == to_compare){
      strcpy(&new_input[i], new_str);
      i += new_len;
      to_compare += old_len;
    
    // otherwise continue copying the strings
    }else{
      new_input[i++] = *to_compare++;
    }
  }
  strcpy(user_input, new_input);
}

void free_command(struct sh_command* command){
  for(int i = 0; i < command->arg_count; i++)
    free(command->arg_list[i]);

  free(command);
}
