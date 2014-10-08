
//QUICK REFS:
// TICKER is using timer 1 for buzzer. 

#include "mbed.h"

#include "keypad.h"
#include "buzz.h"

#include "pindefs.h" //led pin definitions
#include "led_lights.h"

#include "peltier.h"
#include "peltier_pindefs.h"

#include "pumps.h"
#include "pumps_pindefs.h"

#include "airhumidity.h"
#include "soilmoisture.h"
#include "soilmoisture_pindefs.h"

#include "ui.h"
#include "ui_pindefs.h"
#include "TextLCD.h"

#include "rtcimp.h"

#include "weight.h"
#include "weight_pindefs.h"

#include "cycleair.h"
#include "cycleair_pindef.h"

#include "DS18B20.h"
#include "OneWireDefs.h"

#include <string>
using namespace std;

#define     BAUDRATE0       1200
#define     BAUDRATE1       2400
#define     BAUDRATE3       2400

//SERIAL
Serial bluetooth_dev(PTA2, PTA1);  // tx, rx
Serial printer(PTC4,PTC3);  // tx, rx
//TICKERS
Ticker timer1ms;
Ticker clock_mine;
Timer lcd_timeout;
//FLAGS
bool startup_flag=true; 
bool watered_plant1 = false;
int watered_plant1_time = 0; 
bool watered_plant2 = false;
int watered_plant2_time = 0; 

//SETPOINTS
float setpoint_air_humidity = 30.00 ;
float setpoint_soil_humid1 = 0.3 ;
float setpoint_soil_humid2 = 0.3 ;

float setpoint_MAX_air_temp = 45;

int setpoint_red_lights;
int setpoint_blue_lights;
int setpoint_green_lights;

//WINDOWING
float window_f = 5 ;
int window_i =2 ;

//GLOBAL VARS
float current_water_level = 0; 
bool lcd_timeout_flag=false;

int led_set_time=0;
int led_off_time=0;
bool lights_startup_flag=true;

int sensor_function_call_time=0;

int pelt_call_time=0;

float desire_temp=40;
float desire_temp_air=30;
bool pelt_keypad_enable=false;
bool pelt_first_time = true;
bool pelt_overheat = false;
//SAFETY LIMITS
const float max_peltier_temp = 50 ; //CELCIUS
const int MAX_WATERING_TIME_GAP = 20; //Number of Hours
const int CYCLE_AIR_TIME = 2; //Wait between on and off seconds
const int LCD_TIMEOUT_VALUE = 30; //Number of seconds
const char status = 'F'; //For bluetooth Serial
const int led_lights_timeout=5; // Timeout afterwhich to turn off/on lights hours
const int sensor_function_call_timeout=3; //Timeout for sensorcall and function
const int pelt_call_timeout=30;

//*************************FUNCTION CALLBACK FOR SERIAL INTTERUPTS********************************
void callback_bluetooth() {
    // Note: you need to actually read from the serial to clear the RX interrupt
    if(bluetooth_dev.getc()=='S'){
        
        printf("Info %c %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f \n\r", status,outside_temp,inside_temp,inside_humidity,outside_humidity,inside_dewpoint,outside_dewpoint,soil1_humid,soil2_humid) ; 
        
        //myled = !myled;
        }
    //printf("%c\n\r", device.getc());
    
}

void printer_print_values(){
    
    //printf("\r****Autus Sensor Values****\r");
    wait(1);
    printer.printf("Chamber Temp: %4.2f\r",inside_temp);
    wait(1);
    printer.printf("Chamber Humidity: %4.2f\r",inside_humidity);
    //wait(1);
    //printer.printf("Chamber Dewpoint: %4.2f\r",inside_dewpoint);
    wait(1);
    printer.printf("External Temp: %4.2f\r",outside_temp);
    wait(1);
    printer.printf("External Humidity: %4.2f\r",outside_humidity);
    //wait(1);
    //printer.printf("External Dewpoint: %4.2f\r",outside_dewpoint);
    wait(1);
    printer.printf("Soil Moisture-1: %4.2f\r",soil1_humid);
    wait(1);
    printer.printf("Soil Moisture-2: %4.2f\r",soil2_humid);
    wait(1);
    printer.printf("Load Cell: %4.2f\r",weight_sensor);
    wait(1);
    printer.printf("**********END**********\r");
}

//**************************FUNCTION TO READ SENSORS ********************************************** 
void read_sensors(){
    
    //Read values for AIR
    
    //OUTSIDE
    outside_humidity = get_air_humid_outside();
    outside_temp = get_air_temp_outside(temp_unit); 
    outside_dewpoint = get_air_dewpoint_outside();
    
    //INSIDE
    inside_humidity = get_air_humid_inside();
    inside_temp = get_air_temp_inside(temp_unit); 
    inside_dewpoint = get_air_dewpoint_inside();
    
    //Read value for soil
    read_soil_humidity();
    calc_soil_humid_values(); //values in soil1_humid and soil2_humid perecentages
    soil1_humidity = soil1_humid ;
    soil2_humidity = soil2_humid ;
    //printf("%4.2f \n\r",soil1_humidity);
    //READ LOAD SENSOR
    read_weight();
    calc_weight();
    weight_sensor = weight1; 
}

//****************************Functions that perform tasks**************************************
//CYCLE AIR 
void cycleair(){
    
    //Turn on fan 
    cycle_air_on();
    wait(CYCLE_AIR_TIME);
    //turn of fan 
    cycle_air_off();
    
}
//Water Plants
void waterplants(){
    
    
    if(startup_flag == true){
        //call for pump function
        
        watered_plant1 = true;
        watered_plant1_time = hour ; 
        watered_plant2 = true;
        watered_plant2_time = hour ; 
        pumps(true,true,true,false); 
        wait(5.0);
        pumps(false,false,false,false);
    }
    else{
        
        if( (soil1_humid < setpoint_soil_humid1) || (watered_plant1 == false) ){ 
            //enable pump1
            watered_plant1 = true;
            watered_plant1_time = hour ;
            pumps(true,true,true,false); 
            wait(5.0);
            pumps(false,false,false,false);
        }
        if( (soil2_humid < setpoint_soil_humid2) || (watered_plant2 == false) ){ 
            //enable pump2
            watered_plant2 = true;
            watered_plant2_time = hour ;
            pumps(true,true,true,false); 
            wait(5.0);
            pumps(false,false,false,false);
        }
    }
    //Make sure it is watered at least every x hours in case soil humid doesnt work
    if (watered_plant1 == true){
        if(abs(hour-watered_plant1_time)>MAX_WATERING_TIME_GAP){
            watered_plant1 = false;
            cycleair();
        }
    }   
    if (watered_plant2 == true){
        if(abs(hour-watered_plant2_time)>MAX_WATERING_TIME_GAP){
            watered_plant2 = false;
            cycleair();
        }
    } 
    
}

// Air Humidity Level
void airhumidity(){
    
    if( !( (setpoint_air_humidity+window_f > inside_humidity) && (setpoint_air_humidity-window_f < inside_humidity) ) ){
    
        //Turn on Air humidity fan 
        pumps(false,false,false,true);
        wait(5);
        //Turn off Air humisity fan
        pumps(false,false,false,false);
    }   
}


//**********************************LCD TIMEOUT FUNCTION*********************************
void lcd_timeout_check(){
    
    if(lcd_timeout.read()>LCD_TIMEOUT_VALUE){
            lcd_timeout_flag = true;
        }
    
}
void settings_submenu(int counter_menu){
    
    char key_value2;
    bool break_flag = false;
    switch(counter_menu){
        case 0:
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("Not Authorised");
            wait(5);
            break;
        case 1:
            break_flag = false;
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("Cur Air Humidity Lvl");
            //lcd.locate(0,1);
            //lcd.printf("%3.1f",setpoint_air_humidity);
            while(!break_flag){
                lcd.locate(0,1);
                lcd.printf("%3.1f",setpoint_air_humidity);
                key_value2 = Keypad();
                wait(0.2);
                if(key_value2 == 74){setpoint_air_humidity++;}
                else if(key_value2 == 71){setpoint_air_humidity--;}
                else if(key_value2 == 75){break_flag=true;} //enter
                if (setpoint_air_humidity <0 || setpoint_air_humidity >99) {setpoint_air_humidity=30.00;}
            }
            
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("NEW Air Humid Setpoint");
            lcd.locate(0,1);
            lcd.printf("%3.1f",setpoint_air_humidity);
            wait(5);
            
            break;
        case 2:
            break_flag = false;
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("Soil Humidity Setpoint");
            
            while(!break_flag){
                lcd.locate(0,1);
                lcd.printf("S1: %4.2f",setpoint_soil_humid1);
                lcd.printf(" S2: %4.2f",setpoint_soil_humid2);
                key_value2 = Keypad();
                wait(0.2);
                if(key_value2 == 62){ //up
                    setpoint_soil_humid1++;
                    setpoint_soil_humid2++;
                }
                else if(key_value2 == 59){//down
                    setpoint_soil_humid1--;
                    setpoint_soil_humid2--;
                }
                else if(key_value2 == 75){break_flag=true;}//enterkey
                //if (setpoint_air_humidity <0 || setpoint_air_humidity >99) {setpoint_air_humidity=30.00;}
            }
            
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("New Soil Setpoints:");
            lcd.locate(0,1);
            lcd.printf("S1: %4.2f",setpoint_soil_humid1);
            lcd.printf(" S2: %4.2f",setpoint_soil_humid2);
            wait(5);
            break;
        case 3:
        
            break_flag = false;
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("Cur Air Tmp Setpoint");
            while(!break_flag){
                lcd.locate(0,1);
                lcd.printf("%3.1f",setpoint_MAX_air_temp);
                key_value2 = Keypad();
                wait(0.2);
                if(key_value2 == 74){setpoint_MAX_air_temp++;}
                else if(key_value2 == 71){setpoint_MAX_air_temp--;}
                else if(key_value2 == 75){break_flag=true;}
                if (setpoint_MAX_air_temp <0 || setpoint_MAX_air_temp >99) {setpoint_MAX_air_temp=45.0;}
            }
            
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("NEW Air Tmp Setpoint");
            lcd.locate(0,1);
            lcd.printf("%3.1f",setpoint_MAX_air_temp);
            wait(5);
            break;
    }//switch end
    
}
//***************************************SETTINGS UI************************************* 
void ui_settings(){
    
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Autus Settings");
    lcd.locate(0,1);
    lcd.printf("Performing Checks");
    char key_value;
    bool exit_settings=false;
    
    //disable peltier
    peltier(false,false,false,false,0);
    
    wait(2);
    string menu[5];
    menu[0]= "1.LED Brightness";
    menu[1]= "2.Air Humidty Lvl";
    menu[2]= "3.Soil Moisture Lvl";
    menu[3]= "4.Air Temperature";
    menu[4]= " ";
    lcd.cls();
    int counter_menu =0 ; 
    bool overflow_menu = false;
    while(!exit_settings){
        
        key_value = Keypad();
        printf("Key %d\n\r",key_value);
        wait(0.1);
        if(key_value == 150){
            lcd.locate(0,0);
            lcd.printf(">%s",menu[counter_menu].c_str());
            lcd.locate(0,1);
            if(counter_menu ==4){
                counter_menu = 0;
                overflow_menu=true;
                lcd.printf(" %s",menu[counter_menu].c_str());
            }
            else{
                lcd.printf(" %s",menu[counter_menu+1].c_str());
            }
            //wait(0.2); 
        }//No Press If Block
        else if(key_value == 68){//Down Key Pressed if block
            lcd.cls();
            if(overflow_menu==false){
                counter_menu++;
                //printf("CounterOverflow)");
            }
            else{overflow_menu=false;}
            
        }//Down Key Pressed if block end
        else if(key_value == 65){ //OK Key Pressed if block
            settings_submenu(counter_menu);
            lcd.cls();
        } //OK button if block
        else if(key_value == 75){ //ENTR Button
            exit_settings = true;
            lcd.cls();
            lcd.printf("SAVE Config");
            wait(2);
        } 

    }//While settings loop exit 
    lcd.cls();
    lcd.printf("Exit Settings");
    wait(5);
}


//Function to process keypad inputs
void process_user_input(char keypad_value_in, int ui_current_screen_in){
    
    if(keypad_value_in == 61){
            if(lcd_timeout_flag == false){
                switch (ui_current_screen_in){
                    case 1:
                        ui_screen2();
                        ui_current_screen_in =2;
                        break;
                    case 2:
                        ui_screen3();
                        ui_current_screen_in =3;
                        break;
                    case 3:
                        ui_screen4();
                        ui_current_screen_in =4;
                        break;    
                }//switch block current ui screen
                lcd_timeout.reset();
                lcd_timeout.start();
                wait(2);
            }// if block lcdflag
            else{
                ui_screen1();
                wait(5);    
            }
        }// if block 36 end 
        
        // check for setting screen press enter
        if(keypad_value_in == 58){
                //Disable peltier 
                ui_settings();
                wait(5);
        }
        
        if(keypad_value_in ==100){
            ui_screen1();
            wait(0.2);  
        }
}




//***************************PLACEHOLDER FUNCTION TO DISABLE EVERYTHING **********************
void disable_everything(){
    
    //Disable peltier,vac,fans,pumps,lights.
    pumps(false,false,false,false);
    peltier(false,false,false,false,0);
    cycle_air_off();
}


//*********************************************************************************************
//*                                                                                           *
//*                                                                                           *
//*                                                                                           *
//*                                                                                           *
//*                                    MAIN ROUTINE                                           *
//*                                                                                           *
//*                                                                                           *
//*                                                                                           *
//*********************************************************************************************

int main(void)
{
    
    //call function that disables everything.
    disable_everything();
    
    
    lcd_backlight_on();   
    //FOR INTERNAL CLOCK
    clock_mine.attach(&sec_inc, 1.0);

    char keypad_value;
    int ui_current_screen = 1;
    //************************************BOOT SEQUENCE*********************
    //call LCD boot
    ui_startup();
    wait(2);
    //SET TIME
    rtcimp_settime(6,0);
    //call function that reads values
    
/*  FOR CRTITICALITY
    NVIC_DisableIRQ(TIMER3_IRQn);
 
    // critical section
 
    NVIC_EnableIRQ(TIMER3_IRQn);
*/  wait(5);
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Find Pelt Temp Sense");
    
    tempinit();
   //peltierin_temp = dsen_temp_in;
    //peltierout_temp = dsen_temp_out;
    
    wait(2);
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Find Pelt Temp Sense");
    lcd.printf("Found Them");
    
    wait(2);
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Reading Sensors");
    
    read_sensors();
    
    wait(2);
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Done Reading Sensors");
    //call function that performs functions.
    
    wait(2);
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Watering Plants");
    wait(1);
    
    waterplants();
    
    wait(2);
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Air Humidity");
    
    airhumidity();
    
    wait(2);
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Cycling Air");
    
    cycleair();
    
    wait(2);
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Testing Lights");
    led_test();
    
    
    //INTILIZE SERIAL PARAMETERS
    wait(2);
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Initialize Bluetooth");
    bluetooth_dev.baud(9600);
    bluetooth_dev.attach(&callback_bluetooth);
    
    wait(2);
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Initialize Printer");
    printer.baud(BAUDRATE0);
    printer.format(8,Serial::None,1);
    printer.printf("Autus Ready\n\r");
    //printer_print_values();
    wait(5);
    lcd.cls();
    ui_current_screen = 1;
    ui_screen1();
    lcd_timeout.reset();
    wait(5);
    
    //startup_flag = false ;
    timer1ms.attach(&timer1, 0.001); //interrupt attached function(timer) with interval (1 ms)  
    led_blue_pulsewidth=50;
    led_blue.period_us(1000);
    
    while(1) {
       
        
        //FOR BUZZER
        
        keypad_value = Keypad();
        wait(0.1);
        
        //lcd_timeout_check();
        //process_user_input(keypad_value,ui_current_screen);
        
        //Press down arrow to change menu
        if ( (keypad_value==68) && (ui_current_screen==1) ) {
            ui_screen2();
            ui_current_screen=2;
            wait(5);
            lcd.cls();
            //led_blue_pulsewidth=led_blue_pulsewidth+100;
            //led_blue.pulsewidth_us(led_blue_pulsewidth);
            //printf("%c\n\r",keypad_value);
        }
        else if ( (keypad_value==68) && (ui_current_screen==2)) {
            ui_screen3();
            ui_current_screen=3;
            wait(5);
            lcd.cls();
            //printf("%c\n\r",keypad_value);
        }
        else if ( (keypad_value==68) && (ui_current_screen==3)) {
            red_light_value = MAX_RED_LIGHT;
            blue_light_value = MAX_BLUE_LIGHT;
            green_light_value = MAX_GREEN_LIGHT;
            ui_screen4();
            ui_current_screen=1;
            wait(5);
            lcd.cls();
            //printf("%c\n\r",keypad_value);
        }
        else if ( (keypad_value==75) ) { //Press Enter for settings
            ui_settings();
            ui_current_screen=1;
            lcd.cls();
            //wait(5);
            //printf("%c\n\r",keypad_value);
        }
        else if(keypad_value == 73){
            if(MAX_RED_LIGHT<100){
                MAX_RED_LIGHT++;
                pwm_red_led(1);
            }       
        }
        else if(keypad_value == 70){
            if(MAX_RED_LIGHT>0){
                MAX_RED_LIGHT--;
                pwm_red_led(0);
            }       
        }
        else if(keypad_value == 67){
            if(MAX_GREEN_LIGHT<100){
                MAX_GREEN_LIGHT++;
                pwm_green_led(1);
            }       
        }
        else if(keypad_value == 64){
            if(MAX_GREEN_LIGHT>0){
                MAX_GREEN_LIGHT--;
                pwm_green_led(0);
            }       
        }
        else if(keypad_value == 61){
            if(MAX_BLUE_LIGHT<100){
                MAX_BLUE_LIGHT++;
                pwm_blue_led(1);
            }       
        }
        else if(keypad_value == 58){
            if(MAX_BLUE_LIGHT>0){
                MAX_BLUE_LIGHT--;
                pwm_blue_led(0);
            }       
        }
        else {
            ui_screen1();
            wait(0.1);
        }
        
        if (keypad_value == 62 ){ //Backlight red up
            lcd_backlight_on();    
        }
        if (keypad_value == 59 ){ //Backlight red down
            lcd_backlight_off();    
        }
        if(keypad_value == 84){
            pelt_keypad_enable=true;    
        }
        if(keypad_value == 78){
            printer_print_values();
        }
        if(keypad_value ==86){
            pumps(true,false,false,false);
            wait(2);
            pumps(false,false,false,false);
        }
        if(keypad_value ==80){
            pumps(false,true,true,false);
            wait(2);
            pumps(false,false,false,false);
        }
        if(keypad_value ==83){
            pumps(false,false,false,true);
            wait(2);
            pumps(false,false,false,false);
        }
        if(keypad_value ==77){
            cycle_air_on();
            wait(2);
            cycle_air_off();
        }
        //printf("%d\n\r",keypad_value);
        //  -> Key pressed
        
        
        //LEDS Timeout functions
        if(led_set_flag == false){
            if( (abs(hour-led_off_time) > led_lights_timeout)||lights_startup_flag ==true){
               // printf("MINON%d\n\r",::min);
                set_leds();
                led_set_time = hour;
                lights_startup_flag = false;
               // printf("SETLEDS\n\r");
            }
            
        }
        if(led_set_flag ==true){
            if(abs(hour-led_set_time) > led_lights_timeout){
             //   printf("MINOFF %d\n\r",::min);
                turn_off_leds();
                led_off_time = hour;
                led_set_flag =false;
              //  printf("OFF\n\r");
            }
        }
        //printf("MINOFF %d\n\r",::min);
        if( (abs((::min)-sensor_function_call_time) > sensor_function_call_timeout) || startup_flag==true){
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("Updating Values");
            lcd.locate(0,1);
            lcd.printf("Performing func");
            read_sensors();
            airhumidity();
            peltierin_temp = dsen_temp_in;
            peltierout_temp = dsen_temp_out;
            sensor_function_call_time=::min;
            printer_print_values();
            lcd.cls();
        }
        /**********************PELTIER CALLER************************************************************************/
        //pelt_keypad_enable = true;
        if (pelt_keypad_enable == true){
            if ( ( abs( (::min)-pelt_call_time ) > pelt_call_timeout ) || pelt_first_time == true) {
                
                lcd.cls();
                lcd.locate(0,0);
                lcd.printf("Adjusting Temperature");
                lcd.locate(0,1);
                lcd.printf("Pelt Functions");
                wait(2);
                //lcd.cls();
                //wait(5);
                
                lcd.cls();
                lcd.locate(0,0);
                lcd.printf("Find Pelt Temp Sense");
                
                //tempinit();
                readTemp(1);
                dsen_temp_in=temp;
                readTemp(0);
                dsen_temp_out=temp;
                printf("Pi%2.1f Po%2.1f\n\r",dsen_temp_in,dsen_temp_out);
                peltierin_temp = dsen_temp_in;
                peltierout_temp = dsen_temp_out;
                
                wait(5);
                lcd.cls();
                lcd.locate(0,0);
                lcd.printf("Find Pelt Temp Sense");
                lcd.printf("Found Them");
                wait(2);
                lcd.cls();
                pelt_first_time = false ;
                 
                float pelt_inside_temp =0;
                bool exit_pelt = false;
                float dsen_temp_out_prev=0;
                pelt_inside_temp = get_air_temp_inside(0);
                bool read_temp1_valid=false;
                bool read_temp2_valid=false;
                
                while(!read_temp2_valid){
                    readTemp(0);
                    wait(0.1);
                    if(temp>0){
                        read_temp2_valid = true;
                    }
                }
                //readTemp(0);
                dsen_temp_out_prev=temp;
                while (exit_pelt == false){
                    
                    pelt_inside_temp = get_air_temp_inside(0);
                    
                    if (dsen_temp_in<desire_temp) {
                        read_temp1_valid=false;
                        read_temp2_valid=false;
                        peltier(true,true,false,true,90);
                        
                        //wait(0.1);
                        while(!read_temp1_valid){
                            readTemp(1);
                            wait(0.1);
                            if(temp>0){
                                read_temp1_valid = true;
                            }
                        }
                        dsen_temp_in=temp;
                        printf("Inside Temp:%2.1f\n\r",dsen_temp_in);
                        /*while(!read_temp2_valid){
                            readTemp(0);
                            wait(0.1);
                            if(temp>0){
                                read_temp2_valid = true;
                            }
                        }*/
                        readTemp(0);
                        dsen_temp_out=temp;
                        printf("Outside Temp:%2.1f\n\r",dsen_temp_out);
                    }
                    if (dsen_temp_in>=desire_temp) {
                        
                        read_temp1_valid=false;
                        read_temp2_valid=false;
                        peltier(true,true,false,true,0);
                        //wait(0.1);
                        while(!read_temp1_valid){
                            readTemp(1);
                            wait(0.1);
                            if(temp>0){
                                read_temp1_valid = true;
                            }
                        }
                        dsen_temp_in=temp;
                        printf("Inside Temp:%2.1f\n\r",dsen_temp_in);
                        /*while(!read_temp2_valid){
                            readTemp(0);
                            wait(0.1);
                            if(temp>0){
                                read_temp2_valid = true;
                            }
                        }*/
                        readTemp(0);
                        dsen_temp_out=temp;
                        printf("Outside Temp:%2.1f\n\r",dsen_temp_out);
                    }
                    if (dsen_temp_in>=max_peltier_temp){
                        
                        peltier(false,false,false,false,0);
                        lcd.locate(0,0);
                        lcd.printf("Pelt TEMP LIMIT");
                        lcd.locate(0,1);
                        lcd.printf("Pin:%2.1 Pout:%2.1f",dsen_temp_in,dsen_temp_out);
                        exit_pelt = true;
                        pelt_overheat = true;
                    }
                    else{
                        lcd.cls();
                        lcd.locate(0,0);
                        lcd.printf("Air Temp %4.2f",pelt_inside_temp);
                        lcd.locate(0,1);
                        if(!(dsen_temp_out>0)){
                            dsen_temp_out = dsen_temp_out_prev ;
                        }
                        else{
                            dsen_temp_out_prev = dsen_temp_out ;
                        }
                        lcd.printf("Pin:%2.1f Pout:%2.1f",dsen_temp_in,dsen_temp_out);
                        printf("Pi%2.1f Po%2.1f\n\r",dsen_temp_in,dsen_temp_out);
                        wait(15);
                        if ( (pelt_inside_temp-5<=desire_temp_air) && (pelt_inside_temp+5>=desire_temp_air) ){
                            lcd.cls();
                            lcd.locate(0,0);
                            lcd.printf("Exiting Peltier");
                            lcd.locate(0,1);
                            lcd.printf("Temp within Range");
                            exit_pelt = true; 
                            wait(5);
                        }
                    }
                }//pelt while end 
                peltier(false,false,false,false,0);
                lcd.cls();
                lcd.locate(0,0);
                lcd.printf("Pelt Off");
                wait(5);
                lcd.cls();
                //lcd.locate(0,1);
                //lcd.printf("Temp within Range");
                
            }//if block time check end 
            
            pelt_call_time = ::min ; 
            //pelt_keypad_enable = false;
        }//Pelt IF keypad block end
        
        
         //printf("MIN: %d\n\r",::min);
        startup_flag = false ;
    }//WHILE END

        
}


