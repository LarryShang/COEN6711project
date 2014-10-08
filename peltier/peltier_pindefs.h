//DigitalInOut sensor(PTE1);                                              // sensor inside E0
PwmOut peltier_cold(PTA12);
PwmOut peltier_hot(PTA4);
DigitalOut en_drv1(PTC13);
DigitalOut en_drv2(PTC16);
DigitalOut fan_in_peltier(PTC11);                                       // Peltier FAN inside  the camber
DigitalOut fan_out_peltier(PTC10);                                      // Peltier FAN outside of the chamber