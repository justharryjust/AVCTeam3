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
  open_screen_stream();
  while (true){
   take_picture();
   update_screen(); 
   //getting the hlafway pixel
   for (i=0;i<320;i++){
     int whiteness = get_pixel(i, 120, 3);
     if (whiteness>100){whiteness = 255;}
   }
  }
  }
  
     return 0;
}
