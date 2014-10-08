//extern DigitalInOut sensor;                                              // sensor inside E0
extern PwmOut peltier_cold;
extern PwmOut peltier_hot;
extern DigitalOut en_drv1;
extern DigitalOut en_drv2;
extern DigitalOut fan_in_peltier;                                       // Peltier FAN inside  the camber
extern DigitalOut fan_out_peltier;                                 // Peltier FAN outside of the chamber

extern unsigned int peltier_hot_pulsewidth;
extern unsigned int peltier_cold_pulsewidth;
extern unsigned int peltier_period;
extern unsigned int peltier_pulsewidth;
extern unsigned int peltier_step;

extern bool alarm_hi_temp_in;
extern bool alarm_lo_temp_in;
extern bool alarm_hi_temp_out;
extern bool alarm_lo_temp_out;
extern float dsen_temp_in; //of plate inside
extern float dsen_temp_out; //of plate outside
//extern Serial pc; // tx, rx

unsigned int pwm_cold_peltier(int brightness );
unsigned int pwm_hot_peltier(int brightness);

extern float temp;

void tempinit();
void readTemp(int deviceNum);
void peltier(bool fan_in,bool fan_out,bool cold,bool hot,int pwm_procent);
void peltier_auto(float degree);

