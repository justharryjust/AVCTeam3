#include <stdio.h>
#include <time.h>

extern "C" int init_hardware();
extern "C" int write_digital(int chan, char level);
extern "C" int read_analog(int chan);
extern "C" int sleep(int sec, int usec);
extern "C" int set_motor(int motor, int dir, int value);

using namespace std;

int main()
{
    init_hardware();
    int power = 255;
    set_motor(1, 1, power);
    set_motor(2, 1, power);
    sleep(3, 0);
    set_motor(1, 1, 0);
    set_motor(2, 1, 0);
    return 0;
}

