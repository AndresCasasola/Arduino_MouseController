/*

* Arduino mousecontroller
----------------------------------------------------------

* A program that move the mouse in function of data reading from an

* Arduino board. Works on any POSIX system (Mac/Unix/PC).

* This example/application use photoresistors and visible light leds to 

* sense the movement of the hand.
----------------------------------------------------------

* Created 28 December 2017.

* Copyleft (c) 2017, Andres Casasola Domínguez.
----------------------------------------------------------

* Last update: March 2018.

* GitHub: AndresCasasola.

*/

#include <stdio.h>    				// Standard input/output definitions
#include <stdlib.h> 				// Standard library
#include <stdint.h>   				// Standard types
#include <string.h>   				// String function definitions
#include <unistd.h>   				// UNIX standard function definitions
#include <fcntl.h>    				// File control definitions
#include <errno.h>    				// Error number definitions
#include <termios.h>  				// POSIX terminal control definitions
#include <sys/ioctl.h>				// Input/Output control
#include <getopt.h>					// Getopt
#include "arduino-serial-lib.h"		// Libraries for serial communication -> Thanks to todbot, GitHub: https://github.com/todbot
#include "arduino-serial-lib.c"
#include <sys/types.h>				// Types
#include <sys/stat.h>				// Library for read/analyze files
#include <linux/uinput.h>			// Library to use virtual mouse
#include <math.h>					// Math

#define DEVICE_FILENAME "/dev/ttyACM0"		// Usually will be ttyUSB0 or ttyACM0, ttyACM1,...
#define BAUDRATE 9600						// Baudrate from/to arduino

//--------Global Variables -----------------------------------//

static int uinput_fd = 0;

//----------------------------------------------------------//

// Function Headers

int process_received_data(char *buf);
int uinput_mouse_init(void);
void uinput_mouse_move_cursor(char left, char right);

// Main Function

int main(int argc, char*argv[]){

	int fd=0;
	char buf[50];
	int rc, n;
	int cont=0;
	int sens=1;

	fd=serialport_init(DEVICE_FILENAME, BAUDRATE);
	
	if(fd==-1){
		printf("Error opening file descriptor\n");
		exit(2);
	}
	if(uinput_mouse_init()<0){
		printf("Error opening uinput file\n");
		printf("Try to change the /dev/uinput file privileges\n");
		printf("For example: sudo chmod 777 /dev/uinput\n");
		exit(2);
	}
	
	usleep(3000*1000);	//	3 seconds
	
	while(1){
		
		//	Start reading
		n = serialport_read_until(fd, buf, ':', 50, 4);	// (-2 means timeout, -1 means could not open, 0 means data received ok)
		rc = process_received_data(buf);
		if(n==0 && rc==0)
			printf("read [%d]: [%s]\n", n, buf);	//	Debug
		
		if (cont==sens){
			uinput_mouse_move_cursor(buf[0],buf[1]);
			cont = 0;
		}
		cont++;

		usleep(5*1000);
			
	}
	
	close(fd);
	exit(EXIT_SUCCESS);  
}

int process_received_data(char *buf){
	for(int i=0; i<50;i++){
		if(buf[i]==':'){
			buf[i]='\0';
			buf[i+1]='\0';
		}
	}
	
	if(buf[0]=='a'||buf[0]=='e' && buf[1]=='a'||buf[1]=='e')
		return 0;
	else{
		buf[0]='a';		//	If data from arduino is wrong, the program correct it 
		buf[1]='a';
		return -1;
	}

}

int uinput_mouse_init(void){
	
	struct uinput_user_dev dev;
	int i;

	uinput_fd = open("/dev/uinput", O_WRONLY);
	
	if (uinput_fd <= 0) {
		perror("Error opening the uinput device\n");
		return -1;
	}
	
	memset(&dev,0,sizeof(dev)); 	// Intialize the uinput device to NULL
	strncpy(dev.name, "mouse", UINPUT_MAX_NAME_SIZE);
	dev.id.version = 4;
	dev.id.bustype = BUS_USB;
	dev.id.product = 1;
	ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
	ioctl(uinput_fd, UI_SET_EVBIT, EV_REL);
	ioctl(uinput_fd, UI_SET_RELBIT, REL_X);
	ioctl(uinput_fd, UI_SET_RELBIT, REL_Y);
	ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MOUSE);
	ioctl(uinput_fd, UI_SET_KEYBIT, BTN_LEFT);
	ioctl(uinput_fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MIDDLE);

	for (i=0; i<256; i++) {
		ioctl(uinput_fd, UI_SET_KEYBIT, i);
	}
	
	write(uinput_fd, &dev, sizeof(dev));
	
	if (ioctl(uinput_fd, UI_DEV_CREATE)){
		printf("Unable to create UINPUT device.");
		return -1;
	}
	
	return 1;
	
}

void uinput_mouse_move_cursor(char left, char right){
	
	int dir=0;
	struct input_event event; // Input device structure

// Get movement direction

	if(left=='e')
		dir=-1;
	else if(right=='e')
		dir=1;

	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_REL;
	event.code = REL_X;
	event.value = dir;
	write(uinput_fd, &event, sizeof(event));
	event.type = EV_REL;
	event.code = REL_Y;
	event.value = 0;
	write(uinput_fd, &event, sizeof(event));
	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(uinput_fd, &event, sizeof(event));
	
}


