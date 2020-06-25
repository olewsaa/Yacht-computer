

/*
 * 
 * Seatalk data test
 * 
 * Demo sketch for testing receiving of Seatalk data. Uses SoftwareSerial to read 9-bit words  
 * from Seatalk devices.
 * 
 * Ole W. Saastad,
 * June 25th 2020.
 * 
 * 
 */


/*
 * 
 * About SoftwareSerial.
 * On ESP8266:
 * At 80MHz runs up 57600ps, and at 160MHz CPU frequency up to 115200bps with only negligible errors.
 * Connect pin 12 to 14 for loopback testing.
 *
 * 
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
 * swSerial.begin(BAUD_RATE, D5, D6, SWSERIAL_9N1, false, 95, 11); 
 * Pin paramaters BEFORE the world lenght, parity and stop bits.
 * 
 * This testing is initial work in order to read 9-bit data from the Raymarine Seatalk data.
 * 
 */

#define DEBUG  // Simple debug info
#define DEBUG2 // Extended debug info 

#include <SoftwareSerial.h>

 
#define D5 (14)   // D5 is the D5 pin on the board, also named GPIO 14, hence (14) which reflects the GPIO14
#define D6 (12)   // D6 is the D6 pin on the board, also named GPIO 12, hence (12) which reflects the GPIO12

// Create a pointer to a SoftwareSerial object. 
SoftwareSerial SwSerial;



  
void setup() {
 
/*
 * Set up the Serial port and the Software Serial port to 9 bits and Serial for logging.
 * 
 */ 

  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
#ifdef DEBUG    
  Serial.begin(115200);         // Setup Serial port speed, could probably do more
#endif                          // as upload can run at 2 or 4x this speed. 

  // Set up SoftwareSerial.  
  // Baud rate 4800 (Seatalk), Rx pin D5, Tx pin D6, 9 bits word, No parity, 1 stop bit. Rest is std.
  //SwSerial.begin(4800, D5, D6, SWSERIAL_9N1, false, 95, 11); // This did not work.
  SwSerial.begin(4800, D5, D6, SWSERIAL_9N1); // Using the defaults seems to work.

// The command bit is 0x01zz where zz are nibbles or byte with data. The command bit is the 9t bit.
// This is the reason for having to use 9 bits. Some have used the Parity as way of detecting the 9th bit. 

#ifdef DEBUG
  Serial.println("\nSoftware serial test started\n");
#endif
} // End setup



// Empty function, just a dummy to handle calls to Send_to_SignalK,
// but prints out text and values is simple DEBUG is on.

void Send_to_SignalK(String key, double value){
#ifdef DEBUG
  Serial.print(key); Serial.print(": "); Serial.println(value);
#endif
  return;
}
/*
 * Functions to handle the different Seatalk messages based on the message hex code receivced.
 * 
 * This code is not tested on real Seatalk data yet.
 * 
 */


// Apparent wind speed 
/*     
 *  Apparent Wind Speed (AWS): 11 01 XX 0Y
 *  (XX & 0x7F) + YY/10 = speed
 *  XX&0x80=0           = knots (default)
 *  XX&0x80=0x80        = meters/second
 *  
 *  11  01  XX  0Y  Apparent Wind Speed: (XX & 0x7F) + Y/10 Knots
 *                Units flag: XX&0x80=0    => Display value in Knots
 *                            XX&0x80=0x80 => Display value in Meter/Second
 *                Corresponding NMEA sentence: MWV
*/
int aws(){
  unsigned int d;
  char c1, c2;
  float awsp;
  float corr;
  
  d=SwSerial.read();
#ifdef DEBUG2
  Serial.write("d HEX "); Serial.println(d, HEX);
  if (d == 0x0001) Serial.println("Wind speed : ");
#endif
  d=SwSerial.read();
  corr = (d & 0x80)?1.852:1.0;
  c1 = lowByte((d & 0x7f));
#ifdef DEBUG2
  Serial.write("d  HEX "); Serial.println(d, HEX);
  Serial.write("c1 HEX ");Serial.println(c1, HEX);
  Serial.write("corr "); Serial.println(corr);
#endif
  d=SwSerial.read();
  c2=lowByte(d);
#ifdef DEBUG2
  Serial.write("c2 HEX "); Serial.println(c2, HEX);
#endif   
  awsp=((float)c1+((float)c2)/10.0)/corr;
#ifdef DEBUG2
  Serial.println(corr);
  Serial.write("AWS : ");
  Serial.println(awsp);
#endif
  Send_to_SignalK("environment.wind.speedApparent",awsp);
}


/*    
 *  Apparent Wind Angle (AWA): 10 01 XX YY
 *  
 *  10  01  XX  YY  Apparent Wind Angle: XXYY/2 degrees right of bow
 *               Used for autopilots Vane Mode (WindTrim)
 *               Corresponding NMEA sentence: MWV
 *  
*/
int awa() {
  unsigned int d;
  char c1, c2; 
  int awang;
  
  d=SwSerial.read();
#ifdef DEBUG2
  Serial.println(d, HEX);
  if (d == 0x0001) Serial.println("Wind angle : ");
#endif
  d=SwSerial.read();
  c1=lowByte(d);
#ifdef DEBUG2  
  Serial.println(c1, HEX);  
#endif
  d=SwSerial.read();
  c2=lowByte(d);
#ifdef DEBUG2  
  Serial.println(c2, HEX);
#endif 
  awang=makeWord(c1,c2)/2;
#ifdef DEBUG2
  Serial.println(awang);
#endif
  Send_to_SignalK("environment.wind.angleApparent",awang);
}


/*    
 *    Depth below Transducer (DBT): 00 02 YZ XX XX   
 *    Y = 0 for feet (default), 4 for meters
 *    Z = 1 for shallow depth alarm, 4 for defective transducter
 *    XXXX = depth*10
 *    
 *    00  02  YZ  XX XX  Depth below transducer: XXXX/10 feet
 *                     Flags in Y: Y&8 = 8: Anchor Alarm is active
 *                                Y&4 = 4: Metric display units or
 *                                         Fathom display units if followed by command 65
 *                                Y&2 = 2: Used, unknown meaning
 *                    Flags in Z: Z&4 = 4: Transducer defective
 *                                Z&2 = 2: Deep Alarm is active
 *                                Z&1 = 1: Shallow Depth Alarm is active
 *                  Corresponding NMEA sentences: DPT, DBT
 */ 
int dbt() {
  unsigned int d;
  char c1, c2; 
  float dbt, offset=0.6; // Set the offset as needed. 
  
  int meters;
  d=SwSerial.read();
#ifdef DEBUG2
  Serial.println(d, HEX);
  if (d == 0x0002) Serial.println("Depth : ");
#endif
  d=SwSerial.read();
#ifdef DEBUG2
  Serial.println((d & 0x00F0), HEX);
#endif
  meters=((d & 0x00F0) == 0x40);
#ifdef DEBUG2
  Serial.write("Check for meters ");
  Serial.print(d, HEX);
  if (meters) Serial.println("Meters"); else Serial.println("Feet");;
#endif  
  //Read the data XX XX 
  d=SwSerial.read(); 
#ifdef DEBUG2 
  Serial.println(d);
#endif  
  c1=lowByte(d);
#ifdef DEBUG2
  Serial.println(c1, HEX);
#endif
  d=SwSerial.read();
#ifdef DEBUG2
  Serial.println(d, HEX);
#endif
  c2=lowByte(d);
#ifdef DEBUG2
  Serial.println(c2, HEX);
#endif
  d=makeWord(c1,c2);
#ifdef DEBUG2
  Serial.print("Makeword, d : "); 
  Serial.println(d);
#endif      
  if (meters) dbt = (int)d/(10000.0*3.28); else dbt=(int)d/10000.0;
  dbt+=offset; // offset added. 
#ifdef DEBUG2
  Serial.println(dbt);
#endif
  Send_to_SignalK("environment.depth.belowTransducer",dbt);
}


/*
 * Water temperature (WTEMP)
 * WTEMP = (x - 100)/10
 * 
 * 27  01  XX  XX  Water temperature: (XXXX-100)/10 deg Celsius
 *                Corresponding NMEA sentence: MTW  
 *  
 */
int wtemp() {
  unsigned int d;
  char c1, c2;
  int te;
  float wtemp;
  
  d=SwSerial.read(); // Read the 01.
#ifdef DEBUG2
  Serial.print("Water Temperature : ");
  Serial.println(d, HEX);
#endif
  // Read the data XX XX
  d=SwSerial.read();
  c1=lowByte(d);
#ifdef DEBUG2  
  Serial.println(d, HEX);
  Serial.println(c1,HEX);
#endif    
  d=SwSerial.read();
  c2=lowByte(d);
#ifdef DEBUG2  
  Serial.println(d, HEX);
  Serial.println(c1,HEX);
#endif  
  d=makeWord(c1,c2); te=d;
  wtemp=(float)te/1000;
#ifdef DEBUG2
  Serial.println(d, HEX);
  Serial.println(te);
  Serial.println(wtemp);
#endif
  Send_to_SignalK("environment.water.temperature",wtemp);
}


/*
 * Speed through Water (STW): 20 01 XX XX
 * XXXX = speed*10
 * 
 * 20  01  XX  XX  Speed through water: XXXX/10 Knots
 *                Corresponding NMEA sentence: VHW
 */
int stw() { 
  unsigned int d;
  char c1, c2;
  float stw;
  
  d=SwSerial.read(); // Read the 01.
#ifdef DEBUG2
  Serial.println(d, HEX);
  Serial.write("Speed through water : ");
#endif
  Serial.println(d, HEX);// Read the data XX XX
  d=SwSerial.read();
  c1=lowByte(d);
#ifdef DEBUG2  
  Serial.println(d, HEX);
  Serial.println(c1,HEX);
#endif  
  d=SwSerial.read();
  c2=lowByte(d);
#ifdef DEBUG2  
  Serial.println(d, HEX);
  Serial.println(c2,HEX);
#endif 
  d=makeWord(c1,c2); stw=((float)d/10.0); 
#ifdef DEBUG2
  Serial.println(d,HEX);
  Serial.println(stw);
#endif
  Send_to_SignalK("navigation.speedThroughWater",(stw/1.852)); // SignalK is in m/s.
}




/*
 * Main loop start here, this is the endless loop.
 */

void loop() {
  unsigned int d;
  char c1, c2;

  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on while we transfer the data.
#ifdef DEBUG
  Serial.println("void loop top");
#endif  
  while (SwSerial.available() > 0) {
    d=SwSerial.read();
#ifdef DEBUG2
    Serial.println(d, HEX);
    Serial.println((d & 0x0100), HEX);
#endif 
    if ((d & 0x0100) == 0) {  // Check if the command bit is set, if not just enter one more cycle.
#ifdef DEBUG 
      Serial.println("Error no command bit set");
#endif 
      continue; 
    }  // if comand bit is not set, this is the 9th bit. 

    // We have a valid command, or at least hope so.  
    d=(d & 0x00FF); // Strip off the comand bit. Only the least significant 8 bits in 
                    // the variable d (16 bits uint) are left.
                    // Another option is to just use lowByte(d) to have only the 8 last bits. 
#ifdef DEBUG2
    Serial.println(d, HEX);
    Serial.println(lowByte(d), HEX); // The two should be identical.
#endif
    // Could have used a switch statement here.
    // Apparent wind speed 
    if (d == 0x0011) aws();
    
    // Apparent wind angle
    if (d == 0x0010) awa();

    // Depth
    if (d == 0x0000) dbt();

    // Speed through water
    if (d == 0x0020) stw();

    // Water temperature
    if (d == 0x0027) wtemp();

    yield(); // Let other tasks like wifi/tcp etc do their work.
             //  The ESP8266 is single core, we do not have the luxery of multicore as the ESP32.
  } // End while loop
  
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH.
  delay(1000);
  
}  // End loop 
