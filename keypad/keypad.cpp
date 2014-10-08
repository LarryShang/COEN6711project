#include "mbed.h"
#include "buzz.h"
//#include "buzz.cpp"


//#include "TextLCD.h"
//extern void timer1(void);
//extern void beep(float freq,int time);

BusIn colonne(PTE2,PTE3,PTE4,PTA16,PTC0,PTC7);
BusOut righe(PTC17,PTD4,PTB10,PTB9,PTB8);//A2->D4, A1->C17
//TextLCD lcd(PTE5,PTE4,PTE3,PTE2,PTB11,PTB10);

char Keypad(void)
{
    char key = 100;
  //  righe.write(31);//11111
  //  switch(colonne) {
  //      case 31:
  //          key = 1;
  //          break;//011111
  //      case 47:
  //          key = 2;
  //          break;//101111
  //      case 55:
  //          key = 3;
  //          break;//110111
  //      case 59:
  //          key = 4;
  //          break;//111011
  //      case 61:
  //          key = 5;
  //          break;//111101
  //      case 62:
  //          key = 6;
  //          break;//111110
 //   }
    righe.write(30);//11110
    switch(colonne) {
        case 31:
            key = 7;
            beep(0.0002,100);
            break;//011111
        case 47:
            key = 8;
            beep(0.0002,100);
            break;//101111
        case 55:
            key = 9;
            beep(0.0002,100);
            break;//110111
        case 59:
            key = 10;
            beep(0.0002,100);
            break;//111011
        case 61:
            key = 11;
            beep(0.0002,100);
            break;//111101
        case 62:
            key = 12;
            beep(0.0002,100);
            break;//111110
    }
    righe.write(29);
    switch(colonne) {
        case 31:
            key = 13;
            beep(0.0002,100);
            break;//011111
        case 47:
            key = 14;
            beep(0.0002,100);
            break;//101111
        case 55:
            key = 15;
            beep(0.0002,100);
            break;//110111
        case 59:
            key = 16;
            beep(0.0002,100);
            break;//111011
        case 61:
            key = 17;
            beep(0.0002,100);
            break;//111101
        case 62:
            key = 18;
            beep(0.0002,100);
            break;//111110
    }
    righe.write(27);//11011
    switch(colonne) {
        case 31:
            key = 19;
            beep(0.0002,100);
            break;//011111
        case 47:
            key = 20;
            beep(0.0002,100);
            break;//101111
        case 55:
            key = 21;
            beep(0.0002,100);
            break;//110111
        case 59:
            key = 22;
            beep(0.0002,100);
            break;//111011
        case 61:
            key = 23;
            beep(0.0002,100);
            break;//111101
        case 62:
            key = 24;
            beep(0.0002,100);
            break;//111110
    }
    righe.write(23);//10111
    switch(colonne) {
        case 31:
            key = 25;
            beep(0.0002,100);
            break;//011111
        case 47:
            key = 26;
            beep(0.0002,100);
            break;//101111
        case 55:
            key = 27;
            beep(0.0002,100);
            break;//110111
        case 59:
            key = 28;
            beep(0.0002,100);
            break;//111011
        case 61:
            key = 29;
            beep(0.0002,100);
            break;//111101
        case 62:
            key = 30;
            beep(0.0002,100);
            break;//111110
    }
    righe.write(15);//01111
    switch(colonne) {
        case 31:
            key = 31;
            beep(0.0002,100);
            break;//011111
        case 47:
            key = 32;
            beep(0.0002,100);
            break;//101111
        case 55:
        
            key = 33;
            beep(0.0002,100);
            break;//110111
        case 59:
            key = 34;
            beep(0.0002,100);
            break;//111011
        case 61:
            key = 35;
            beep(0.0002,100);
            break;//111101
        case 62:
            key = 36;
            beep(0.0002,100);
            break;//111110
    }
    key=key+50;
    return key;
}


