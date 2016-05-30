#include <stdio.h>
#include<time.h>
#include <iostream>

extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);
extern "C" int read_analog(int ch_adc);
extern "C" int Sleep(int sec, int usec);
extern "C" bool run_sensor=true;


using namespace std;


int main(){
	//This sets up the RPi hardware and ensures
	//everything is working correctly
	init(1);
	//We declare an integer variable to store the ADC data


	int adc_reading;

	while(run_sensor){
  	adc_reading = read_analog(0);

	printf("%d\n", adc_reading);

	Sleep(0,500000);

		if(adc_reading>300){
		run_sensor=false;
		}
	}

//connects to server with the ip address 130.195.6.196

    connect_to_server("130.195.6.196",1024);

//sends a message to the connected server

    send_to_server("Please");
    char pswd[24];
    receive_from_server(pswd);
    send_to_server(pswd);

return 0;
}
