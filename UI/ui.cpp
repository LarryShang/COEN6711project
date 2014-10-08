#include "mbed.h"
//#include "keypad.h"
//#include "globalvalues.h"
#include "ui.h"
#include "airhumidity.h" //for error variables

char lcd_keypad_value ; 
int temp_unit = 0; //0 for celcius 1 for faren 2 for kelvin 
char temp_unit_char = 'C'; 

float outside_humidity;
float outside_temp;
float outside_dewpoint;

float inside_humidity;
float inside_temp;
float inside_dewpoint;

float soil1_humidity;
float soil2_humidity;

float weight_sensor;

unsigned int peltierout;
unsigned int peltierin;
float peltierin_temp;
float peltierout_temp;

unsigned int red_light_value;
unsigned int blue_light_value;
unsigned int green_light_value;
//Set Temperature Unit
void set_temp_unit(){
    if(temp_unit == 1){
        temp_unit_char = 'F';
    }
    else if(temp_unit == 2){
        temp_unit_char = 'K';
    }
    else{
        temp_unit_char = 'C';
    }
}

void ui_startup(){

    lcd.cls();
    lcd.setCursor(TextLCD::CurOff_BlkOn);
    lcd.locate(0,0);
    lcd.printf("Project Autus");
    lcd.locate(0,1);
    lcd.printf("Initializing Machine");
}

void ui_screen1() {
    
    set_temp_unit();
    //lcd.cls();
    lcd.setCursor(TextLCD::CurOff_BlkOn);
    lcd.locate(0,0);
    lcd.printf("T: %3.1f%c",inside_temp,temp_unit_char);
    lcd.printf(" H: %3.1f",inside_humidity);   
    lcd.locate(0,1);
    lcd.printf("S1: %4.2f",soil1_humidity);
    lcd.printf(" S2: %4.2f",soil2_humidity);
}

void ui_screen2() {
    
    set_temp_unit();
    lcd.cls();
    lcd.setCursor(TextLCD::CurOff_BlkOn);
    lcd.locate(0,0);
    lcd.printf("iT: %3.1f%c",inside_temp,temp_unit_char);
    lcd.printf(" iH: %3.1f",inside_humidity);   
    lcd.locate(0,1);
    lcd.printf("oT: %3.1f%c",outside_temp,temp_unit_char);
    lcd.printf(" oH: %3.1f",outside_humidity);
}

void ui_screen3() {
    
    //set_temp_unit();
    lcd.cls();
    lcd.setCursor(TextLCD::CurOff_BlkOn);
    lcd.locate(0,0);
    lcd.printf("PlateOut %u T%3.1f",peltierout,peltierout_temp);
    //lcd.printf(" iH: %3.1f",inside_humidity);   
    lcd.locate(0,1);
    lcd.printf("PlateIn %u T%3.1f",peltierin,peltierin_temp);
}
void ui_screen4() {
    
    //set_temp_unit();
    lcd.cls();
    lcd.setCursor(TextLCD::CurOff_BlkOn);
    lcd.locate(0,0);
    lcd.printf("Red:%u",red_light_value);
    lcd.printf(" Blue:%u",blue_light_value);   
    lcd.locate(0,1);
    lcd.printf("G::%u",green_light_value);
    lcd.printf("H20: %3.1f",weight_sensor);
    //lcd.printf(" oH: %3.1f",outside_humidity);
}

void lcd_backlight_on(){
    
    lcd_backlight = 1;    
}
void lcd_backlight_off(){
    
    lcd_backlight = 0;    
}