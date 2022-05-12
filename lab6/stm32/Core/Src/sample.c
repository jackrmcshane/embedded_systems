#include "main.h"


extern uint32_t period;
extern flash_status_t fs;
extern RTC_HandleTypeDef hrtc;


void sample(void){
  sensordata_t sd;
  RTC_TimeTypeDef current_time;
  RTC_DateTypeDef current_date;
  HAL_RTC_GetTime(&hrtc,&current_time,RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc,&current_date,RTC_FORMAT_BIN);
  
  sd.watermark = 1;
  sd.status = 1;
  sd.record_number = fs.next_record_number;
  sd.battery_voltage = ( uint16_t )read_vrefint();
  sd.temperature = ( uint16_t )read_temp();
  sd.sensor_period = (uint32_t)(read_light_sensor() * 1000);
  sd.timestamp = pack((uint8_t)current_date.Month,
                      (uint8_t)current_date.Date,
                      (uint8_t)current_date.Year,
                      (uint8_t)current_time.Hours,
                      (uint8_t)current_time.Minutes,
                      (uint8_t)current_time.Seconds); 

  if (write_record( (void *)&sd ) == -1){
    printf("NOK\r\n");
    return;
  }

  printf("OK\r\n");
}


/*
 * need to fix the timestamp field for the logdata_t structure
 */
void log_event(uint8_t *msg){

  logdata_t log;
  RTC_TimeTypeDef current_time;
  RTC_DateTypeDef current_date;
  HAL_RTC_GetTime(&hrtc,&current_time,RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc,&current_date,RTC_FORMAT_BIN);

  log.watermark = 1;
  log.status = 2;
  log.record_number = fs.next_record_number;
  log.timestamp = pack((uint8_t)current_date.Month,
                      (uint8_t)current_date.Date,
                      (uint8_t)current_date.Year,
                      (uint8_t)current_time.Hours,
                      (uint8_t)current_time.Minutes,
                      (uint8_t)current_time.Seconds);


  if(strlen(msg) > 8)
    msg = "len-err";
    
  memset(log.msg, 0, sizeof(char) * 8);
  for(uint i = 0; i < strlen(msg); i++)
    log.msg[i] = *(msg + i);

  write_record((void *)&log);
}
