#include "main.h"

extern ADC_HandleTypeDef hadc1;

uint32_t read_vrefint(){

  uint32_t rawVintref;

  static uint32_t first_time_up = 1;
  static ADC_ChannelConfTypeDef sConfig = {0};

  if (first_time_up) {
    sConfig.Channel = ADC_CHANNEL_VREFINT;
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
  rawVintref = HAL_ADC_GetValue(&hadc1);

  HAL_ADC_Stop(&hadc1);


  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      Error_Handler();

  return __HAL_ADC_CALC_VREFANALOG_VOLTAGE(rawVintref, ADC_RESOLUTION_12B);
}
