#include <iostream>
#include <stdio.h>
#include <time.h>

extern "C" int init(int d_level);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor, int value);
extern "C" int read_analog(int ch_adc);

using namespace std;

const int speed = 60;

int main()
{
init(1);
signal(2,signal_callback_handler);//prevents the pi spazzing out with ctrl+c

 while (true){//test loop to find the number to replace 30 with, so that we can control distance of stopping/ point of turning
    int sen1 = read_analog(1);//left
    int sen2 = read_analog(0);//middle
    int sen3 = read_analog(2);//right
    printf("\nSensor 3 "+sen3);
    int dif = sen3-400;
    dif = dif * 0.15;
    set_motor(1,speed+dif);
    set_motor(2,speed-dif);
    printf("\nThe difference is  "+dif);
    
     if(sen2>200 && sen3>300 && sen1>300) {//should turn 180 degrees on the spot
        set_motor(1,0);
        set_motor(2,0);
        Sleep(1,0);
        set_motor(1,speed);
        set_motor(2,-speed);
        sleep(1,0);
      }
      
      else if(sen2>200 && sen3>300) {//should turn 90 degrees but in an arc following the curve
        set_motor(1,0);
        set_motor(2,0);
        Sleep(1,0);
        set_motor(1,speed);
        set_motor(2,0);
        sleep(1,0);
      }
 
 }
 
}

