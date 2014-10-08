#include "mbed.h"
#include "pumps.h"
//#include "pumps_pindefs.h"


void pumps(bool pumpup,bool pumpbottom,bool valve1,bool humidityfan)
{
    pump_up=0;
    pump_bottom=0;
    valve_1=0;
    humidity_fan=0;
    if(pumpup==true) {
        pump_up=1;//enable
    }
    if(pumpbottom==true) {
        pump_bottom=1;//enable
    }
    if(valve1==true) {
        valve_1=1;//enable
    }
    if(humidityfan==true) {
        humidity_fan=1;//enable
    }


}