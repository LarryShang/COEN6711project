#include "mbed.h"
#include "soilmoisture.h"
/*https://mbed.org/users/chris/notebook/Getting-best-ADC-performance*/
static const int MAX_DATAPOINTS=500;
static const float MAX_ADC_VALUE=65535;

unsigned short current_soil_humid1=0;
unsigned short current_soil_humid2=0;

unsigned int current_soil_humid1_int=0;
unsigned int current_soil_humid2_int=0;

unsigned short CMA_soil1=0;
unsigned short CMA_soil2=0; 

float soil1_humid=0;
float soil2_humid=0;

void read_soil_humidity(){
    
    int count =0; 
    while(count<MAX_DATAPOINTS){
        
        current_soil_humid1 = soil_sense1.read_u16();
        current_soil_humid2 = soil_sense2.read_u16();
        
        //convert from short to int
        //current_soil_humid1_int = (unsigned int)current_soil_humid1;
        //current_soil_humid2_int = (unsigned int)current_soil_humid2;
        
        CMA_soil1 =  (current_soil_humid1 + (count*CMA_soil1))/(count+1);
        CMA_soil2 =  (current_soil_humid2 + (count*CMA_soil2))/(count+1); 
        
        wait_ms(5);
        count++;    
    }

}

void calc_soil_humid_values(){
    printf("%d \n\r",CMA_soil1);
    soil1_humid = (float)CMA_soil1/MAX_ADC_VALUE;    
    soil2_humid = (float)CMA_soil2/MAX_ADC_VALUE;
    printf("%4.2f \n\r",soil1_humid);
}