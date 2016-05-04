#include <stdio.h>
#include <time.h>

extern "C" int init(int d_level);
extern "C" int sleep(int sec, int usec);
extern "C" int set_motor(int motor, int value);
extern "C" int take_picture();
extern "C" int get_pixel(int row, int col, int colour);
extern "C" int open_screen_stream();
extern "C" int close_screen_stream();
extern "C" int update_screen();
using namespace std;

int main()
{
    init(0);
    int power = 100;
    int c = 0;
    open_screen_stream();
    while(c++ < 100){
        take_picture();
        update_screen();
        int left = 0;
        int right = 0;
        for(int i = 0; i < 160; i++){
            left += get_pixel(i, 120, 3);
        }
        for(int i = 160; i < 320; i++){
            right += get_pixel(i, 120, 3);
        }
        right = right / 160;
        left = left / 160;
        set_motor(1, left);
        set_motor(2, right);
        sleep(0, 10000);
    }
    close_screen_stream();
    return 0;
}

