/*

 Select board ESP-12E, NodeMCU 1.0 ESP12-E mmodule in the Arduino IDE. 
 
*/


/*
 *  The blue LED on the ESP-01 module is connected to GPIO1 
 *  (which is also the TXD pin; so we cannot use Serial.print() at the same time,
 *   it also blinks with high frequency during upload or communication).

 * A total of 12 pins can be used for digital IO. However, as some have several uses
 * like 16 the built in led and 1 and 3 which is serial communication the number is less.
 * In practice 9 can be freely used. Also issues with GPIO pin 15.
 * 
 */
// Right side :
// GPIO_16 is equal to LED_BUILTIN.
#define GPIO_16 16  // Can be used for output, controlled like any other Digital IO pin.
#define GPIO_05  5
#define GPIO_04  4 
#define GPIO_00  0
#define GPIO_02  2 // Had issues with this, trigger blue led on the wifi module
// 3.3V and GND between the above and below
#define GPIO_14 14
#define GPIO_12 12
#define GPIO_13 13
#define GPIO_15 15
#define GPIO_03 03 // These two are for Serial communication.
#define GPIO_01 01 // Using these for IO blocks of serial commmunication.

// Left side :
// Only GPIO 10 works.
#define GPIO_10 10


// Including WiFi stuff
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>

// Instanciate WiFi object
ESP8266WiFiMulti WiFiMulti;
// Instanciate UDP object
WiFiUDP Udp;


// Global variables

// Wifi info
const char * ssid = "openplotter";
const char * pwd = "blackpearl";
const uint16_t port = 55557;      // SignalK uses this port.
const char * host = "10.10.10.1"; // ip number of the SignalK server.

// IO Pins for sensors
// These are are the pins used :
// GPIO 14, GPIO 12, GPIO_13, GPIO_04 
// On the card  D5, D6, D7 and D2

const short num_sens=4;
const int sensor_pin[num_sens]={GPIO_14, GPIO_12, GPIO_13, GPIO_04};

  
// Set up the the system and configure the pins and set up the wifi.
void setup() {
// Serial.begin(115200);
// Setting up the pins as digital IO pins.  
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  for (int j=0;j<num_sens;j++) {
    //Serial.print(j); Serial.print(" "); Serial.print(sensor_pin[j]);
    pinMode(sensor_pin[j],INPUT_PULLUP); // initialize the pin for input.
  }
 
// Setting up the wifi
  // We start by connecting to a WiFi network
    WiFiMulti.addAP(ssid, pwd);
    /*
    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");
    */
// Wait for association with the access point and connection to the network.
    while(WiFiMulti.run() != WL_CONNECTED) {
        //Serial.print(".");
        delay(500);
    }
    /*  
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    */
}  // End setup 



void Send_to_SignalK(String path, float value){
// Settings for SignalK port and SignalK server.
    String cmd;
    char valuestring[6];

    cmd = "{\"updates\": [{\"$source\": \"ESP8266\",\"values\":[ {\"path\":\"";
    cmd += path; cmd += "\","; cmd += "\"value\":";
    dtostrf((double)value,3,2,valuestring); // Convert double to a string
    cmd += valuestring;
    cmd += "}]}]}\0";
    /*
    Serial.println(cmd);
    Serial.println(cmd.length());
    */

    char cmdc[cmd.length()+1];        // Convert the String to an array of characters.
    Udp.beginPacket(host,port);       // Connect to to server and prepare for UDP transfer.
    strncpy(cmdc,cmd.c_str(),sizeof(cmdc));  // Convert from String to array of characters. 
    //Serial.println(cmdc); Serial.print(" Message har length: "); Serial.println(sizeof(cmdc));
    Udp.write(cmdc);                  // Send the message to the SignalK server. 
    Udp.endPacket();                  // End the connection.
    delay(10);                        // Short delay to recover. 
} /* End Send_to_SignalK */

  

void Measure_and_send_Tanklevel(int tankno){
   
    short sensors;

    float tanklevel=0.0;
    const float inc = 1.0/(float)num_sens;

    for (int j=0;j<num_sens;j++){
      tanklevel += inc * (~(digitalRead(sensor_pin[j]))&1) ;
      /*
      Serial.print(j); Serial.print(" : sensor ");
      Serial.print(sensor_pin[j]); Serial.print(" signl ");Serial.print((digitalRead(sensor_pin[j]))&1);
      Serial.print(" Tanklevel :"); Serial.println(tanklevel);
      */
    }
 
    //Serial.print("Tanklevel :"); Serial.println(tanklevel);    

    Send_to_SignalK("tanks.blackWater.1.currentLevel",tanklevel);
}


// the loop function runs over and over again forever in an endless loop.
void loop() { 

  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on.
  Measure_and_send_Tanklevel(1);    // Get the Tank level (tank 1) and send it to SignalK.
  delay(10);                        // long enough to see it.
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH.
  delay(5000);                      // Wait for five seconds 0.2 Hz update frequency is ok.
}  /* End infinite loop */


