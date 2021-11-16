/*
 * 
 *  This sketch uses a ESP 8266 and a number of DS18B20 devices  to measure temperature 
 *  and sends messages to a SignalK server at regular intervals.
 *  
 *  Select board  ESP-12E , NodeMCU 1.0 ESP12-E module. 
 *
 * 
 *  SignalK uses UDP. The syntax is differ between ESP8622 and ESP32, this sketch 
 *  cannot be used for ESP32. 
 *  
 *  
 *  Names of Signal K keys of the temperature sensors initialised in the setup function.
 *  Check valid names of keys with the signal K documentation, 
 *  https://signalk.org/specification/1.5.0/doc/vesselsBranch.html
 *  
 */

 /*
  * Version 1.0  02/01-2020
  * Version 1.1  15/11-2021 
  * 
  */

// Enable debug to print out messages on the serial port.
#define DEBUG;

// One wire bus setup
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D3      // Pin to which is attached a temperature sensor
#define ONE_WIRE_MAX_DEV 15  // The maximum number of devices

// Instanciate onewire and DS18B20 objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

// Including WiFi stuff - this is UDP and differ from TCP and also from ESP32 cards-
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>

// Instanciate WiFi and UDP objects
ESP8266WiFiMulti WiFiMulti;
WiFiUDP Udp;
  

// Global variables 
int numberOfDevices;                      // Number of temperature devices found
DeviceAddress devAddr[ONE_WIRE_MAX_DEV];  // An array devicetemperature sensors
float tempDev[ONE_WIRE_MAX_DEV];          // Saving the last measurement of temperature

// WiFi network name and password 
const char * ssid = "openplotter";
const char * pwd = "blackpearl";


// IP address to send UDP data to.
// Settings for SignalK port and SignalK server.
// const char * udpAddress        = "192.168.1.252";
// const char * udpAddress        = "10.10.10.1";
// const char * SignalK_server    = "192.168.1.252";
const char * SignalK_server = "10.10.10.1";
const int udpPort = 55557;

/*
 * Signal K paths/names name of the temperature sensors, description of what they measure.
 */
const char * keys[ONE_WIRE_MAX_DEV];  // keys assigned in setup function.

/*
 * Iterations counter zeroed before restart, global interation counter. 
 * Used to reset at regular inrervals as the loop tend to hang at random intervals.
 */
int  iter=0;

/* 
 *  
 *  End declaration of global variables.
 *  
 */



// The Setup function

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


  // Set up the DS18B20 temperature sensors
    SetupDS18B20();   

/*
 * 
 * Set up the different SignalK names/paths which must be spec  
 * compliant (see: https://signalk.org/specification/1.5.0/doc/vesselsBranch.html ):
 * 
 * Some of these expect temperature in K like environment.inside.temperature.
 */
//********************************************************************************************************************************** 

    keys[0]="propulsion.1.temperature";                       // Engine engine block ; SignalK: propulsion/<RegExp>/temperature
    keys[1]="environment.inside.engineRoom.temperature";      // Engine room temp   ; SignalK: environment/inside/engineRoom   environment/inside/engineRoom1/temperature
    keys[2]="electrical.alternators.1.temperature.warnUpper"; // Alternator ; SignalK: electrical/alternators/<RegExp>/temperature

    keys[3]="propulsion.engine.coolantTemperature"; 
    keys[4]="propulsion.1.transmission.oilTemperature";        // Engine; SignalK: propulsion/<RegExp>/transmission/oilTemperature
    
//**********************************************************************************************************************************

}  // End setup function



// Convert device id to String
String GetAddressToString(DeviceAddress deviceAddress){
  String str = "";
  for (uint8_t i = 0; i < 8; i++){
    if( deviceAddress[i] < 16 ) str += String(0, HEX);
    str += String(deviceAddress[i], HEX);
  }
  return str;
} // End GetAddressToString



// Setting the temperature sensor
void SetupDS18B20(){
  DS18B20.begin();

#ifdef DEBUG  
  Serial.print("Parasite power is: "); 
  if( DS18B20.isParasitePowerMode() ){ 
    Serial.println("ON");
  }
  else {
    Serial.println("OFF");
  }
#endif
  
  numberOfDevices = DS18B20.getDeviceCount();

#ifdef DEBUG    
  Serial.print( "Device count: " );
  Serial.println( numberOfDevices );
#endif 

  DS18B20.requestTemperatures();

  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if( DS18B20.getAddress(devAddr[i], i) ){
    // devAddr[i] = tempDeviceAddress;
#ifdef DEBUG      
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: " + GetAddressToString(devAddr[i]));
      Serial.println(); 
#endif          
    }else{
#ifdef DEBUG  
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
#endif      
    }

  // Get resolution of DS18B20
#ifdef DEBUG    
    Serial.print("Resolution: ");
    Serial.print(DS18B20.getResolution(devAddr[i]));
    Serial.println();
#endif
  // Read temperature from DS18b20
    float tempC = DS18B20.getTempC(devAddr[i]);
#ifdef DEBUG      
    Serial.print("Temp C: ");
    Serial.println(tempC);
#endif    
  }  // End for numberOfDevices
}  // End SetupDS18B20




/*
 * Function to convert strings and values and forward it using UDP to the Signal K server 
 *
 * The SignalK message is manually put together to make it simple. A library might be 
 * simpler, but introduce yet dependencies. 
 * 
 */
 
void Send_to_SignalK(String key, double value){
   
  // Settings for SignalK port and SignalK server.
    const uint16_t port = 55557;   // SignalK uses this port.
    //const char * host = "192.168.1.254"; // ip number of the SignalK server.
    const char * host = SignalK_server; // ip number of the SignalK server.
    String cmd;
    char valuestring[6];

    // SignalK selected keys expects the temperaure in Kelvin
    value+=273.15;
    cmd = "{\"updates\": [{\"$source\": \"ESP8266-Temp\",\"values\":[ {\"path\":\"";
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



/*
 * 
 * Loop measuring the temperature, collect all temperatures and forward to signal K sender.
 * 
 */

void TempLoop(){
    for(int i=0; i<numberOfDevices; i++){
      float tempC = DS18B20.getTempC(devAddr[i]);
      // Measuring temperature in Celsius
      // If temperature reading fails, e.g. the error code -127 is reported the
      // 1-wire setup is called and the function return. Hopefully this will cause the
      // reading to return to normal again. Initially some electric noise caused the sensor
      // readings to fail after only a few minutes.
      if (tempC < -100) {   
          // Error trigger er restart, it might work.
          ESP.restart();
          return; 
      }
      tempDev[i] = tempC; //Save the measured value to the array
#ifdef DEBUG        
      Serial.print("Device "); Serial.print(i); Serial.print(" Temp C: "); Serial.println(tempC);
#endif      
      Send_to_SignalK(keys[i],tempDev[i]);  // Send the key with its temperature to the SignalK sever. 
    }
    DS18B20.setWaitForConversion(false); // No waiting for measurement
    DS18B20.requestTemperatures();       // Initiate the temperature measurement
  }  // End TempLoop
 

// The infinite loop
void loop() {
  if (++iter>100) ESP.restart();   // A really ugly hack, force a restart every 500 seconds, it might help when 1-wire is failing. 
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on while we transfer the data.
  TempLoop();                      // Loop through devices and measure 
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH.
  delay(5000);                     // Send frequency, every 5 second => 0.2 Hz.
}  // End looop
 
