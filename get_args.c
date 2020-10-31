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

  // expand $$ on user_input
  dollar_sign_expansion(user_input);

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
        command->background = WNOHANG;
        
        // if no output file specified, send to /dev/null
       if(command->output_file == 0){
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

void free_command(struct sh_command* command){
  for(int i = 0; i < command->arg_count; i++)
    free(command->arg_list[i]);

  free(command);
}

// adapted from https://www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/ 
void dollar_sign_expansion(char* user_input){
  char new_input[2048];
  char* to_replace = "$$";
  int old_len = 2;
  char str_pid[10];
  sprintf(str_pid, "%d", getpid());
  int new_len = strlen(str_pid);
  int i, count = 0;

  // count number of times to_replace occurs
  for(i = 0; user_input[i] != '\0'; i++){
    if(strstr(&user_input[i], to_replace) == &user_input[i]){
      count ++;

      // jump to index after to_replace
      i++;
    }
  }

  if(count){
    i = 0;
    char* to_copy = user_input;
    while(*to_copy){
      // compare substring with result 
      if((strstr(to_copy, to_replace)) == to_copy){
        strcpy(&new_input[i], str_pid);
        i += new_len;
        to_copy += old_len;
      }else{
        new_input[i++] = *to_copy++;
      }
    }
    new_input[i] = '\0';
  
    strcpy(user_input, new_input);
  }
}
