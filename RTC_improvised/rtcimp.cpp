#include "mbed.h"


int sec=0;
int min=0;
int hour=0;
void sec_inc() {
    
    if(sec<59){
        sec++;
    }
    else{
        sec=0;
        min++;
        if(min>59){
            min=0;
            hour++;
            if(hour>=24){
                hour=0;
            }
        }
    }
}

void rtcimp_settime(int hour_in ,int min_in){
    hour = hour_in;
    min = min_in;
};