#include "menu.h"

typedef struct command {
  char * cmd_string;
  void (*cmd_function)(char * arg);
} command_t;

command_t commands[] = {
  {"help",help_command},
  {"lof",lof_command},
  {"lon",lon_command},
  {"test",test_command},
  {0,0}
};

void help_command(char *arguments) {
  printf("Available Commands:\n\rhelp\n\rlof\n\rlon\n\rtest\n\r");
}
void lof_command(char *arguments) {
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}
void lon_command(char *arguments) {
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}
void test_command(char *arguments) {
}

int parse_command (uint8_t *line, uint8_t **command, uint8_t **args) {
  // Point command at the beginning of the buffer
  *command = line;
  for (;;line++) {
    if (*line == '\0') {
      *args = NULL;
      break;
    } else if (*line == ',') {
      *line = '\0';
      *args = (line+1);
      break;
    }
  }
  return 0;
}

int execute_command(uint8_t * line) {
  uint8_t *cmd;
  uint8_t *arg;
  command_t *p = commands;
  int success = 0;

  if (!line) {
    return (-1); // Passed a bad pointer 
  }
  if (parse_command(line,&cmd,&arg) == -1) {
    printf("Error with parse command\n\r");
    return (-1);
  }
  while (p->cmd_string) {
    if (!strcmp(p->cmd_string,(char *) cmd)) {
      if (!p->cmd_function) {
        return (-1);
      }
      (*p->cmd_function)((char *)arg); // Run the command with the passed arguments
      success = 1;
      break;
    }
    p++;
  }
  if (success) {
    return (0);
  }
  else {
    return (-1);
  }
}