#include "smallsh.h"

void run_process(struct sh_command* command, int* status, struct bg_child* cur_running){
  if(command->arg_list[0] == NULL || command->arg_list[0][0] == '#'){
    ;
  }else if(!strcmp(command->arg_list[0], "cd")){
    change_dir(command, status);
  }else if(!strcmp(command->arg_list[0], "status")){
    get_status(status);
  }else{
    typed_process(command, status, cur_running);
  }
  fflush(stdout);
  free_command(command);
}

void change_dir(struct sh_command* command, int* status){
  if(command->arg_count == 1){
    chdir(getenv("HOME"));
  }else{
    chdir(command->arg_list[1]);
  }
}

void get_status(int* status){
  printf("exit value %d\n", *status);
}

void child_process(struct sh_command* command, struct bg_child* cur_running){
  int result;
  
  set_SIGINT_action();

  // foreground processes ignore CTRL-Z
  if(command->background == 0){
    signal(SIGTSTP, SIG_IGN);
  }
      
  result = dup2(command->output_file, 1);
  if(result == -1){
    perror("dup2");
    exit(2);
  }

  result = dup2(command->input_file, 0);
  if(result == -1){
    perror("dup2");
    exit(2);
  }
  execvp(command->arg_list[0],command->arg_list);
  // exec only returns if an error
  perror(command->arg_list[0]);
  exit(2);
}

void typed_process(struct sh_command* command, int* status, struct bg_child* cur_running){
  pid_t child_pid = fork();

  switch(child_pid){
    // if spawning a child fails
    case -1:
      perror("fork() failed\n");
      exit(1);
      break;

    // fork successful
    case 0:
      child_process(command, cur_running);
      break;

    default:
      // if background process, print pid
      if(command->background != 0){
        printf("background pid is %d\n", child_pid);
        // add to the list of running process
        //cur_running = add_bg_process(cur_running, child_pid);
      }
      child_pid = waitpid(child_pid, status, command->background);
      *status = (*status != 0) ? 1 : 0;

      // reset sigint ignore
      signal(SIGINT, SIG_IGN);
      break;
  }
}

// add a process - takes pointer to linked list head and pid
struct bg_child* add_bg_process(struct bg_child* cur_running, pid_t pid){
printf("head pid: %d", cur_running->pid ? cur_running->pid : 0);
  struct bg_child* head = cur_running;
  // create a new new node
  struct bg_child* new_proc = malloc(sizeof(struct bg_child));
  new_proc->pid = pid;
  new_proc->next = NULL;
printf("new child pid: %d", new_proc->pid);
  // linked list empty (head = NULL)
  if(cur_running == NULL){
    head = new_proc;

  }else{
    while(cur_running->next){
      cur_running = cur_running->next;
    }
    cur_running->next = new_proc;
  }
printf("head pid: %d", cur_running->pid ? cur_running->pid : 0);
  return head;
}
