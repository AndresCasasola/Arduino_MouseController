/*

* Arduino mousecontroller
----------------------------------------------------------

* A program that gets values from photoresistors and send them

* to the computer by serial interface with "Serial.h".

* This example/application use photoresistors and visible light leds to 

* sense the movement of the hand.
----------------------------------------------------------

* Created 28 December 2017

* Copyleft (c) 2017, Andres Casasola Dominguez
----------------------------------------------------------

* Last update: March 2018

*/

#define BAUDRATE 9600
#define SENSOR_1 A0			    //	Analogic pins for analog readings
#define SENSOR_2 A1
#define MAX_DATA_BYTES 50	  //	Max number of bytes

char data[MAX_DATA_BYTES];
int s1_value, s2_value;		//	Sensor variable value
int s1_calibrate_value, s2_calibrate_value;
char s1_data, s2_data;

void uC_config_init();
void calibrate_sensors();
void process_sensors_data();
void debug_1();
void debug_2();

void setup(){
  	
  	uC_config_init();
  	calibrate_sensors();
    Serial.println(" ");
    Serial.println("Starting...");
    
    //debug_1();	// Uncomment to debug
    
  	delay(500);
}

void loop(){

  s1_value=analogRead(SENSOR_1);
  s2_value=analogRead(SENSOR_2);
  
  process_sensors_data();
  
  data[0]=s1_data;
  data[1]=s2_data;
  data[2]=':';    	//  Msg end
  data[3]='\0';
  
  Serial.print(data);
  
  //debug_2();		// Uncomment to debug

  delay(200);
  
}

void uC_config_init(){
	
	Serial.begin(BAUDRATE);
  	pinMode(13, OUTPUT);
  	pinMode(SENSOR_1, INPUT);
  	pinMode(SENSOR_2, INPUT);

}

void calibrate_sensors(){

	int repeat=20;
    int s1=0;
    int s2=0;
    
	for(int i=0; i<repeat; i++){
    	
    	s1_value=analogRead(SENSOR_1);
        s2_value=analogRead(SENSOR_2);		
        s1 += s1_value;
		s2 += s2_value;
	}
	
	s1_calibrate_value = int(s1/repeat);
	s2_calibrate_value = int(s2/repeat);  

}

void process_sensors_data(){
       
        if(s1_value > s1_calibrate_value+30)    // Sensibility 30
          s1_data='e';    //  On
        else
          s1_data='a';    //  Off
          
        if(s2_value > s2_calibrate_value+30)
          s2_data='e';    //  On
        else
          s2_data='a';    //  Off
          
}

void debug_1(){

	Serial.println("Calibrate values");
    Serial.print("S1:    ");
    Serial.println(s1_calibrate_value);
    Serial.print("S2:    ");
    Serial.println(s2_calibrate_value);

}

void debug_2(){

  Serial.println("Data:");
  Serial.print("Sensor 1:    ");
  Serial.print(s1_value);
  
  if(s1_data=='e')
    Serial.println("  On");
  else
    Serial.println("  Off");
  
  Serial.print("            Sensor 2:    ");
  Serial.print(s2_value);
  
  if(s2_data=='e')
    Serial.println("  On");
  else
    Serial.println("  Off");

}



