#define SERIAL_BAUD_RATE 19200 // 115200 // 19200
#include <stdio.h> // sprintf

void setup() {
// #define SERIAL_CONFIG SERIAL_8N1 // default
// Serial.begin(SERIAL_BAUD_RATE, SERIAL_CONFIG);
		Serial.begin(SERIAL_BAUD_RATE);
		Serial1.begin(SERIAL_BAUD_RATE);

		Serial.print("STARTUP OK : )\r\n");
}

void loop() {
		// if tx1/rx1 is available
		if(Serial1.available()){
				int inByte = Serial1.read();
				Serial.write(inByte);
		}

		// if tx0/rx0 is available
		if(Serial.available()){
				int inByte = Serial.read();
				Serial1.write(inByte);
		}

}
