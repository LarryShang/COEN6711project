#include "mbed.h"
#include "airhumidity.h"
#include "DHT.h"

int error_outside;
int error_inside;

DHT sensor_outside(PTD1,SEN11301P); 
DHT sensor_inside(PTD3,SEN11301P); 

float get_air_temp_outside(int tempunit){
    wait(1); 
    error_outside = sensor_outside.readData();
    switch (tempunit){
        case 0:
            return sensor_outside.ReadTemperature(CELCIUS);
            //break;
        case 1:
            return sensor_outside.ReadTemperature(FARENHEIT);
            //break;
        case 2:
            return sensor_outside.ReadTemperature(KELVIN);
           // break;
        default:
            return sensor_inside.ReadTemperature(CELCIUS);
           // break;
    }
}

float get_air_humid_outside(){
    wait(1); 
    error_outside = sensor_outside.readData();
    return sensor_outside.ReadHumidity();
}

float get_air_dewpoint_outside(){
      wait(1); 
    error_outside = sensor_outside.readData();
    return sensor_outside.CalcdewPointFast(sensor_outside.ReadTemperature(CELCIUS), sensor_outside.ReadHumidity());
    
}

//INSIDE FUNCTIONS
float get_air_temp_inside(int tempunit){
    wait(1); 
    error_inside = sensor_inside.readData();
    switch (tempunit){
        case 0:
            return sensor_inside.ReadTemperature(CELCIUS);
          //  break;
        case 1:
            return sensor_inside.ReadTemperature(FARENHEIT);
           // break;
        case 2:
            return sensor_inside.ReadTemperature(KELVIN);
        //    break;
        default:
            return sensor_inside.ReadTemperature(CELCIUS);
        //    break;
    }
}


float get_air_humid_inside(){
    wait(1); 
    error_inside = sensor_inside.readData();
    return sensor_inside.ReadHumidity();
}

float get_air_dewpoint_inside(){
    wait(1); 
    error_inside = sensor_inside.readData();
    return sensor_inside.CalcdewPointFast(sensor_inside.ReadTemperature(CELCIUS), sensor_inside.ReadHumidity());
    
}
//printf("Temperature is %4.0f C \r\n",sensor.ReadTemperature(CELCIUS));
    //printf("Temperature is %4.2f F \r\n",sensor.ReadTemperature(FARENHEIT));
    //printf("Temperature is %4.2f K \r\n",sensor.ReadTemperature(KELVIN));
    //printf("Humidity is %4.2f \r\n",sensor.ReadHumidity());
    //printf("Dew point is %4.2f  \r\n",sensor.CalcdewPoint(sensor.ReadTemperature(CELCIUS), sensor.ReadHumidity()));
    //printf("Dew point (fast) is %4.2f  \r\n",sensor.CalcdewPointFast(sensor.ReadTemperature(CELCIUS), sensor.ReadHumidity()));