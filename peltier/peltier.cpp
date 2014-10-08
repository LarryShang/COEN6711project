#include "mbed.h"

#include "TextLCD.h"
//#include "DS18B20.h"
#include "peltier.h"
//----------------------------------
#include "DS18B20.h"
#include "OneWireDefs.h"

#define THERMOMETER DS18B20

float temp;
float dsen_temp_in;
float dsen_temp_out;
// device( mbed pin )
THERMOMETER device(PTE0);


//-----------------------------------
//Serial pc(USBTX, USBRX); // tx, rx

unsigned int peltier_hot_pulsewidth=1000;
unsigned int peltier_cold_pulsewidth=1000;
unsigned int peltier_period =1000;
unsigned int peltier_pulsewidth = 0;
unsigned int peltier_step = 10;

bool alarm_hi_temp_in=false;
bool alarm_lo_temp_in=false;
bool alarm_hi_temp_out=false;
bool alarm_lo_temp_out=false;

//TextLCD lcd(PTE29, PTE30, PTC12, PTD0, PTD5, PTA13, TextLCD::LCD20x2);   // rs, e, d4-d7 ok
//DigitalOut backlight(PTD6);
//unsigned int peltier_hot_pulsewidth=1000;
//unsigned int peltier_cold_pulsewidth=1000;
//unsigned int peltier_period =1000;
//unsigned int peltier_pulsewidth = 0;
//unsigned int peltier_step = 10;
//bool alarm_hi_temp_in=false;
//bool alarm_lo_temp_in=false;
//bool alarm_hi_temp_out=false;
//bool alarm_lo_temp_out=false;
float dsen_temp;

void readTemp(int deviceNum)
{
    temp = device.readTemperature(deviceNum);
    printf("Device %d is %f\n\r",deviceNum, temp);
    wait(0.5);
}
void tempinit(void)
{
    while (!device.initialize());    // keep calling until it works

    int deviceCount = device.getDeviceCount();
    printf("Found %d sensors\n\r",deviceCount);

    device.setResolution(twelveBit);
}
unsigned int pwm_cold_peltier(int brightness )
{
    if (brightness==1) {
        peltier_cold_pulsewidth=peltier_cold_pulsewidth+peltier_step;
        peltier_cold.pulsewidth_us(peltier_cold_pulsewidth);
    };
    if (brightness==0) {
        peltier_cold_pulsewidth=peltier_cold_pulsewidth-peltier_step;
        peltier_cold.pulsewidth_us(peltier_cold_pulsewidth);
    };
    return peltier_cold_pulsewidth;
}
unsigned int pwm_hot_peltier(int brightness)
{
    if (brightness==1) {
        peltier_hot_pulsewidth=peltier_hot_pulsewidth+peltier_step;
        peltier_hot.pulsewidth_us(peltier_hot_pulsewidth);
    }
    if (brightness==0) {
        peltier_hot_pulsewidth=peltier_hot_pulsewidth-peltier_step;
        peltier_hot.pulsewidth_us(peltier_hot_pulsewidth);
    }
    return peltier_hot_pulsewidth;
}
void peltier(bool fan_in,bool fan_out,bool cold,bool hot,int pwm_procent)
{
    //tempinit();
    //sensor.mode(PullUp);
   // readTemp(1);
   // dsen_temp_in=temp;
    //  ROM_Code_t ROM_Code = ReadROM();
    //  lcd.setCursor(TextLCD::CurOff_BlkOn);
    //  lcd.locate(0,0);
    //  lcd.printf("Family code: 0x%X\n\r", ROM_Code.BYTES.familyCode);
    //  wait(5.0);
    bool read_temp_flag = true;
    if( fan_in==false &&  fan_out==false && cold == false && hot==false){
        read_temp_flag = false;
    }
    en_drv1=0;//disable
    en_drv2=0;//disable
    fan_in_peltier=0;//disable
    fan_out_peltier=0;//disable


    if(fan_in==true) {
        fan_in_peltier=1;//enable
    }
    if(fan_out==true) {
        fan_out_peltier=1;//enable
    }
    if ((cold==true)||(hot==true)) {
        en_drv1=1;
        en_drv2=1;
    }

    if ((cold==true)&&(hot==false)) {

        peltier_cold.period_us(peltier_period);
        peltier_cold_pulsewidth=pwm_procent*10;
        peltier_cold.pulsewidth_us(peltier_cold_pulsewidth);
    }
    if ((hot==true)&&(cold==false)) {

        peltier_hot.period_us(peltier_period);
        peltier_hot_pulsewidth=pwm_procent*10;
        peltier_hot.pulsewidth_us(peltier_hot_pulsewidth);
    }
        
        if(read_temp_flag){
       /* readTemp(1);
        dsen_temp_in=temp;
        readTemp(0);
        dsen_temp_out=temp;*/
        }
        //printf("Pinside Temp is %2.1fC, outside temp plate is %2.1fC\n\r", dsen_temp_in,dsen_temp_out);
    //lcd.cls();
   // lcd.locate(0,1);
   // lcd.printf("Temp is %2.1fC\n\r", dsen_temp_in);

}
void pel_c(int pwm)
{
    peltier_cold.period_us(peltier_period);
    peltier_cold_pulsewidth=pwm*10;
    peltier_cold.pulsewidth_us(peltier_cold_pulsewidth);

}
void pel_h(int pwm)
{
    peltier_hot.period_us(peltier_period);
    peltier_hot_pulsewidth=pwm*10;
    peltier_hot.pulsewidth_us(peltier_hot_pulsewidth);
}
void peltier_auto(float degree){
//tempinit();

   // backlight=1;

    en_drv1=1;
    en_drv2=1;
    fan_in_peltier=1;//enable
    fan_out_peltier=1;//enable
    while(1) {

        readTemp(1);
        dsen_temp_in=temp;

        /* (degree>dsen_temp) {
            pel_h(0);
            pel_c(90);
        }*/
        if (degree<dsen_temp_in) {
            pel_c(0);
            pel_h(90);

        }
        if (degree==dsen_temp_in) {
            pel_c(0);
            pel_h(0);

        }

        //lcd.cls();
        //lcd.locate(0,1);

        readTemp(1);
        dsen_temp_in=temp;
        readTemp(0);
        dsen_temp_out=temp;
        //pc.printf("Temp is %2.1fC,dsen temp= %2.1f , outchamber plate temp =%2.1fC\n\r", dsen_temp_in,degree,dsen_temp_out);
        // lcd.printf("Temp is %2.1fC\n\r", dsen_temp);



        //peltier_hot.period_us(peltier_period);
        //peltier_hot_pulsewidth=1000;//at 50%
        //pwm_hot_peltier(1);
        // wait(1.0);



        /* peltier_cold.period_us(peltier_period);
         peltier_cold_pulsewidth=peltier_pulsewidth;
         for (int i=1; i<=100; i++) {
             pwm_cold_peltier(1);
             wait (0.1);
         }
         for (int i=1; i<=100; i++) {
             pwm_cold_peltier(0);
             wait (0.1);
         }
         peltier_cold_pulsewidth=10;    // set to 0
         pwm_cold_peltier(0);           // set to 0

         peltier_hot.period_us(peltier_period);
         peltier_hot_pulsewidth=peltier_pulsewidth;
         for (int i=1; i<=100; i++) {
             pwm_hot_peltier(1);
             wait (0.1);
         }
         for (int i=1; i<=100; i++) {
             pwm_hot_peltier(0);
             wait (0.1);
         }
         peltier_hot_pulsewidth=10;    // set to 0
         pwm_hot_peltier(0);           // set to 0
         */
    }

}
