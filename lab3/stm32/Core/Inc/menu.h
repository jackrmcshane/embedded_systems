#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "main.h"

void help_command(char *arguments);
void lof_command(char *arguments);
void lon_command(char *arguments);
void test_command(char *arguments);
int parse_command (uint8_t *line, uint8_t **command, uint8_t **args);
int execute_command(uint8_t * line);
