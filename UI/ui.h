#include "mbed.h"
#include "TextLCD.h"
extern TextLCD lcd;



extern int temp_unit;

extern float outside_humidity;
extern float outside_temp;
extern float outside_dewpoint;

extern float inside_humidity;
extern float inside_temp;
extern float inside_dewpoint;

extern float soil1_humidity;
extern float soil2_humidity;

extern unsigned int peltierout;
extern unsigned int peltierin;
extern float peltierin_temp;
extern float peltierout_temp;

extern unsigned int red_light_value;
extern unsigned int blue_light_value;
extern unsigned int green_light_value;

extern float weight_sensor;

extern DigitalOut lcd_backlight;

void ui_startup();
void ui_screen1() ;
void ui_screen2() ;
void ui_screen3() ;
void ui_screen4() ;
//void ui_settings();
void lcd_backlight_on();
void lcd_backlight_off();