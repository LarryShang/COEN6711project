extern PwmOut led_blue;
extern PwmOut led_green;
extern PwmOut led_red;

extern unsigned int led_red_pulsewidth;
extern unsigned int led_green_pulsewidth;
extern unsigned int led_blue_pulsewidth;
extern unsigned int led_period ;
extern unsigned int led_pulsewidth;
extern unsigned int led_step;

extern bool led_set_flag;
extern int MAX_RED_LIGHT;
extern int MAX_BLUE_LIGHT;
extern int MAX_GREEN_LIGHT;

unsigned int pwm_blue_led(int brightness );
unsigned int pwm_green_led(int brightness);
unsigned int pwm_red_led(int brightness);
void led_test(void);
void turn_off_leds();
void set_leds();