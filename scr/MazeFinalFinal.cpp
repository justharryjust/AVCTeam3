#include <iostream>
#include <stdio.h>
#include <time.h>
#include <cstdlib>
#include <signal.h>

extern "C" int init(int d_level);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor, int value);
extern "C" int read_analog(int ch_adc);

using namespace std;

const int speed1 = 60;

//Does Not do dead ends!!!
//Follows Right wall. (No dead ends)
//atm left sensor is not used!
//the "signal_callback_handler" stuff is to stop the pi spazzing out when ctrl+c

void signal_callback_handler(int signum){
set_motor(1,0);
set_motor(2,0);
exit(signum);

}
int main()
{
    init(1);
    signal(2,signal_callback_handler);

    while (1){
        int sen1 = read_analog(1);//left
        int sen2 = read_analog(0);//middle
        int sen3 = read_analog(2);//right

        int dif = sen3-410;
        if(dif<-240){dif=-240;}//used to maintain constant distance from wall
        dif=dif*0.15;
        set_motor(1,speed1+dif);
        set_motor(2,speed1-dif);

    if(sen2>200 && sen3>200){//turning Left method
        set_motor(1,0);
        set_motor(2,0);
        Sleep(1,0);
        set_motor(1,speed1+28);
        set_motor(2,0);
        Sleep(1,0);

    }
    }
    return 0;

}
