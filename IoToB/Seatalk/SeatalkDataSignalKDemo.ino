

/*
 * 
 * Seatalk demo messages converted to SignalK over wifi.
 * 
 * Demo sketch for testing Seatalk to SignalK over wifi. Uses SoftwareSerial to read 9-bit words  
 * from Seatalk, this is demonstrated by a loopback connection from one pin to another.  Real emulated
 * Seatalk data is written, decoded converted to SignalK messaged and transmitted over wifi to a SignalK server.
 * 
 * Ole W. Saastad,
 * May 2020.
 * 
 * 
 */



/*
 * 
 * About SoftwareSerial.
 * On ESP8266:
 * At 80MHz runs up 57600ps, and at 160MHz CPU frequency up to 115200bps with only negligible errors.
 * Connect pin 12 to 14.
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

#define DEBUG

#include <SoftwareSerial.h>
// Including WiFi stuff - this is UDP and differ from TCP and also from ESP32 cards.
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>

 
#define D5 (14)   // D5 is the D5 pin on the board, also named GPIO 14, hence (14) which reflects the GPIO14
#define D6 (12)   // D6 is the D6 pin on the board, also named GPIO 12, hence (12) which reflects the GPIO12

// Reminder: the buffer size optimizations here, in particular the isrBufSize that only accommodates
// a single 8N1 word, are on the basis that any char written to the loopback SoftwareSerial adapter gets read
// before another write is performed. Block writes with a size greater than 1 would usually fail. 
SoftwareSerial SwSerial;


// Instanciate WiFi and UDP objects
ESP8266WiFiMulti WiFiMulti;
WiFiUDP Udp;
  

// WiFi network name and password 
const char * ssid = "openplotter";
const char * pwd = "12345678";

// IP address to send UDP data to.
// Settings for SignalK port and SignalK server.
const char * udpAddress     = "10.10.10.1";
const char * SignalK_server = "10.10.10.1";
const int udpPort = 55557; // This port is set using the SignalK server web page. Under Server/Connections/add.

// a global iteration counter, mostly for initiaring reboot/restart because of fail.
int iter=0;


void setup() {
 
/*
 * Set up the Serial port and the Software Serial port to 9 bits
 * Set up wifi and connect.
 * 
 */ 
#ifdef DEBUG   
  Serial.begin(115200);
#endif
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
#ifdef DEBUG    
  Serial.begin(115200);         // Setup Serial port speed
#endif 

  // Connect to the WiFi network
  WiFi.begin(ssid, pwd);
#ifdef DEBUG
  Serial.println("Connect to the WiFi network");
#endif
  // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
#ifdef DEBUG     
      Serial.print(".");
#endif
      if (++iter>30) ESP.restart(); // Issue a restart if fail to attach to network. Not really needed, but a restart is ok.
    }
    iter=0;
#ifdef DEBUG    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
#endif
    // This initializes udp and transfer buffer
    Udp.begin(udpPort);    
    delay(10);


  // Set up SoftwareSerial.  
  // Baud rade 4800 (Seatalk), Rx pin D5, Tx pin D6, 9 bits word, No parity, 1 stop bit. Rest is std.
  SwSerial.begin(4800, D5, D6, SWSERIAL_9N1, false, 95, 11);

// The command bit is 0x01zz where zz are nibbles or byte with data. The command bit is the 9t bit.

#ifdef DEBUG
  Serial.println("\nSoftware serial test started");
#endif
} // End setup


/*
 * Seatalk data related constants, included for reference.
 */

#define DBT      0x00,0x02,0x44,0x01,0x60    //  (DBT): 00 02 YZ XX XX
#define AWA      0x10,0x01,0x01,0x20         //  (AWA): 10 01 XX YY
#define AWS      0x11,0x01,0x20,0x01         //  (AWS): 11 01 XX 0Y
#define STW      0x20,0x01,0x01,0x20         //  (STW): 20 01 XX XX
#define TRMIL    0x21,0x02,0x01,0x20,0x05    //  Trip Mileage XXXXX/100 nautical miles : 21 02 XX XX 0X
#define TMIL     0x22,0x01,0x01,0x20,0x00    //  Total Mileage XXXXX/100 nautical miles : 22 02 XX XX 00
#define WTEMP    0x27,0x01,0x01,0x20         //  Water temp xxxx-100/10 Celsius: 27 01 XX XX


void generate_dummy_data(){
/*  
 *   A function to make some random like Seatalk data.
 */
  unsigned int value;  // int is a 16 bit variable. 
 
// The command bit is 0x01zz where zz are nibbles or byte with data. The command bit is the 9th bit. 
// The 9-bit version takes a 16 bit word as in/out parameter, unlike the 8 bit version which uses 
// byte/char 8 bit sized argument. 
 

// Apparent Wind speed:
  SwSerial.write(0x0111);
  SwSerial.write(0x0001);
  value=(int)random(0,25);  // Random return long (32 bit), cast to int (16 bit).
  SwSerial.write(value);
  value=(int)random(0,255);
  SwSerial.write(value);

// No command bit. Should be not be handled.
  SwSerial.write(0x007b); 

// Apparent Wind angle
  SwSerial.write(0x0110);
  SwSerial.write(0x0001);
  value=(int)random(0,1);
  SwSerial.write(value);
  value=(int)random(0,255);
  SwSerial.write(value); // Hex 0x010a er  266 Decimal, an early test. First write 0x0001 second 0x000a.

// Depth 
  SwSerial.write(0x0100); // 00 Depth
  SwSerial.write(0x0002); 
  SwSerial.write(0x0040); // Most significant nibble in low byte 4 for meters 0 (default) for feet.
  value=(int)random(0,2);
  SwSerial.write(value); // Example 0x0001 + 0x00f2 Hex => 0x01f3 is decimal 499.
  value=(int)random(0,255);
  SwSerial.write(value);

// Speed through water
  SwSerial.write(0x0120); // 20 Speed through water
  SwSerial.write(0x0001); // 01 
  value=(int)random(0,1);
  SwSerial.write(value); // Example 0x0001 + 0x0004 => 0x0104 hex is 260 decimal, 26 knots 
  value=(int)random(0,255);
  SwSerial.write(value); // 

// Some data with error (no command bit, should just be trown)
  SwSerial.write(0x001a); // No command bit. Should be not be handled.
  SwSerial.write(0x003c); // No command bit. Should be not be handled.
  SwSerial.write(0x006b); // No command bit. Should be not be handled.

// Water temperature 
  SwSerial.write(0x0127);
  SwSerial.write(0x0001);
  value=(int)random(1,2);
  SwSerial.write(value); // Example Hex 0x101 is 257. WTemp = (257-100)/10 => 15.7 
  value=(int)random(0,255);
  SwSerial.write(value);

#ifdef DEBUG
  Serial.println("\nData written to SoftwareSerial port");
#endif
} // End generate_dummy_data



/*
 * Function to convert strings and values and forward it using UDP to the Signal K server 
 * 
 */
 
void Send_to_SignalK(String key, double value){
   
    // Settings for SignalK port and SignalK server.
    const uint16_t port = udpPort;   // SignalK uses this port.
    const char * host = SignalK_server; // ip number of the SignalK server.
    String cmd;
    char valuestring[6];

    // Making a SignalK string in this way is a hack. 
    cmd = "{\"updates\": [{\"$source\": \"Simulation\",\"values\":[ {\"path\":\"";
    cmd += key; cmd += "\","; cmd += "\"value\":";
    dtostrf(value,3,2,valuestring); // Convert double to a string
    cmd += valuestring;
    cmd += "}]}]}\0";
#ifdef DEBUG      
    Serial.println(cmd);
    Serial.println(cmd.length());
#endif     
    
    char cmdc[cmd.length()+1];        // Convert the String to an array of characters.
    Udp.beginPacket(host,port);       // Connect to to server and prepare for UDP transfer.
    strncpy(cmdc,cmd.c_str(),sizeof(cmdc));  // Convert from String to array of characters. 
#ifdef DEBUG      
    Serial.println(cmdc); Serial.print(" Message has length: "); Serial.println(sizeof(cmdc));
#endif    
    Udp.write(cmdc);                  // Send the message to the SignalK server. 
    Udp.endPacket();                  // End the connection.
    delay(10);                        // Short delay to recover. 
} /* End Send_to_SignalK */




/*
 * Functions to handle the different Seatalk messages based on the message hex code receivced.
 */


// Apparent wind speed 
/*     
 *  Apparent Wind Speed (AWS): 11 01 XX 0Y
 *  (XX & 0x7F) + YY/10 = speed
 *  XX&0x80=0           = knots (default)
 *  XX&0x80=0x80        = meters/second
*/
int aws(){
  unsigned int d;
  char c1, c2;
  float awsp;
  float corr;
  
  d=SwSerial.read();
#ifdef DEBUG
  Serial.write("d HEX "); Serial.println(d, HEX);
  if (d == 0x0001) Serial.write("Wind speed : ");
#endif
  d=SwSerial.read();
  corr = (d & 0x80)?1.0:1.852;
  c1 = lowByte((d & 0x7f));
#ifdef DEBUG
  Serial.write("d  HEX "); Serial.println(d, HEX);
  Serial.write("c1 HEX ");Serial.println(c1, HEX);
  Serial.write("corr "); Serial.println(corr);
#endif
  d=SwSerial.read();
  c2=lowByte(d);
#ifdef DEBUG
  Serial.write("c2 HEX "); Serial.println(c2, HEX);
#endif   
  awsp=((float)c1+((float)c2)/10.0)/corr;
#ifdef DEBUG
  Serial.println(corr);
  Serial.write("AWS : ");
  Serial.println(awsp);
#endif
  Send_to_SignalK("environment.wind.speedApparent",awsp);
}


/*    
 *  Apparent Wind Angle (AWA): 10 01 XX YY
 *  
 */
int awa() {
  unsigned int d;
  char c1, c2; 
  int awang;
  
  d=SwSerial.read();
#ifdef DEBUG
  Serial.println(d, HEX);
  if (d == 0x0001) Serial.write("Wind angle : ");
#endif
  d=SwSerial.read();
  c1=lowByte(d);
#ifdef DEBUG  
  Serial.println(c1, HEX);  
#endif
  d=SwSerial.read();
  c2=lowByte(d);
#ifdef DEBUG  
  Serial.println(c2, HEX);
#endif 
  awang=makeWord(c1,c2);
#ifdef DEBUG
  Serial.println(awang);
#endif
  Send_to_SignalK("environment.wind.angleApparent",(awang/180.0)*3.1415);
}


/*    
 *    Depth below Transducer (DBT): 00 02 YZ XX XX   
 *    Y = 0 for feet (default), 4 for meters
 *    Z = 1 for shallow depth alarm, 4 for defective transducter
 *    XXXX = depth*10
 */ 
int dbt() {
  unsigned int d;
  char c1, c2; 
  float dbt;
  
  int meters;
  d=SwSerial.read();
#ifdef DEBUG
  Serial.println(d, HEX);
  if (d == 0x0002) Serial.write("Depth : ");
#endif
  d=SwSerial.read();
#ifdef DEBUG
  Serial.println((d & 0x00F0), HEX);
#endif
  meters=((d & 0x00F0) == 0x40);
#ifdef DEBUG
  Serial.println(meters, HEX);
#endif  
  //Read the data XX XX 
  d=SwSerial.read(); 
#ifdef DEBUG 
  Serial.println(d);
#endif  
  c1=lowByte(d);
#ifdef DEBUG
  Serial.println(c1, HEX);
#endif
  d=SwSerial.read();
#ifdef DEBUG
  Serial.println(d, HEX);
#endif
  c2=lowByte(d);
#ifdef DEBUG
  Serial.println(c2, HEX);
#endif
  d=makeWord(c1,c2);
#ifdef DEBUG
  Serial.println(d);
#endif      
  if (meters) dbt = (int)d/10.0; else dbt=(int)d/10.0*3.28;
#ifdef DEBUG
  Serial.println(dbt);
#endif
  Send_to_SignalK("environment.depth.belowTransducer",dbt);
}


/*
 * Water temperature (WTEMP)
 * WTEMP = (x - 100)/10
 */
int wtemp() {
  unsigned int d;
  char c1, c2;
  float wtemp;
  
  d=SwSerial.read(); // Read the 01.
#ifdef DEBUG
  Serial.write("Water Temperature : ");
#endif
  // Read the data XX XX
  d=SwSerial.read();
  c1=lowByte(d);
  d=SwSerial.read();
  c2=lowByte(d);
  d=makeWord(c1,c2); wtemp=(((float)d-100.0)/10.0);
#ifdef DEBUG
  Serial.println(wtemp);
#endif
  Send_to_SignalK("environment.water.temperature",(wtemp+273.15));
}


/*
 * Speed through Water (STW): 20 01 XX XX
 * XXXX = speed*10
 * 
 */
int stw() { 
  unsigned int d;
  char c1, c2;
  float stw;
  
  d=SwSerial.read(); // Read the 01.
#ifdef DEBUG
  Serial.println(d, HEX);
  Serial.write("Speed through water : ");
#endif
  // Read the data XX XX
  d=SwSerial.read();
  c1=lowByte(d);
  d=SwSerial.read();
  c2=lowByte(d);
  d=makeWord(c1,c2); stw=((float)d/10.0)/1.852; // SignalK is in m/s.
#ifdef DEBUG
  Serial.println(stw);
#endif
  Send_to_SignalK("navigation.speedThroughWater",stw);
}




/*
 * Main loop start here, this is the endless loop.
 */

void loop() {
  unsigned int d;
  char c1, c2;

  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on while we transfer the data.
  
  generate_dummy_data(); // Generate some random data in order to have some changing data to display.
#ifdef DEBUG
  Serial.println("\nRandom data sendt:");
#endif  
  
  while (SwSerial.available() > 0) {
    d=SwSerial.read();
#ifdef DEBUG
    Serial.println(d, HEX);
    Serial.println((d & 0x0100), HEX);
#endif 
    if ((d & 0x0100) == 0) {  // Check if the command bit is set, if not just enter one more cycle.
#ifdef DEBUG 
      Serial.println("Error no command bit set");
#endif 
      continue; 
    }  // if comand bit is set, this is the 9th bit. 
     
    d=(d & 0x00FF); // Strip off the comand bit. Only the least significant 8 bits in 
                    // variable d (16 bits uint) are left.
#ifdef DEBUG
    Serial.println(d, HEX);
#endif
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
  } // End while loop
  
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH.
  delay(20000);
  
}  // End loop 
