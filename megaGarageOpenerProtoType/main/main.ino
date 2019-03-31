#define SERIAL_BAUD_RATE 115200 // 19200
#include <stdio.h> // sprintf

#define GARAGE_OPENER_PIN 7
#define GARAGE_OPENER_DELAY_MS 500 // how long to hold garage opener button for
#define GARAGE_SENSOR_PIN 8

volatile unsigned int garage_open_s = 0; // how long the garage has been open for
volatile unsigned int lastMillis = 0;
volatile unsigned int mode = 0; // 0=monitor, 1=open attempt to close
volatile unsigned int last_click_at_s = 0; // when the last time the clicker was clicked relative to how long the garage has been open

void setup() {
// #define SERIAL_CONFIG SERIAL_8N1 // default
// 		Serial.begin(SERIAL_BAUD_RATE, SERIAL_CONFIG);
// 		Serial1.begin(SERIAL_BAUD_RATE, SERIAL_CONFIG);
		Serial.begin(SERIAL_BAUD_RATE);
		Serial1.begin(SERIAL_BAUD_RATE);

		// Garage Opener
		pinMode(GARAGE_OPENER_PIN, OUTPUT);
		digitalWrite(GARAGE_OPENER_PIN, LOW);

		// Garage Sensor
		pinMode(GARAGE_SENSOR_PIN, INPUT);
		digitalWrite(GARAGE_SENSOR_PIN, HIGH);

		Serial.print("STARTUP OK : )\r\n");
}

// hold_ms how long to hold garage button down for
void clickGarageOpener(int hold_ms) {
		digitalWrite(GARAGE_OPENER_PIN, HIGH);
		delay(hold_ms);
		digitalWrite(GARAGE_OPENER_PIN, LOW);
		last_click_at_s = garage_open_s;
}

boolean isGarageOpen(void) { return digitalRead(GARAGE_SENSOR_PIN); }

void updateGarageOpenTimer(){
		// this is a horible busy poll/update of now
		unsigned int curMillis = millis();
		if( curMillis < lastMillis ){
				garage_open_s += (65335-lastMillis + curMillis)/1000;
				lastMillis = curMillis;
		} else {
				unsigned int delta = (curMillis - lastMillis)/1000;
				if(delta>0){
						lastMillis = curMillis;
						garage_open_s += delta;
				}
		}
}

// todo make these constants
unsigned int close_garage_after_s = 6;
unsigned int garage_close_time_s = 3; // should be 3x actual garage close time

#define STATUS_GARAGE_CLOSED 0 // is closed
#define STATUS_GARAGE_OPEN_TOOLONG 1 // open for longer than `close_garage_after_s`
unsigned int g_status = STATUS_GARAGE_CLOSED;

void  garage_loop(){
		char buf[360];
		sprintf(buf, "[%u]\r\n", garage_open_s);
		Serial.print(buf);

		if(isGarageOpen()){
				updateGarageOpenTimer();
				if (garage_open_s >= close_garage_after_s ){
						if( g_status == STATUS_GARAGE_CLOSED ){
								// first time we've detected garage has been open
								// beyond acceptable amount of time
								g_status=STATUS_GARAGE_OPEN_TOOLONG;
								// close garage
								clickGarageOpener(GARAGE_OPENER_DELAY_MS);
								// delay(garage_close_time_s*1000); // TODO: this wait is busy wait - bad for if GPRS is enabled
						} else if ( g_status == STATUS_GARAGE_OPEN_TOOLONG){
								sprintf(buf, "[go] = %u [lca] = %u", garage_open_s, last_click_at_s);
								Serial.print(buf);
								if( garage_open_s - last_click_at_s > garage_close_time_s){
										clickGarageOpener(GARAGE_OPENER_DELAY_MS);
								}

								Serial.print('.');
						}
				}
				updateGarageOpenTimer(); // again in case of the delay
		} else {
				if (g_status == STATUS_GARAGE_OPEN_TOOLONG ) {
						Serial.print("Closed!\r\n");
				}
				g_status = STATUS_GARAGE_CLOSED;
				garage_open_s = 0;
				lastMillis = millis();
		}
}

void sendSMS(char *num, char*msg){
		char str[360]; // 160 should be max or is 180? idk... idc
		Serial1.print("AT+CMGF=1\r"); //sms mode
		delay(100);

		// send to number
		sprintf(str, "AT+CMGS=\"%s\"\r", num);
		Serial1.print(str);
		delay(100);

		sprintf(str, "%s\r\n", msg);
		Serial1.print(str);
		delay(100);

		Serial1.print((char)26); // 26 is Ctrl-Z to end
}

#define Serial900 Serial1

void initSim900(){
		Serial900.print("AT");
}

void atInfo(){
// 		Serial1.write("AT\r\n");
// 		Serial1.write("ATI\r\n"); // model num
// 		Serial1.write("AT+CGMI\r\n"); // manufacturer specific identity
// 		Serial1.write("AT+CGMM\r\n"); // manufacturer specific model identity
// 		Serial1.write("AT+CGM\r\n"); // manufacturer specific model identity
// 		Serial1.write("AT+CPIN?\r\n"); // check pin
//		Serial1.write("AT+CCID?\r\n"); // check SIM ID
// 		Serial1.write("AT+CRSM=176,28536,0,0,12\r\n"); //Reading Forbidden PLMNs - Identifier is 28536
// 		Serial1.write("AT+CRSM=176,28539,0,0,12\r\n"); //not sure what this is ?
// 		Serial1.write("AT+CGDCONT?\r\n"); // Read PDP context's  parameters (APN)
// 		Serial1.write("AT+CGACT?\r\n"); // PDP context
// 		Serial1.write("AT+COPS?\r\n"); // network status
// 		Serial1.write("AT+CSQ?\r\n"); // signal quality

}


void loop() {
		// if tx1/rx1 is available
		if(Serial1.available()){
				int inByte = Serial1.read();
				Serial.write(inByte);
		}

		if(Serial.available()){
				int inByte = Serial.read();

				if (inByte == '>'){
						Serial1.write('\n');
				} else if (inByte == '~'){
						atInfo();
				} else if (inByte == '<') {
						Serial1.write((char)26);
// 				} else if (inByte == 'Z'){
//
// 						Serial1.print("AT+CMGF=1\r");
// 						delay(100);
// 						Serial1.println("AT + CMGS = \"+1515555555\"");
// 						delay(100);
// 						Serial1.println("Hello, world. This is a text message from an Arduino Uno.");
// 						delay(100);
// 						Serial1.println((char)26); // End AT command with a ^Z, ASCII code 26
// 						delay(100);
// 						Serial1.println();
				} else if (inByte == '`'){
						sendSMS("2936", "Hey this is a test of the sprintf function : )");
				} else if (inByte == '%'){
						sendSMS("+5155555555", "Hurry UP ! ! !, sent from ARDUINO");
				} else if (inByte == '^'){
						//^ is 6 is G
						clickGarageOpener(GARAGE_OPENER_DELAY_MS);
				} else if (inByte == '('){
						//( is ( is (heck
						char str[360];
						sprintf(str, "garage is %s\r\n", (isGarageOpen()?"open":"closed"));
						Serial1.print(str);
				} else {
						Serial1.write(inByte);
				}
		}

// 		// 15 secos
// 		if(isGarageOpen() && (millis()-lastCheck) > 6000){
// 				Serial.write("uh oh garage is open :'-(\r\n");
// 				clickGarageOpener(GARAGE_OPENER_DELAY_MS);
// 				lastCheck = millis();
// 		}

}
