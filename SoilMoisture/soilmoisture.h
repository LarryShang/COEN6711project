extern AnalogIn soil_sense1;
extern AnalogIn soil_sense2;


extern unsigned short CMA_soil1;
extern unsigned short CMA_soil2; 

extern float soil1_humid;
extern float soil2_humid;

void read_soil_humidity();
void calc_soil_humid_values();
