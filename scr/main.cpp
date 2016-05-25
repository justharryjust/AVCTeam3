#include <iostream>
#include <stdio.h>
#include <time.h>

extern "C" int init(int d_level);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor, int value);
extern "C" int read_analog(int ch_adc);
void turnLeft();
void turnRight();


using namespace std;

const int speed = 110;

//Need method to turn right/left 90
//Not sure which motor is which


int main()
{
//change numbers based on which inputs sensors are connected to. 1 is the left most sensor, 2 is the middle, 3 is the right
    int sen1 = read_analog(0);//left
    int sen2 = read_analog(1);//middle
    int sen3 = read_analog(2);//right

    while (1){
    while (sen2>30 && sen1<30){
    sen1 = read_analog(0);//change numbers based on which inputs sensors are connected to. 1 is the left most sensor, 2 is the middle, 3 is the right
    sen2 = read_analog(1);
    sen3 = read_analog(2);
    int dif = sen1-sen3;
    dif=dif*0.5;
    set_motor(1,speed+dif);//change +-
    set_motor(2,speed-dif);//possibly going to rotate
    printf("The difference is "+dif);
     }
     if(sen1<30 && sen2<30 && sen3<30){
     turnLeft();
     turnLeft();
     }
    else if(sen1>30){turnLeft();}
    else if (sen2<30 && sen1<30){turnRight();}
    else {
    set_motor(1,-100);
    set_motor(2,-100);
    }
    }
    return 0;
}

void turnLeft(){
    set_motor(1,speed);
    set_motor(2,-speed);//possible 0 instead of -speed
    Sleep(1,0);//turns for 1 second, posibly too long/ too short
    }

void turnRight(){
    set_motor(1,-speed);//possibly 0 instead of -speed
    set_motor(2,speed);
    Sleep(1,0);//turns for 1 second
    }
