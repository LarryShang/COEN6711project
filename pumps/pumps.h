extern DigitalOut pump_up;
extern DigitalOut pump_bottom;
extern DigitalOut valve_1;
extern DigitalOut humidity_fan;

//DigitalOut pump1(PTC6);
//DigitalOut pump2(PTC5);
//DigitalOut valve1(PTE31);
//DigitalOut humidity_fan1(PTA17);
void pumps(bool pumpup,bool pumpbottom,bool valve1,bool humidityfan);