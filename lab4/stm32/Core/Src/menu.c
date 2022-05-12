#include "menu.h"

extern RTC_HandleTypeDef hrtc;

typedef struct command {
  char * cmd_string;
  void (*cmd_function)(char * arg);
} command_t;

command_t commands[] = {
  {"help",help_command},
  {"lof",lof_command},
  {"lon",lon_command},
  {"test",test_command},
  {"ts",ts_command},
  {"ds",ds_command},
  {0,0}
};

void help_command(char *arguments) {
  printf("Available Commands:\n\rhelp\n\rlof\n\rlon\n\rtest\n\r");
  printf("OK\r\n");
}
void lof_command(char *arguments) {
  lof();
  printf("OK\r\n");
}
void lon_command(char *arguments) {
  lon();
  printf("OK\r\n");
}
void test_command(char *arguments) {
}
void ts_command(char *arguments){
  RTC_TimeTypeDef current_time;
  char *arg;
  int arg_count = 0;
  if (arguments == NULL) {
    printf("NOK\r\n");
  }
  arg = strtok(arguments,",");
  while (arg != NULL) {
    switch (arg_count) {
      case 0:
        current_time.Hours = (int) strtol(arg,NULL,10);
        if (current_time.Hours > 24 || current_time.Hours < 0) {
          printf("NOK\r\n");
          return;
        }
        break;
      case 1:
        current_time.Minutes = (int) strtol(arg,NULL,10);
        if (current_time.Minutes > 59 || current_time.Minutes < 0) {
          printf("NOK\r\n");
          return;
        }
        break;
      case 2:
        current_time.Seconds = (int) strtol(arg,NULL,10);
        if (current_time.Seconds > 59 || current_time.Seconds < 0) {
          printf("NOK\r\n");
          return;
        }
        break;
      default:
        printf("NOK\r\n");
        return;
        break;
    }
    arg = strtok(NULL,",");
    arg_count++;
  }
  if (arg_count < 3) {
    printf("NOK\r\n");
    return;
  }
  HAL_RTC_SetTime(&hrtc,&current_time,RTC_FORMAT_BIN);
  printf("OK\r\n");
}
void ds_command(char *arguments) {
  RTC_DateTypeDef current_date = {0};
  char *arg;
  int arg_count = 0;
  if (arguments == NULL) {
    printf("NOK\r\n");
  }
  arg = strtok(arguments,",");
  while (arg != NULL) {
    switch (arg_count) {
      case 0:
        current_date.Month = (int) strtol(arg,NULL,10);
        if (current_date.Month > 12 || current_date.Month < 1) {
          printf("NOK\r\n");
          return;
        }
        break;
      case 1:
        current_date.Date = (int) strtol(arg,NULL,10);
        if (current_date.Date > 31 || current_date.Date < 1) {
          printf("NOK\r\n");
          return;
        }
        break;
      case 2:
        current_date.Year = (int) strtol(arg,NULL,10);
        if (current_date.Year < 0 || current_date.Year > 99) {
          printf("NOK\r\n");
          return;
        }
        break;
      default:
        printf("NOK\r\n");
        return;
        break;
    }
    arg = strtok(NULL,",");
    arg_count++;
  }
  if (arg_count < 3) {
    printf("NOK\r\n");
    return;
  }
  HAL_RTC_SetDate(&hrtc,&current_date,RTC_FORMAT_BIN);
  printf("OK\r\n");
}

void prompt() {
  RTC_TimeTypeDef current_time;
  RTC_DateTypeDef current_date;
  HAL_RTC_GetTime(&hrtc,&current_time,RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc,&current_date,RTC_FORMAT_BIN);
  printf("%02d/%02d/%02d %02d:%02d:%02d IULS> ",current_date.Month,current_date.Date,current_date.Year,
                                        current_time.Hours,current_time.Minutes,current_time.Seconds);
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

void lon() {
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}

void lof() {
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}
