// On ESP8266:
// At 80MHz runs up 57600ps, and at 160MHz CPU frequency up to 115200bps with only negligible errors.
// Connect pin 12 to 14.

/*
 * Test for SoftwareSerial (to get en extra serial port on the ESP8266. 
 * This example is part if initial testing using a SoftwareSerial 9 bit implementation.
 * https://github.com/ionini/espsoftwareserial Downloaded and installed as under : ~/Arduino/libraries
 * EspSoftwareSerial -> EspSoftwareSerial-9bit/
 * EspSoftwareSerial-9bit
 * EspSoftwareSerial-orig
 *
 * An additional change needed to be applied in the file EspSoftwareSerial/src/SoftwareSerial.cpp :
 *  //attachInterruptArg(digitalPinToInterrupt(m_rxPin), (void (*)())(rxBitISR), this, CHANGE);
 *  attachInterruptArg(digitalPinToInterrupt(m_rxPin), reinterpret_cast<void (*)(void*)>(rxBitISR), this, CHANGE);
 * 
 * An error in the casting of void function and viod parameters. Using the same line from the initial
 * 8 bit version worked. 
 * 
 * The calling sequence is different in the original 8 bit versions and the extened 9 bit version.
 * The 9 bit version looks like this: 
 * swSer.begin(BAUD_RATE, D5, D6, SWSERIAL_9N1, false, 95, 11); 
 * Pin paramaters BEFORE the world lenght, parity and stop bits.
 * 
 * This testing is initial work in order to read 9-bit data from the Raymarine Seatalk data.
 * 
 */



#include <SoftwareSerial.h>

#if defined(ESP8266) && !defined(D5)
#define D5 (14)   // D5 is the D5 pin on the board, also named GPIO 14, hence (14) which reflects the GPIO14
#define D6 (12)   // D6 is the D6 pin on the board, also named GPIO 12, hence (12) which reflects the GPIO12
#define D7 (13)
#define D8 (15)
#endif

#ifdef ESP32
#define BAUD_RATE 57600
#else
#define BAUD_RATE 4800
#endif

// Reminder: the buffer size optimizations here, in particular the isrBufSize that only accommodates
// a single 8N1 word, are on the basis that any char written to the loopback SoftwareSerial adapter gets read
// before another write is performed. Block writes with a size greater than 1 would usually fail. 
SoftwareSerial swSer;

void setup() {
	Serial.begin(115200);
	swSer.begin(BAUD_RATE, D5, D6, SWSERIAL_9N1, false, 95, 11);

	Serial.println("\nSoftware serial test started");

	for (char ch = ' '; ch <= 'z'; ch++) {
		swSer.write(ch);
    Serial.write(ch);
	}
	swSer.write(makeWord(' ','\n'));
  Serial.println("\nData written to SoftwareSerial port");
}


void loop() {
  	while (swSer.available() > 0) {
    unsigned int d; 
    //Serial.write(swSer.peek());
    d=swSer.read();
    Serial.write(d);
		// Serial.write(swSer.read());
		yield();
	}
	while (Serial.available() > 0) {
		swSer.write(Serial.read());
		yield();
	}
}
