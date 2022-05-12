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
void sample_command(char *arguments);
void erase_flash_command(char *arguments);
void prompt();
int parse_command (uint8_t *line, uint8_t **command, uint8_t **args);
int execute_command(uint8_t * line);
void log_command(char * arguments);
void data_command(char * arguments);
void unpack(uint32_t packedBits,uint8_t *month, uint8_t *day, uint8_t *year, uint8_t *hours, uint8_t *minutes, uint8_t *seconds);
uint32_t pack(uint8_t month, uint8_t day, uint8_t year,
  uint8_t hours, uint8_t minutes, uint8_t seconds);
void lon();
void lof();
float read_light_sensor(void);
