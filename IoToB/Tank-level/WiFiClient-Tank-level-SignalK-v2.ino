/*

 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)

 Select board ESP-12E, NodeMCU 1.0 ESP12-E mmodule
*/



/*
 * A total of 12 pins can be used for digital IO. However, as some have several uses
 * like 16 the built in led and 1 and 3 which is serial communication the number is less.
 * In practice 9 can be freely used.
 * 
 */
// Right side :
// GPIO_16 is equal to LED_BUILTIN.
#define GPIO_16 16  // Can be used for output, controlled like any other Digital IO pin.
#define GPIO_05  5
#define GPIO_04  4
#define GPIO_00  0
#define GPIO_02  2
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
WiFiUDP Udp;


// Global variables
int ledpin  =   10;
const int red=  GPIO_05;
const int green=GPIO_04;
const int blue= GPIO_00;
const int low = GPIO_13;
const int mid = GPIO_12;
const int top = GPIO_14;


// Set up the the system and configure the pins and set up the wifi.
void setup() {
  Serial.begin(115200);
// Setting up the pins as digital IO pins.  
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(ledpin,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(low,INPUT_PULLUP);
  pinMode(mid,INPUT_PULLUP);
  pinMode(top,INPUT_PULLUP);

// Setting up the wifi
  // We start by connecting to a WiFi network
    WiFiMulti.addAP("TeamRocketHQ", "password");

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");
    
// Wait for association with the access point and connection to the network.
    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);
    digitalWrite(green,LOW); digitalWrite(blue,LOW);  digitalWrite(red,LOW);
}  // End setup 



void Send_to_SignalK(String path, float value){
// Settings for SignalK port and SignalK server.
    const uint16_t port = 55557;   // SignalK uses this port.
    const char * host = "192.168.1.160"; // ip number of the SignalK server.
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
    Serial.println(cmdc); Serial.print(" Message har length: "); Serial.println(sizeof(cmdc));
    Udp.write(cmdc);                  // Send the message to the SignalK server. 
    Udp.endPacket();                  // End the connection.
    delay(10);                        // Short delay to recover. 
} /* End Send_to_SignalK */

  

void Measure_and_send_Tanklevel(int tankno){
   
    short sensors;
    const short num_sens=3;
    float tanklevel=0.0;
    const float inc = 1.0/(float)num_sens;

    const int TOP=GPIO_14;
    const int MID=GPIO_12;
    const int BOT=GPIO_13;

  // Read the sensors starting at the Highest down to the lowest.
    sensors=(~(digitalRead(TOP))&1) ;                 //  The topmost sensor 
    sensors=(sensors<<1)+(~(digitalRead(MID))&1);     //  The mid sensor
    sensors=(sensors<<1)+(~(digitalRead(BOT))&1);     //  The lowest sensor 

/*
 * Either this block for only SignalK use
 */
    for (int j=0;j<num_sens;j++){
      tanklevel+=inc*(sensors&1); 
      sensors=sensors>>1;   
    }
/*
 * Or this block if RGB LED is used as an indiacator during prototyping and demo.
 */
    
    /*    
    if (sensors>>2){digitalWrite(red,HIGH);  digitalWrite(green,LOW);digitalWrite(blue,LOW);tanklevel=0.95;}else
    if (sensors>>1){digitalWrite(green,LOW); digitalWrite(blue,HIGH);digitalWrite(red,LOW); tanklevel=0.66;}else
    if (sensors>>0){digitalWrite(green,HIGH);digitalWrite(blue,LOW); digitalWrite(red,LOW); tanklevel=0.33;}else
    if (!sensors)  {digitalWrite(green,LOW); digitalWrite(blue,LOW); digitalWrite(red,LOW); tanklevel=0.0;}
    */
    Serial.print("Tanklevel :"); Serial.println(tanklevel);    

    Send_to_SignalK("tanks.freshWater.1.currentLevel",tanklevel);
}



// the loop function runs over and over again forever in an endless loop.
void loop() { 

  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on 
  
  Measure_and_send_Tanklevel(1);    // Get the Tank level (tank 1) and send it to SignalK.
 
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH 
  delay(1000);                      // Wait for two seconds (to demonstrate the active low LED)
  digitalWrite(ledpin,LOW);         // Blinking a LED just to monitor progress and heartbeat.
  delay(1000);
  digitalWrite(ledpin,HIGH);
}  /* End infinite loop */


