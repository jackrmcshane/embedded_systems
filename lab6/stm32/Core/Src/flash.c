// #include "flash.h"
#include "main.h"


#define SENTINEL_MARK_BOTTOM 0xDEADBEEFA5A5A5A5
#define SENTINEL_MARK_TOP    0xFEEDC0DE5A5A5A5A
#define FLASH_BEGIN          0x08000000

extern int _edata;
extern int _sdata;
extern int __fini_array_end;
extern flash_status_t fs;

static uint32_t GetPage(uint32_t);
static uint32_t GetBank(uint32_t);


int flash_erase(void) {
  uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
  uint32_t PAGEError = 0;
  /* __IO uint32_t data32 = 0 , MemoryProgramStatus = 0; */

  static FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t program_end = ((uint32_t )&__fini_array_end 
                  + (uint32_t)&_edata
                  - (uint32_t)&_sdata); 
  uint32_t bottom  = (((uint32_t)program_end & ~0x7FF) + 0x800);
  uint32_t top = FLASH_END;
  
  HAL_FLASH_Unlock();
  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 
  /* Get the 1st page to erase */
  FirstPage = GetPage(bottom);
  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(top) - FirstPage + 1;
  /* Get the bank */
  BankNumber = GetBank(bottom);
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;

  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
    HAL_FLASH_Lock();
    return -1;
  }
  HAL_FLASH_Lock();
  return 0;
} 




int write_record(void * record){

  raw_t * write_data;
  HAL_StatusTypeDef status = 0;

  // check for empty record, uninitialized flash_status struct
  if(fs.data_start == 0 || !record) {
    if (fs.data_start == 0) {
      printf("Flash Status Structure not initialized (null)\r\n");
    }
    if (!record) {
      printf("Record Structure not initialized (null)\r\n");
    }
    return -1;
  }

  // check for full mem
  if( fs.next_address <= fs.data_end ){
    printf("flash full, exiting...\n\r");
    return -1;
  }


  write_data = (raw_t *)record;

  HAL_FLASH_Unlock();
  // writing first half of record
  if((status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (int)fs.next_address++, write_data->data0))){
    printf("Error Writing First Chunk\r\n");
    HAL_FLASH_Lock();
    return -1;
  }

  // writing second half on success
  if((status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (int)fs.next_address, write_data->data1))){
    printf("Error Writing Second Chunk\r\n");
    HAL_FLASH_Lock();
    return -1;
  }

  HAL_FLASH_Lock(); 
  fs.next_address-=3;
  fs.next_record_number++;
  fs.total_records++;

  return 0;
}


int flash_write_init() {

  sensordata_t *sd = 0;
  uint16_t record_counter = 0;

  uint64_t *program_end = (uint64_t *)( (uint32_t)&__fini_array_end + (int32_t)&_edata - (uint32_t)&_sdata );
  uint64_t * top = (uint64_t *)(FLASH_END - 0xF); // FLASH_END = 0x0801FFF0
  uint64_t * bottom = (uint64_t *)(((uint32_t)program_end & ~0x7FF) + 0x800);



  fs.data_start = top;
  fs.data_end = bottom;
  fs.max_possible_records = (((uint32_t)top - (uint32_t)bottom)>>4) - 1;
  sd = (sensordata_t *)top;

  while(sd->watermark != 0xFF){ // while record is not empty
    record_counter++;
    sd--;
  }

  fs.next_record_number = record_counter;
  fs.total_records = record_counter;
  fs.next_address = (uint64_t *)sd;

  return 0;
}


static uint32_t GetBank(uint32_t Addr) {
  return FLASH_BANK_1;
}


static uint32_t GetPage(uint32_t Addr) {
  uint32_t page = 0;
  
  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }
  
  return page;
}
