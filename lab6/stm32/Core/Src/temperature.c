#include "main.h"

extern ADC_HandleTypeDef hadc1;

uint32_t read_temp( void ){

  uint32_t vref;
  uint32_t rawTemp;

  static uint32_t first_time_up = 1;
  static ADC_ChannelConfTypeDef sConfig = {0};
  
    
  // configure adc to read temp sensor
  if (first_time_up) {
    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    first_time_up = 0;
  }

  sConfig.Rank = ADC_REGULAR_RANK_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    Error_Handler();


  while (HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED) != HAL_OK);   // Calibrate the A2D

  HAL_ADC_Start(&hadc1); 

  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  rawTemp = HAL_ADC_GetValue(&hadc1);

  HAL_ADC_Stop(&hadc1);

  vref = read_vrefint();

  return __HAL_ADC_CALC_TEMPERATURE(vref, rawTemp, ADC_RESOLUTION_12B);
}
