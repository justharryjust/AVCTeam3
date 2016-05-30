#include <iostream>
#include <stdio.h>
#include <time.h>
#include <chrono>

extern "C" int init(int d_level);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor, int value);
extern "C" int take_picture();
extern "C" int get_pixel(int row, int col, int colour);
extern "C" int open_screen_stream();
extern "C" int close_screen_stream();
extern "C" int update_screen();
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);
extern "C" int read_analog(int ch_adc);
extern "C" int set_pixel(int row, int col, char red, char green, char blue);

using namespace std;
using namespace std::chrono;

//PID coefficients
const float P = 0.28;
const float I = 0.0;
const float D = 50;

//Other constants
const int white_value = 110;
const int min_line_width = 40;
const float speed = 0.9;

//Restrict val between a and b
int clamp(int a, int b, int val){
	if(val < a){
		return a;
	}
	if(val > b){
		return b;
	}
	return val;
}

long get_time(){
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void print_picture(){
	for(int i = 0; i < 240; i++){
		for(int j = 0; j < 320; j++){
			int value = get_pixel(j, i, 3);
			if(value > white_value){
				set_pixel(j, i, 255, 255, 255);
			} else {
				set_pixel(j, i, 0, 0, 0);
			}
		}
	}
	update_screen();
}

int get_error(int y_value){
	int min = 0;
	int consec_white_pixels = 0;
	for(int i = 0; i < 320; i++){
		int value = get_pixel(i, y_value, 3);
		if(value>white_value){
			consec_white_pixels += 1;
			if(consec_white_pixels >= min_line_width){
				min = i - consec_white_pixels;
				break;
			}
		}
		else {
			consec_white_pixels = 0;
		}
	}
	int max = 0;
	consec_white_pixels = 0;
	for(int i = 320; i >=0; i--){
		int value = get_pixel(i, y_value, 3);
		if(value>white_value){
			consec_white_pixels += 1;
			if(consec_white_pixels>=min_line_width){
				max = i + consec_white_pixels;
				break;
			}
		} else {
			consec_white_pixels = 0;
		}
	}
	return ((min + max)/2)-160;
}

bool vertical_line(int y_coord){
	int consec_white_pixels = 0;
	for (int i = 0;i<320;i++) {
		int value = get_pixel(i, y_coord, 3);
		if (value>white_value) {
			consec_white_pixels += 1;
			if(consec_white_pixels >= min_line_width){
				return true;
			}
		} else {
			consec_white_pixels = 0;
		}
	}
	return false;
}

bool is_intersection(int y_coord){
	int consec_white_pixels = 0;
	for (int i = 0;i<320;i++) {
		int value = get_pixel(i, y_coord, 3);
		if (value>white_value) {
			consec_white_pixels += 1;
			if(consec_white_pixels >= 200 /* 200 = 80% are white*/){
			return true;
		}
	} else {
		consec_white_pixels = 0;
	}
}
return false;
}

bool horizontal_line(int x_coord){
	int consec_white_pixels = 0;
	for (int i = 0;i<240;i++) {
		int value = get_pixel(x_coord, i, 3);
		if (value>white_value) {
			consec_white_pixels += 1;
			if(consec_white_pixels >= min_line_width){
				return false;
			}
		} else {
			consec_white_pixels = 0;
		}
	}
	return true;
}



//Initial values
int integral = 0;
int last_error = 0;
long last_loop_end_time = 0;
void follow_line(){
	take_picture();

	print_picture();
	if(is_intersection(0)){
		if(vertical_line(230 /* 230y = top pixel */)){

		} else if (horizontal_line(0 /* 0x = left pixel */)){

		} else if (horizontal_line(310 /* 310x = top pixel */)){

		}
	} else if(vertical_line(0 /* 0y = bottom pixel */);){
		//if there is a lineon the bottom pixel, get the error from it
		int error = get_error(0 /* 0 = bottom pixel */);
	} else {
		//lost the line
		error = last_error;
	}

	integral += error;
	long time = get_time();
	float derivitive = ((float)(error - last_error))/((float)(time - last_loop_end_time));
	last_loop_end_time = time;
	last_error = error;

	int final_signal = P*error + I*integral + D*derivitive;

	//Remove clamp?
	set_motor(1 , speed*clamp(-100, 150, 50+final_signal));
	set_motor(2 , speed*clamp(-100, 150, 50-final_signal));
}

int main()
{
	init(0);
	open_screen_stream();

	//    connect_to_server("130.195.6.196",1024);
	//    send_to_server("Please");
	//    char pswd[24];
	//    receive_from_server(pswd);
	//    send_to_server(pswd);

	int adc_reading = 0;
	last_error = get_error();
	last_loop_end_time = get_time();
	while(true){
		adc_reading = read_analog(0);
		if(adc_reading<400){
			//Follow the line
			follow_line();
		} else {
			//Stop
			set_motor(1 , 0);
			set_motor(2 , 0);
			last_loop_end_time = get_time();
		}
	}
	return 0;
}