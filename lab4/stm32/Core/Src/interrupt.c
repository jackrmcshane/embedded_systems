#include "main.h"
#include "interrupt.h"



uint32_t disable(void){

    int32_t priority_mask = 0;
    priority_mask = __get_PRIMASK();
    __disable_irq(); 
    return priority_mask;
}


void restore(uint32_t priority_mask){
    __set_PRIMASK(priority_mask);
}



void enable(void){
    __enable_irq();
}
