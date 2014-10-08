#include "mbed.h"
                                //set timer 1ms
int beep_delay;
DigitalOut myled(PTE5);
void timer1(void)        //interrupt every 1ms
{
    beep_delay=beep_delay-1;
}
void beep(float freq,int time)
{

    beep_delay=time;

    do {
        myled = 1;
        wait(freq);
        myled = 0;
        wait(freq);
    } while(beep_delay!=0);
}