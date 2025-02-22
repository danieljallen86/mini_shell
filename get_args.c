#include "smallsh.h"

/*************************************************
* Function gets user input
*   Parameters: pointer to char array for input, 
*     array of background pids, and ptr to status
*   Returns: None
**************************************************/
void get_user_input(char* user_input, pid_t* running){
  // checck for any background processes that have ended
  check_background(running);

  // show prompt and get input
  printf(": ");
  fgets(user_input, 2048, stdin);

  // ignore white space
  while(*user_input == ' ')
    user_input++;

  dollar_sign_swap(user_input);
}

/*********************************************
* Initializes an sh_command structure
*   Parameters: None
*   Returns: pointer to an sh_command struct
**********************************************/
struct sh_command* command_init(){
  struct sh_command* command = malloc(sizeof(struct sh_command));
  
  for(int i = 0; i < 512; i++)
    command->arg_list[i] = NULL;
  
  command->background = 0;
  command->input_file = 0;
  command->output_file = 1;
  command->arg_count = 0;

  return command;
}

/**********************************************
* Parses user input into an sh_command struct
*   Parameters: char array of user input
*   Returns: pointer to an sh_command struct
***********************************************/
struct sh_command* parse_command(char* user_input){
  struct sh_command* command = command_init();
  char *saveptr;

  // parse command
  char* token = strtok_r(user_input, " \n", &saveptr);
  if(token){
    char* arg = calloc(strlen(token) + 1, sizeof(char)); 
    strcpy(arg, token);
    command->arg_list[command->arg_count] = arg;
    command->arg_count++;
  } 

  // parse arguments
  do{
    token = strtok_r(NULL, " \n", &saveptr);
    if(token){
      // if < is parsed get the input file file descriptor 
      if(!strcmp(token, "<")){
        token = strtok_r(NULL, " \n", &saveptr);  // get the input file
        command->input_file = open(token, O_RDONLY, 0666);
        if(command->input_file == -1){
          printf("cannot open %s for input\n", token);
        }
      
      // if > is parsed get the output file file descriptor
      }else if(!strcmp(token, ">")){
        token = strtok_r(NULL, " \n", &saveptr);  // get the input file
        command->output_file = open(token, O_RDWR | O_CREAT, 0666);
        if(command->output_file == -1){
          printf("cannot open %s for output\n", token);
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

/****************************************************************************
* Replaces all instances of '$$' with the pid in the user input
*   Parameters: char array of user input
*   Returns: a char array with $$ replaced with the pid
*   Credit: based on code from 
*     www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/
****************************************************************************/
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

/*********************************************
* Frees memory allocated for command struct
*   Parameters: pointer to command struct
*   Returns: None
**********************************************/
void free_command(struct sh_command* command){
  for(int i = 0; i < command->arg_count; i++)
    free(command->arg_list[i]);

  free(command);
}
