#include "mbed.h"
#include "led_lights.h"
unsigned int led_red_pulsewidth;
unsigned int led_green_pulsewidth;
unsigned int led_blue_pulsewidth;
unsigned int led_period =1000;
unsigned int led_pulsewidth = 0;
unsigned int led_step = 10;

int MAX_RED_LIGHT = 100;
int MAX_BLUE_LIGHT = 100;
int MAX_GREEN_LIGHT = 100;

bool led_set_flag= false;

unsigned int pwm_blue_led(int brightness )
{
    if (brightness==1) {
        led_blue_pulsewidth=led_blue_pulsewidth+led_step;
        led_blue.pulsewidth_us(led_blue_pulsewidth);
    };
    if (brightness==0) {
        led_blue_pulsewidth=led_blue_pulsewidth-led_step;
        led_blue.pulsewidth_us(led_blue_pulsewidth);
    };
    return led_blue_pulsewidth;
}
unsigned int pwm_green_led(int brightness)
{
    if (brightness==1) {
        led_green_pulsewidth=led_green_pulsewidth+led_step;
        led_green.pulsewidth_us(led_green_pulsewidth);
    }
    if (brightness==0) {
        led_green_pulsewidth=led_green_pulsewidth-led_step;
        led_green.pulsewidth_us(led_green_pulsewidth);
    }
    return led_green_pulsewidth;
}
unsigned int pwm_red_led(int brightness)
{
    if (brightness==1) {
        led_red_pulsewidth=led_red_pulsewidth+led_step;
        led_red.pulsewidth_us(led_red_pulsewidth);
    }
    if (brightness==0) {
        led_red_pulsewidth=led_red_pulsewidth-led_step;
        led_red.pulsewidth_us(led_red_pulsewidth);
    }
    return led_red_pulsewidth;
}

void led_test(void){
    led_blue.period_us(led_period);
    led_blue_pulsewidth=led_pulsewidth;
    for (int i=1; i<=MAX_BLUE_LIGHT; i++) {
        pwm_blue_led(1);
        wait (0.1);
    }
    for (int i=1; i<=MAX_BLUE_LIGHT; i++) {
        pwm_blue_led(0);
        wait (0.1);
    }
    led_blue_pulsewidth=10;    // set to 0
    pwm_blue_led(0);           // set to 0

    led_red.period_us(led_period);
    led_red_pulsewidth=led_pulsewidth;
    for (int i=1; i<=MAX_RED_LIGHT; i++) {
        pwm_red_led(1);
        wait (0.1);
    }
    for (int i=1; i<=MAX_RED_LIGHT; i++) {
        pwm_red_led(0);
        wait (0.1);
    }
    led_red_pulsewidth=10;    // set to 0
    pwm_red_led(0);           // set to 0
    
    led_green.period_us(led_period);
    led_green_pulsewidth=led_pulsewidth;
    for (int i=1; i<=MAX_GREEN_LIGHT; i++) {
        pwm_green_led(1);
        wait (0.1);
    }
    for (int i=1; i<=MAX_GREEN_LIGHT; i++) {
        pwm_green_led(0);
        wait (0.1);
    }
    led_green_pulsewidth=10;    // set to 0
    pwm_green_led(0);           // set to 0
}

void set_leds(){
    
    //BLUE
    led_blue.period_us(led_period);
    led_blue_pulsewidth=led_pulsewidth;
    for (int i=1; i<=MAX_BLUE_LIGHT; i++) {
        pwm_blue_led(1);
        wait (0.1);
    }
    //RED
    led_red.period_us(led_period);
    led_red_pulsewidth=led_pulsewidth;
    for (int i=1; i<=MAX_RED_LIGHT; i++) {
        pwm_red_led(1);
        wait (0.1);
    }
    //GREEN
    led_green.period_us(led_period);
    led_green_pulsewidth=led_pulsewidth;
    for (int i=1; i<=MAX_GREEN_LIGHT; i++) {
        pwm_green_led(1);
        wait (0.1);
    }
    led_set_flag = true;
    
}
void turn_off_leds(){
        
    led_red_pulsewidth=10;    // set to 0
    pwm_red_led(0);        
    led_blue_pulsewidth=10;    // set to 0
    pwm_blue_led(0);           // set to 0
    
    led_green_pulsewidth=10;    // set to 0
    pwm_green_led(0);  
}