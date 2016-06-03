#include<iostream> 
#include<stdio.h> 
#include<time.h> 
#include<chrono> 
#include<cstdlib> 
#include<signal.h>

extern "C" int init(int d_level);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor, int value);
extern "C" int take_picture();
extern "C" int get_pixel(int row, int col, int colour);
extern "C" int open_screen_stream();
extern "C" int close_screen_stream();
extern "C" int update_screen();
extern "C" int connect_to_server(char server_addr[15], int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);
extern "C" int read_analog(int ch_adc);
extern "C" int set_pixel(int row, int col, char red, char green, char blue);

using namespace std;
using namespace std::chrono;

const int white_value = 110;
const int camera_y_coord = 0;
const int min_line_width = 25;
const float speed = 0.9; // working value 0.9
const int speed1 = 60; // speed for maze code
bool run_sensor = true;
float P = 0.28; // working value 0.28
float I = 0.0; // working value 0.0
float D = 50; // working value 50
float max_error = 160; // working value 160

// This method is called when the program exists
void signal_callback_handler(int signum)
{
    set_motor(1, 0);
    set_motor(2, 0);
    exit(signum);
}

// Return the error signal from the current picture
int error_method_4(int cam_y)
{
    int min = 0;
    int consec_white_pixels = 0;
    for (int i = 0; i < 320; i++) { // Loop through line of pixels at given y coord
        int value = get_pixel(i, cam_y, 3);
        if (value > white_value) { // If the pixel is white
            consec_white_pixels += 1; // Increate the consecutive pixel counter
            if (consec_white_pixels >= min_line_width) { // If there are enough white pixels
                min = i - consec_white_pixels; // Set min to where the consecutive pixels started
                break; // Stop the loop
            }
        }
        else { // If the pixel is black
            consec_white_pixels = 0; // Reset the pixel counter
        }
    }
    int max = 0;
    consec_white_pixels = 0;
    for (int i = 320; i >= 0; i--) { // Loop through the line of pixels again, this time 
        int value = get_pixel(i, camera_y_coord, 3); // back to front
        if (value > white_value) { // if the pixels is white
            consec_white_pixels += 1; // increate the counter
            if (consec_white_pixels >= min_line_width) { // if the counter is big enough
                max = i + consec_white_pixels; // Set min to where the consecutive pixels started
                break; // Stop the loop
            }
        }
        else { // If pixel si black
            consec_white_pixels = 0; // Reset the pixel counter
        }
    } 
    // Return the average of the min and max point of the line, ie the middle,
    // and set subtract 160 so that the signal is 0 in the center of the picture,
    return ((min + max) / 2) - 160; // -ive to the left and +ive to the right.
}

// Restrict val between a and b
int clamp(int a, int b, int val)
{
    if (val < a) {
        return a;
    }
    if (val > b) {
        return b;
    }
    return val;
}

// Return the current time stamp in miliseconds as a long
long get_time()
{
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

// Draw what the camera is seeing on screen in black & white
void print_picture()
{
    for (int i = 0; i < 240; i++) {
        for (int j = 0; j < 320; j++) {
            int value = get_pixel(j, i, 3);
            if (value > white_value) {
                set_pixel(j, i, 255, 255, 255);
            }
            else {
                set_pixel(j, i, 0, 0, 0);
            }
            if (i == 0) {
                set_pixel(j, i, 200, 0, 0);
            }
        }
    }
    update_screen();
}

// Return true if there is a sharp right turn
bool line_on_right()
{
    int white_pixels = 0;
    for (int i = 0; i < 160; i++) { // Loop from the right to the middle of the picture
        int value = get_pixel(i, camera_y_coord, 3);
        if (value > white_value) { // if the pixel is white
            white_pixels += 1; // add to the pixel counter
        }
        if (white_pixels > 150) { // if 150/160 pixels are white
            return true; // there is a sharp right turn, return true
        }
    }
    return false; // return false otherwise
}

// Return true if there is a sharp left turn
bool line_on_left()
{
    int white_pixels = 0;
    for (int i = 160; i < 320; i++) { // Loop from the middle to the left of the picture
        int value = get_pixel(i, camera_y_coord, 3);
        if (value > white_value) { // if the pixel is white
            white_pixels += 1; // add to the pixel counter
        }
        if (white_pixels > 150) { // if 150/160 pixels are white
            return true; // there is a sharp left turn, return true
        }
    }
    return false; // return false otherwise
}

// Return true the robot is at an intersection
bool is_at_intersection()
{
    int white_pixels = 0;
    for (int i = 0; i < 320; i++) { // Loop through a line on the picture
        int value = get_pixel(i, camera_y_coord, 3);
        if (value > white_value) { // if the pixel is white
            white_pixels += 1; // add to the pixel counter
        }
        if (white_pixels > 300) { // if 300/320 pixels are white
            return true; // there is an intersection, return true
        }
    }
    return false; // return false otherwise
}

// Return true if there is a line to make an error signal
bool is_line_there(int camera_y)
{
    int consec_white_pixels = 0;
    for (int i = 0; i < 320; i++) { // Loop through a line on the picture
        int value = get_pixel(i, camera_y, 3);
        if (value > white_value) { // if the pixel is white
            consec_white_pixels += 1; // add to consecutive pixel counter
            if (consec_white_pixels >= min_line_width) { // if the counter is big enough
                return true; // return true, there is a line there
            }
        }
        else { // if the pixel is black
            consec_white_pixels = 0; // reset the counter
        }
    }
    return false;
}

int integral = 0;
int last_error = 0;
long last_loop_end_time = 0;
// Do one iteration of the line following code
void follow_loop()
{
    take_picture(); // Take a new picture
    int error = 0;
    if (is_line_there(camera_y_coord)) { // if there is a line
        error = error_method_4(camera_y_coord); // get the error signal from that line
    }
    else { if there is no line (lost the line)
        if (last_error > 0) { // if the last error was > 0
            error = max_error; // set the error to the maximum error
        }
        else { // otherwise
            error = 0 - max_error; // set the error to negative of the maximum error
        }
    }
    if (is_at_intersection()) { // If the robot is at an intersection
        if (is_line_there(220 /*220 = pixel near top */)) { 
            //If there is a going line forward
            error = error_method_4(220); // get error based on that line
        }
        else { // otherwise
            error = max_error; //go left
        }
    }
    else if (line_on_left()) {// If the robot is at a sharp left turn
        if (is_line_there(220 /*220 = pixel near top */)) {
            error = error_method_4(220); // get error based on that line
        }
        else { // otherwise
            error = max_error; //go left
        }
        error = max_error;
    }
    else if (line_on_right()) { // If the robot is at a sharp right turn
        if (is_line_there(220 /*220 = pixel near top */)) {
            error = error_method_4(220); // get error based on that line
        }
        else { // otherwise
            error = 0 - max_error; //go right
        }
    }
    print_picture(); // print out the current picture
    integral += error; // the integral is the runing total of errors
    long time = get_time(); // get the current timestamp
    // The derivative is change in error over change in time
    float derivitive = ((float)(error - last_error)) / ((float)(time - last_loop_end_time));
    last_loop_end_time = time; // set the last end time to the current time
    last_error = error; // set last error to current error
    int final_signal = P * error + I * integral + D * derivitive; // make the final signal using PID constants
    cout << "error " << error << endl; // print out the error for debugging
    cout << "integral " << integral << endl; // print out the integral for debugging
    cout << "derivitive " << derivitive << endl; // print out the derivitive for debugging
    cout << "final " << final_signal << endl; // print out the final signal for debugging
    // set the motor speeds to a value that iss  50 +- the final signal, but also between -100 and 150
    set_motor(1, speed * clamp(-100, 150, 50 + final_signal));
    set_motor(2, speed * clamp(-100, 150, 50 - final_signal));
}

// main method, run when the program is started
int main()
{
    init(0); // initialise the e101 library
    open_screen_stream(); // open the stream for printing the picture on the screen

    connect_to_server("130.195.6.196", 1024); // connect to the gate server
    send_to_server("Please"); // send "Please" to the gate sever
    char pswd[24]; // Create a char array to store the password
    receive_from_server(pswd); // Get the password from the sever and store it
    send_to_server(pswd); // Send the password back to the server

    int count = 0; // a counter for delaying the activation of the maze code
    last_error = error_method_4(0); // initialise the last error variable
    last_loop_end_time = get_time(); // initialise the loop end time variable
    while (true) { // repeat forever
        int sen3 = read_analog(2); // read the right ir sensor
        int sen1 = read_analog(1); // read the left ir sensor
        int sen2 = read_analog(0); // read the middle ir sensor
        count++;
        if (sen3 < 300 && sen1 < 300) { // if the sensors dont see anythign close
            follow_loop(); //follow the line
        }
        else if (sen2 > 300) { // if the middle sensor seed something close
            set_motor(1, 0); // stop the motor
            set_motor(2, 0); // stop the other motor
            last_loop_end_time = get_time(); // set the last loop time
        }
        else if ((sen3 > 400 || sen1 > 400) && sen2 < 300 && count > 200) {
            while (1) {
                int sen1 = read_analog(1); // read the left ir sensor
                int sen2 = read_analog(0); // read the middle ir sensor
                int sen3 = read_analog(2); // read the right ir sensor

                int dif = sen3 - 400; //distance from right hand wall (Smaller is a great distance)
                if (dif < -220) {
                    dif = -220;
                } //sharpness of turn (smaller is less sharp)
                dif = dif * 0.15; //how responsive robot is
                set_motor(1, speed1 + dif);
                set_motor(2, speed1 - dif);

                if (sen2 > 220 && sen3 > 200) { //turning Left method
                    set_motor(1, 0);
                    set_motor(2, 0);
                    Sleep(1, 0);
                    set_motor(1, speed1 + 40); //speed of turn
                    set_motor(2, 0);
                    Sleep(0, 600000); //turn duration
                }
            }
        }
    }

    return 0;
}
