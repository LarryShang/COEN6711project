#include "mbed.h"
#include "weight.h"
static const int MAX_DATAPOINTS=500;
static const float MAX_ADC_VALUE=65535;

unsigned short current_weight_1=0;
unsigned int current_weight_1_int=0;
unsigned short CMA_weight1=0;
unsigned int dead_load=32500;
float weight1=0;
void read_weight(){
    int count =0; 
    while(count<MAX_DATAPOINTS){
        
        current_weight_1 = weight_sense1.read_u16();
        
        CMA_weight1 =  (current_weight_1 + (count*CMA_weight1))/(count+1);
        wait_ms(5);
        count++;    
    }
CMA_weight1=CMA_weight1-dead_load;
}

void calc_weight(){
    weight1 = (float)CMA_weight1/MAX_ADC_VALUE;    
    //printf("%d -> weightsensor=%4.4f\n\r",CMA_weight1,weight1);
}