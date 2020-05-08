/*
 * 
 *  This sketch uses a ESP 8266 and a number of DS18B20 devices  to measure temperature 
 *  and sends messages to a SignalK server.
 *  
 *  Select board  ESP-12E , NodeMCU 1.0 ESP12-E module
 *
 * 
 *  SignalK uses UDP. The syntax is differ between ESP8622 and ESP32, this sketch 
 *  cannot be used for ESP32.
 *  
 *  
 *  Names of Signal K keys in the setup function.
 *  
 */

// Enable debug to print out messages on the serial port.
#define DEBUG;

// Including WiFi stuff - this is UDP and differ from TCP and also from ESP32 cards-
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>

// Instanciate WiFi and UDP objects
ESP8266WiFiMulti WiFiMulti;
WiFiUDP Udp;
  

// WiFi network name and password 
const char * ssid = "openplotter";
const char * pwd = "12345678";

// IP address to send UDP data to.
// Settings for SignalK port and SignalK server.
// const char * udpAddress        = "192.168.1.252";
// const char * udpAddress     = "10.10.10.1";

const char * SignalK_server    = "192.168.1.169";
// const char * SignalK_server = "10.10.10.1";
const int udpPort = 55557; // This port is set using the SignalK server web page. Under Server/Connections/add.


/*
 * Signal K paths/names name of the temperature sensors, description of what they measure.
 */
const char * keys[10];  // keys assigned in setup function.

/*
 * Iterations counter before restart, an ugly hack.
 */
int  iter=0;


/* 
 *  
 *  End declaration of global variables.
 *  
 */



// Setup function

void setup() {
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

/*
 * 
 * Set up the different SignalK names/paths which must be spec  
 * compliant (see: https://signalk.org/specification/1.3.0/doc/vesselsBranch.html ):
 * 
 * Some of these expect temperature in K like environment.inside.temperature.
 */
//********************************************************************************************************************************** 

    keys[0]="propulsion.1.temperature";                       // Engine engine block ; SignalK: propulsion/<RegExp>/temperature
    keys[1]="environment.inside.engineRoom.temperature";      // Engine room temp   ; SignalK: environment/inside/engineRoom   environment/inside/engineRoom1/temperature
    keys[2]="electrical.alternators.1.temperature.warnUpper"; // Alternator ; SignalK: electrical/alternators/<RegExp>/temperature

    keys[3]="propulsion.engine.coolantTemperature"; 
    keys[4]="propulsion.1.transmission.oilTemperature";        // Engine; SignalK: propulsion/<RegExp>/transmission/oilTemperature

    keys[5]="environment.wind.speedApparent";
    keys[6]="environment.wind.angleApparent";
//**********************************************************************************************************************************

}  // End setup 



/*
 * Function to convert strings and values and forward it using UDP to the Signal K server 
 * 
 */
 
void Send_to_SignalK(String key, double value){
   
  // Settings for SignalK port and SignalK server.
    const uint16_t port = udpPort;   // SignalK uses this port.
    //const char * host = "192.168.1.254"; // ip number of the SignalK server.
    const char * host = SignalK_server; // ip number of the SignalK server.
    String cmd;
    char valuestring[6];

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
    Serial.println(cmdc); Serial.print(" Message har length: "); Serial.println(sizeof(cmdc));
#endif    
    Udp.write(cmdc);                  // Send the message to the SignalK server. 
    Udp.endPacket();                  // End the connection.
    delay(10);                        // Short delay to recover. 
} /* End Send_to_SignalK */




//**********************************************************************************************************************************
//                  The simulation function :
//**********************************************************************************************************************************
void Send_simulation(){
    double s,d;
    long t;
    
    t=millis()%60; // Get a number from 0 to 60. 
    
    s=1.0*0.1*t; // Simulate wind t runs from 0 to 60, e.g. sine from 0 to 6 m/s.
    d=6.0*t;     // Simulate direction from 0-360 degrees. 
        
    Send_to_SignalK("environment.wind.speedApparent",s);
    Send_to_SignalK("environment.wind.angleApparent",(d/180)*3.14159264); // SignalK uses Radians. Most of us uses degrees.
}
 

//**********************************************************************************************************************************
//                    The infinite loop
//**********************************************************************************************************************************

void loop() {
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on while we transfer the data.
  Send_simulation();                      // Loop through devices and measure 
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH.
  delay(2000);                     // Send frequancy N seconds. 
}  // End infinite loop
 


