#include "DHT.h"

extern int error_outside;
extern int error_inside;

float get_air_temp_outside(int tempunit);
float get_air_humid_outside();
float get_air_dewpoint_outside();
float get_air_temp_inside(int tempunit);
float get_air_humid_inside();
float get_air_dewpoint_inside();
