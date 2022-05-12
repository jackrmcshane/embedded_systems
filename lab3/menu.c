#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

void help_command(char *arguments) {
  printf("Available Commands:\nhelp\nlof\nlon\ntest\nOK\n");
}
void lof_command(char *arguments) {
}
void lon_command(char *arguments) {
}
void test_command(char *arguments) {
}

int parse_command (uint8_t *line, uint8_t **command, uint8_t **args) {
  // Point command at the beginning of the buffer
  *command = line;
  uint8_t **argptr = args;
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

int main(void)
{
  uint8_t *cmd;
  uint8_t *arg;
  uint8_t buffer[64];
  uint8_t *buff = buffer;
  printf("System Up and Running\n> ");
  while (true) {
    char ch = getchar();
    if (ch == '\n') {
      break;
    }
    *(buff++) = ch;
  }
  (*buff) = '\0';
  parse_command(buffer,&cmd,&arg);
}
