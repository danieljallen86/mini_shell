CS344 Project 3 = Smallsh
Author: Dan Allen

The program emulates many functionalities of a bash shell including three built in functions:
  - cd:     change directories to the specified location. If no location is given, changes to HOME directory.
  - status: returns a message regarding the last exit status or terminating signal of a process. 
  - exit:   closes all child processes of smallsh running before returning.

Other built in functionality:
  - Redirect input (<):     to redirect input in from a source not stdin, use '<' followed by the input file
  - Redirect output (<):    to redirect output to a source not stdout, use '<' followed by the destination file
  - Background process (&): to run processes in the background, enter '&' as the last argument
  
  NOTE FOR ALL OPERATORS: white space (spaces/new line) must surround them to function properly

Variable expansion:
  - Any instance of '$$' will be expanded to the pid of smallsh. 

Signals:
  - CTRL-C will be ignored by smallsh, but will terminate child process w/ a signal of 2
  - CTRL-Z will put the shell in foreground only mode where any subsequent '&' will be ignored
    until CTRL-Z is pressed again returning smallsh to normal operation

Background Processes:
  Background processes, when launched, will print the pid of the child to stdout. When the child ends,
  it will terminate and will print a message before the next prompt with the exit status.


COMPILE & RUN

To compile and run smallsh, execute the following:

./make_smallsh.sh
./smallsh
