#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <float.h>
#include "stm32l4xx_hal.h"
#include "main.h"

void help_command(char *arguments);
void lof_command(char *arguments);
void lon_command(char *arguments);
void test_command(char *arguments);
void ts_command(char *arguments);
void ds_command(char *arguments);
void tsl237_command(char *arguments);
void temp_command(char *arguments);
void battery_command(char *arguments);
void prompt();
int parse_command (uint8_t *line, uint8_t **command, uint8_t **args);
int execute_command(uint8_t * line);

void lon();
void lof();
