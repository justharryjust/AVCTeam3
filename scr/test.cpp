#include <stdio.h>
#include <time.h>

extern "C" int init(int d_level);
extern "C" int write_digital(int chan, char level);
extern "C" int read_analog(int chan);
extern "C" int sleep(int sec, int usec);
extern "C" int set_motor(int motor, int value);

using namespace std;

int main()
{
    init(0);
    int power = 100;
    set_motor(1, power);
    set_motor(2, power);
    sleep(3, 0);
    return 0;
}

