#include <Arduino.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define ONBOARD_LED  2

using namespace std;

//Drone Lab: NETGEAR05
//Drone Lab: rapidroad510
//Home: SETUP-125B
//Home: fever6602answer
const char* ssid = "NETGEAR05"; 
const char* password =  "rapidroad510"; 

float sys_time;

//for v_data, [current, v-1, v-2]
//packed array in struct isn't accessible to ISR
double v_data[3][6]= {0.00,0.00,0.00,0.00,0.00,0.00,
					  0.00,0.00,0.00,0.00,0.00,0.00,
					  0.00,0.00,0.00,0.00,0.00,0.00
					  };

// array for storing position data via incrementing count
uint32_t p_data[1][6] = {0,0,0,0,0,0};

//array for storing position in radians, [r0: most recent, r1: mr-1, r2: mr-2]
double pr_data[3][6] = {0.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00
						};

//array for storing the timestamp of the current and previous frame of each ISR 
double ts_data[3][6] = {0.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00
						};

//new packet data
struct __attribute__((__packed__))data{

uint32_t timestamp = 0;
uint32_t index = 0;
uint32_t extremely_important_data = 0;
//motor 1
double counter_1 =  0;
double revolutions_1 = 0;
double recent_ts_1 = 0;
double av_1 = 0;
//motor 2
double counter_2 =  0;
double revolutions_2 = 0;
double recent_ts_2 = 0;
double av_2 = 0;
//motor 3
double counter_3 =  0;
double revolutions_3 = 0;
double recent_ts_3 = 0;
double av_3 = 0;
//motor 4
double counter_4 =  0;
double revolutions_4 = 0;
double recent_ts_4 = 0;
double av_4 = 0;
//motor 5
double counter_5 =  0;
double revolutions_5 = 0;
double recent_ts_5 = 0;
double av_5 = 0;
//motor 6
double counter_6 =  0;
double revolutions_6 = 0;
double recent_ts_6 = 0;
double av_6 = 0;
} packetv2 ;

const int port = 2222;

//192.168.0.74 home ip
//school ip 192, 168, 1, 6
//make sure to change this
uint8_t ip[] = {192, 168, 1, 6};

//what I need to read from the UDP source
char buff [255];

//without this, parsepacket won't work(if I want to read locally)
WiFiUDP Udp;

//function for getting angular velocity from position and passing it through a low pass filter
//for each motor i will need create an array of size 3 for its velocity and position
//uint32 for storing the count. every time th ecount increments by 1, add 1/12 of a revolution

double angVfromP(double position_x, double position_x_2, double velocity_1, double velocity_2, double current_ts) {
	
	int z = 1; //dampning
	int w = 1; //was 10 in the paper
	double current_v = 0; // where the current velocity will be stored

	//equation for finding the velocity from position
	current_v = ((2*current_ts*w*w*position_x)- (2*current_ts*w*w*position_x_2) -(4-4*current_ts*z*w+current_ts*current_ts*w*w)*velocity_2 -(2*current_ts*current_ts*w*w-8)*velocity_1)/ 
	(4+4*current_ts*z*w + current_ts*current_ts*w*w);

	return current_v;
};

//need IRAM_ATTR to give interrupt routines access to memory

//printing inside of the interrupt routine will cause it to time out
void IRAM_ATTR R1(){

	//Interrupt routine for pin 1
	//timestamp for data transmission
	packetv2.recent_ts_1 = micros();
	//count revolutions
	packetv2.counter_1++;
	if (packetv2.counter_1 > 11){
		packetv2.revolutions_1++;
		packetv2.counter_1 = 0;
	}
	//fill pr_data with incrementing distance traveled in radians (tested: works)
	pr_data[2][0] = pr_data[1][0];
	pr_data[1][0] = pr_data[0][0];
	p_data[0][0]++;
	pr_data[0][0]=p_data[0][0]*((2*3.141)/7); //was 6

	//find the distance in seconds between interrupt pulses
	//ESP32 Interrupt priority messes up the millis timer!!!!!!!!!
	//millis and micros don't increment inside of an interrupt
	ts_data[2][0] = ts_data[1][0];
	ts_data[1][0] = (esp_timer_get_time());
	ts_data[0][0] = ((ts_data[1][0]- ts_data[2][0])/1000000.00);
	//ts_data[0][0] = ts_data[0][1]-ts_data[0][2] ;
	
	//increment through the stored velocity array
	v_data[2][0] = v_data[1][0];
	v_data[1][0] = v_data[0][0];
	v_data[0][0] = angVfromP(pr_data[0][0],pr_data[2][0],v_data[1][0],v_data[2][0], ts_data[0][0]);	
	
}
void IRAM_ATTR R2(){
	//Interrupt routine for pin 2
	packetv2.recent_ts_2 = micros();
	packetv2.counter_2++;
	if (packetv2.counter_2 > 11){
		packetv2.revolutions_2++;
		packetv2.counter_2 = 0;
	}
	//fill pr_data with incrementing distance traveled in radians (tested: works)
	pr_data[2][1] = pr_data[1][1];
	pr_data[1][1] = pr_data[0][1];
	p_data[0][1]++;
	pr_data[0][1]=p_data[0][1]*((2*3.141)/7);

	//find the distance in seconds between interrupt pulses
	//ESP32 Interrupt priority messes up the millis timer!!!!!!!!!
	//millis and micros don't increment inside of an interrupt
	ts_data[2][1] = ts_data[1][1];
	ts_data[1][1] = (esp_timer_get_time());
	ts_data[0][1] = ((ts_data[1][1]- ts_data[2][1])/1000000.00);
	//ts_data[0][0] = ts_data[0][1]-ts_data[0][2] ;
	
	//increment through the stored velocity array
	v_data[2][1] = v_data[1][1];
	v_data[1][1] = v_data[0][1];
	v_data[0][1] = angVfromP(pr_data[0][1],pr_data[2][1],v_data[1][1],v_data[2][1], ts_data[0][1]);	

}
void IRAM_ATTR R3(){
	//Interrupt routine for pin 3
	packetv2.recent_ts_3 = micros();
	packetv2.counter_3++;
	if (packetv2.counter_3 > 11){
		packetv2.revolutions_3++;
		packetv2.counter_3 = 0;
	}
	//fill pr_data with incrementing distance traveled in radians (tested: works)
	pr_data[2][2] = pr_data[1][2];
	pr_data[1][2] = pr_data[0][2];
	p_data[0][2]++;
	pr_data[0][2]=p_data[0][2]*((2*3.141)/7);

	//find the distance in seconds between interrupt pulses
	//ESP32 Interrupt priority messes up the millis timer!!!!!!!!!
	//millis and micros don't increment inside of an interrupt
	ts_data[2][2] = ts_data[1][2];
	ts_data[1][2] = (esp_timer_get_time());
	ts_data[0][2] = ((ts_data[1][2]- ts_data[2][2])/1000000.00);
	//ts_data[0][0] = ts_data[0][1]-ts_data[0][2] ;
	
	//increment through the stored velocity array
	v_data[2][2] = v_data[1][2];
	v_data[1][2] = v_data[0][2];
	v_data[0][2] = angVfromP(pr_data[0][2],pr_data[2][2],v_data[1][2],v_data[2][2], ts_data[0][2]);	
}
void IRAM_ATTR R4(){
	//Interrupt routine for pin 4
	packetv2.recent_ts_4 = micros();
	packetv2.counter_4++;
	if (packetv2.counter_4 > 11){
		packetv2.revolutions_4++;
		packetv2.counter_4 = 0;
	}
	//fill pr_data with incrementing distance traveled in radians (tested: works)
	pr_data[2][3] = pr_data[1][3];
	pr_data[1][3] = pr_data[0][3];
	p_data[0][3]++;
	pr_data[0][3]=p_data[0][3]*((2*3.141)/7);

	//find the distance in seconds between interrupt pulses
	//ESP32 Interrupt priority messes up the millis timer!!!!!!!!!
	//millis and micros don't increment inside of an interrupt
	ts_data[2][3] = ts_data[1][3];
	ts_data[1][3] = (esp_timer_get_time());
	ts_data[0][3] = ((ts_data[1][3]- ts_data[2][3])/1000000.00);
	//ts_data[0][0] = ts_data[0][1]-ts_data[0][2] ;
	
	//increment through the stored velocity array
	v_data[2][3] = v_data[1][3];
	v_data[1][3] = v_data[0][3];
	v_data[0][3] = angVfromP(pr_data[0][3],pr_data[2][3],v_data[1][3],v_data[2][3], ts_data[0][3]);	
}
void IRAM_ATTR R5(){
	//Interrupt routine for pin 5
	packetv2.recent_ts_5 = micros();
	packetv2.counter_5++;
	if (packetv2.counter_5 > 11){
		packetv2.revolutions_5++;
		packetv2.counter_5 = 0;
	}
	//fill pr_data with incrementing distance traveled in radians (tested: works)
	pr_data[2][4] = pr_data[1][4];
	pr_data[1][4] = pr_data[0][4];
	p_data[0][4]++;
	pr_data[0][4]=p_data[0][4]*((2*3.141)/7);

	//find the distance in seconds between interrupt pulses
	//ESP32 Interrupt priority messes up the millis timer!!!!!!!!!
	//millis and micros don't increment inside of an interrupt
	ts_data[2][4] = ts_data[1][4];
	ts_data[1][4] = (esp_timer_get_time());
	ts_data[0][4] = ((ts_data[1][4]- ts_data[2][4])/1000000.00);
	//ts_data[0][0] = ts_data[0][1]-ts_data[0][2] ;
	
	//increment through the stored velocity array
	v_data[2][4] = v_data[1][4];
	v_data[1][4] = v_data[0][4];
	v_data[0][4] = angVfromP(pr_data[0][4],pr_data[2][4],v_data[1][4],v_data[2][4], ts_data[0][4]);	
}
void IRAM_ATTR R6(){
	//Interrupt routine for pin 6
	packetv2.recent_ts_6 = micros();
	packetv2.counter_6++;
	if (packetv2.counter_6 > 11){
		packetv2.revolutions_6++;
		packetv2.counter_6 = 0;
	}
	//fill pr_data with incrementing distance traveled in radians (tested: works)
	pr_data[2][5] = pr_data[1][5];
	pr_data[1][5] = pr_data[0][5];
	p_data[0][5]++;
	pr_data[0][5]=p_data[0][5]*((2*3.141)/7);

	//find the distance in seconds between interrupt pulses
	//ESP32 Interrupt priority messes up the millis timer!!!!!!!!!
	//millis and micros don't increment inside of an interrupt
	ts_data[2][5] = ts_data[1][5];
	ts_data[1][5] = (esp_timer_get_time());
	ts_data[0][5] = ((ts_data[1][5]- ts_data[2][5])/1000000.00);
	//ts_data[0][0] = ts_data[0][1]-ts_data[0][2] ;
	
	//increment through the stored velocity array
	v_data[2][5] = v_data[1][5];
	v_data[1][5] = v_data[0][5];
	v_data[0][5] = angVfromP(pr_data[0][5],pr_data[2][5],v_data[1][5],v_data[2][5], ts_data[0][5]);	
}

void setup() {
  //setup onboard LED
  pinMode(ONBOARD_LED,OUTPUT);
  //set the desired pins as input pins
  pinMode(13,INPUT);
  pinMode(12,INPUT);
  pinMode(14,INPUT);
  pinMode(27,INPUT);
  pinMode(26,INPUT);
  pinMode(25,INPUT);
  //attach interrupt pins
  attachInterrupt(digitalPinToInterrupt(13), R1, RISING);
  attachInterrupt(digitalPinToInterrupt(12), R2, RISING);
  attachInterrupt(digitalPinToInterrupt(14), R3, RISING);
  attachInterrupt(digitalPinToInterrupt(27), R4, RISING);
  attachInterrupt(digitalPinToInterrupt(26), R5, RISING);
  attachInterrupt(digitalPinToInterrupt(25), R6, RISING);
  //begin serial connection
  Serial.begin(115200);

  //wifi connection
  //iniate wifi connection //comment out while working on rpm data
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Udp.begin(port);

}

void loop() {
	//add an exponential filter
	//spoof data
	packetv2.timestamp = micros();
    packetv2.index++;
    packetv2.extremely_important_data = random(0,UINT16_MAX);

	//all timers freeze collecting data during interrupt, use sys time instead

	// 
	// packetv2.av_2 = v_data[0][1];
	// packetv2.av_3 = v_data[0][2];
	// packetv2.av_4 = v_data[0][3];
	// packetv2.av_5 = v_data[0][4];
	// packetv2.av_6 = v_data[0][5];

	Serial.println("v_data");
	
	Serial.println(v_data[0][0]);
	Serial.flush();
	packetv2.av_1 = v_data[0][0];
	
	Serial.println(v_data[0][1]);
	Serial.flush();
	packetv2.av_2 = v_data[0][1];

	Serial.println(v_data[0][2]);
	Serial.flush();
	packetv2.av_3 = v_data[0][2];

	Serial.println(v_data[0][3]);
	Serial.flush();
	packetv2.av_4 = v_data[0][3];

	Serial.println(v_data[0][4]);
	Serial.flush();
	packetv2.av_5 = v_data[0][4];

	Serial.println(v_data[0][5]);
	Serial.flush();
	packetv2.av_6 = v_data[0][5];

	// Serial.println("struct_data");
	
	// Serial.println(packetv2.av_2);
	// Serial.println(packetv2.av_3);
	// Serial.println(packetv2.av_4);
	// Serial.println(packetv2.av_5);
	// Serial.println(packetv2.av_6);
	
	// Serial.println("-----------------");

	// Send the data to remote machine. 
    Udp.beginPacket(ip,port);
    Udp.write((byte*)&packetv2, sizeof(data));
	Udp.endPacket();

}


		//function for Sending UDP data from the ESP32
	// bool udpSendMessage(IPAddress ipAddr, String udpMsg, int udpPort) {
	// 	/** WiFiUDP class for creating UDP communication */
	// 	WiFiUDP udpClientServer;

	// 	// Start UDP client for sending packetsSerial.println("UDP connection failed");
	// 	int connOK = udpClientServer.begin(udpPort);

	// 	int beginOK = udpClientServer.beginPacket(ipAddr, udpPort);

	// 	//technically the only needed section
	// 	int bytesSent = udpClientServer.print(udpMsg);
	// 	if (bytesSent == udpMsg.length()) {
			
	// 		//used to verify that the ESP32 is sending data to the UDP server
	// 		//Serial.println("Sent " + String(bytesSent) + " bytes from " + udpMsg + " which had a length of " + String(udpMsg.length()) + " bytes");
			
	// 		udpClientServer.endPacket();
	// 		udpClientServer.stop();
	// 		return true;

	// 	} 
	// }

	//code for sending UDP data
	//udpSendMessage(ip,packet,port);

	//use to find the current size of the struct
	//Serial.println(sizeof(packetv2));
	
	//coonvert struct data into a string to enable data transmit over UdpSendMessage
	// char pSend[255];
  	// sprintf (pSend, "old_ts:%lf, new_ts:%lf, rev 1:%lf, rev 2:%lf, rev 3:%lf, rev 4:%lf, rev 5:%lf, rev 6:%lf", 
	// packetv2.new_ts, packetv2.old_ts, packetv2.revolutions_1, packetv2.revolutions_2, packetv2.revolutions_3, packetv2.revolutions_4,
	// packetv2.revolutions_5,packetv2.revolutions_6);

	//it works!!
	// udpSendMessage(ip,pSend,port);

	//test printing out new packet, pSend
  	//printf ("%s \n",pSend);

//----------------------------------------------------------------
	//if I want to read data from wifi on the ESP32 instead of my Laptop
	//can't send and recieve at the same time
	//code for parsing the packet
	//int packetSize = Udp.parsePacket();
	//trying to figure out why my packet size is 0, 
	//Serial.println(packetSize);
// 	if (packetSize) {
//     Serial.print("Received packet of size ");
//     Serial.println(packetSize);
//     Serial.print("From ");
//     IPAddress remoteIp = Udp.remoteIP();
//     Serial.print(remoteIp);
//     Serial.print(", port ");
//     Serial.println(Udp.remotePort());
//     // read the packet into packetBufffer
//     int len = Udp.read(buff, 255);
//     if (len > 0) {
// 	  buff[len] = 0;
//     }
//     Serial.println("Contents:");
//     Serial.println(buff);	
// 	//removed writing an acknowledgement
// }
