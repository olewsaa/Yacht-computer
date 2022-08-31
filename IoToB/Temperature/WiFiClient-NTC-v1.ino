/*
 *  This sketch uses a ESP 32 measure temperatures using NTC and  
 *  sends messages to a SignalK server
 * 
 *  Ole W. Saastad 
 *  v1.00 23 Aug 2022
 *  v1.01 26 Aug 2022 Polynomial eval macro.
 *  v1.02 27 Aug 2022 Fixed Temp to Kelvin, change width in dtostrf, check size.
 *  
 * 
 *  Select board ESP32 , NodeMCU-32S, Node32s
 *  
 *  IDE Arduino 1.8.19 
 *  ESP modules can be added with this string in settings "boards manager"
 *  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 *
 *  Analog voltage is single ended and read by the ESP analog inputs. 
 *  
 *  The current sensors emits Vdd/2 for zero current and voler that Vdd/2 for flow
 *  in one direction and voltage higher than Vdd/2 for flow in the oposite direction,
 *  Hence differential measurement is sensible as it yield negative values below Vdd/2 
 *  and positive values above Vdd/2.
 *
 *  SignalK uses UDP. The syntax is differ between ESP8622 and ESP32.
 *  
 *  https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
 *  https://www.electronics-tutorials.ws/io/thermistors.html
 *  
 */
#define DEBUG ;
 
#define POLY2(x,a,b,c)     (a + b*x + c*x*x)
#define POLY3(x,a,b,c,d)   (a + b*x + c*x*x + d*x*x*x)
#define POLY4(x,a,b,c,d,e) (a + b*x + c*x*x + d*x*x*x + e*x*x*x*x)

// Analog Input on the ESP32, this board has many analog inputs.
#define ANALOG_PIN_0 36  // Pin GPIO 36
#define ANALOG_PIN_3 39  // Pin GPIO 39
#define ANALOG_PIN_4 32  // Pin GPIO 32
#define ANALOG_PIN_5 33  // Pin GPIO 33
#define ANALOG_PIN_6 34  // Pin GPIO 34
#define ANALOG_PIN_7 35  // Pin GPIO 35

#define MAX_SENSORS 6 // Maximum number of ADCs on channel 1 
                      // ADC 2 cannot be used concurrent with wifi.
// Define the pins to use.                      
const int adcpin[MAX_SENSORS] = {ANALOG_PIN_0, ANALOG_PIN_3, ANALOG_PIN_4, ANALOG_PIN_5, ANALOG_PIN_6, ANALOG_PIN_7}; 

/*
 * Signal K paths/names name of the temperature sensors, description of what 
 * they measure. Temperatures in K.
 *
 * Set up the different SignalK names/paths which must be spec      
 * compliant (see: https://signalk.org/specification/1.7.0/doc/vesselsBranch.html )
 * 
 */
const char * signalk_keys[MAX_SENSORS] =
  {"environment.inside.engineroom.temperature",     // environment/inside/[A-Za-z0-9]+/temperature	  
   "propulsion.engine.temperature",                 // propulsion/<RegExp>/temperature		
   "propulsion.engine.coolantTemperature",          // propulsion/<RegExp>/coolantTemperature
   "electrical.alternators.1.temperature",          // electrical/alternators/<RegExp>/temperature	
   "electrical.alternators.1.regulatorTemperature", // electrical/alternators/<RegExp>/regulatorTemperature	
   "propulsion.engine.transmission.oilTemperature"};// propulsion/<RegExp>/transmission/oilTemperature

/* 
 * How many sensors are installed ? 
*/
#define SENSORS 1 // Number of sensors currently.


// ADC stuff
#include "driver/adc.h"
#include "esp_adc_cal.h"


// Including WiFi stuff
#include <WiFi.h>
#include <WiFiUdp.h>

/* WiFi network name and password */
const char * ssid = "TeamRocketHQ";
const char * pwd = "password";
//const char * ssid = "openplotter";
//const char * pwd = "password";

// IP address to send UDP data to.
// it can be ip address of the server or 
// Settings for SignalK port and SignalK server.
//const char * udpAddress = "192.168.1.160";
const char * udpAddress = "10.10.10.1"; // This is the openplotter address.
const int udpPort = 55557;  // Openplotter is listening on this port,
                            // port is set in Openplotter connections.
                            // Each of the wifi clients can have a unique
                            // port.

// Instanciate WiFi object
WiFiUDP Udp;


void setup() {
    pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
#ifdef DEBUG 
    Serial.begin(115200);
#endif

  //Connect to the WiFi network
    WiFi.begin(ssid, pwd);
#ifdef DEBUG 
    Serial.println("");
#endif

  // Wait for connection
   while (WiFi.status() != WL_CONNECTED) {
      delay(500);
#ifdef DEBUG 
      Serial.print(".");
#endif
   } // end while wifi connection.
#ifdef DEBUG 
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
#endif
  //This initializes udp and transfer buffer
    Udp.begin(udpPort);    
    delay(10);
   

 // Set up ADC1 and its channels.
    // Set 12 bits (4096) values
    adc1_config_width(ADC_WIDTH_BIT_12);
    // Set all channels to 11 dB attenuation,
    // 11 dB attenuation (ADC_ATTEN_DB_11) gives full-scale voltage 3.9 V
    
    // esp_err_t status;
    // status = adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    // if (status != ESP_OK) ESP.restart();
    // if (adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11) != ESP_OK) ESP.restart();
    
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_1, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_2, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);


} /* End setup */



void Send_to_SignalK(String path, float value){
    String cmd;
    char valuestring[6]; // Must be long enough to accomodate the number. Length of "289.2" is 5 so this is ok with dtostrf(). 
    uint8_t buffer[160]; // Buffer to hold the cmd string.
    int len;     
            
  // SignalK selected keys expects the temperaure in Kelvin.
    value+=273.15;

  // Prepage the SignalK string to send.
    cmd = "{\"updates\": [{\"$source\": \"ESP32\",\"values\":[ {\"path\":\"";
    cmd += path; cmd += "\","; cmd += "\"value\":";
    dtostrf(value,4,1,valuestring); // Convert float to a string, width need
                                    // to be enough, overflow cause abort.
    cmd += valuestring;
    cmd += "}]}]}\0";
    len=cmd.length(); // Get the lenght of message, needed by the udp write.
#ifdef DEBUG    
    Serial.print("JSON string "); Serial.println(cmd); 
    Serial.print("Length of JSON string "); Serial.println(len);
#endif    
  //data will be sent to the SignalK server
    //uint8_t buffer[160]; 
    //for(int j=0;j<len;j++){buffer[j]=cmd[j];} // Convert from char to bytes. 
    //memcpy(buffer,&cmd[0],len); // Convert from char to bytes, only len bytes.
    cmd.getBytes(buffer, len); // Extract the characters into a byte array.
    //send buffer to server
    Udp.beginPacket(udpAddress, udpPort);
    Udp.write(buffer, len); // UDP write will only write sequence of bytes. 
    Udp.endPacket();
#ifdef DEBUG    
    Serial.println(cmd); 
#endif
    delay(10); 
   
} /* End Send_to_SignalK */

// Calibration from ADC count to degree °C. 
float cal(int adc){
  // fourth order polynomal a + bx + cx^2 + dx^3 + ex^4
  const double  a=-27.6846446459551;
  const double  b=0.01991717672664;
  const double  c=3.8135734768874e-05;
  const double  d=-2.79430835108657e-08;
  const double  e=5.8654883747814e-12;
  
  // temporary exp function.
  // return (float) a*exp(b*adc); 

  return (float) POLY4((double)adc, a, b, c, d, e);
}


void TemperatureLoop() { // Loop through all the sensors.
  int sensor[MAX_SENSORS]; // Maximum number of sensors using only ADC 1
  float temp[SENSORS]; // Keep temperatures in an array
   // Loop through the NTC sensors
  for(int j=0;j<SENSORS;j++) {
    // Average over a number of samples, 200 samples seems to work nice.
    int  sum=0;
    for(int k=0;k<200;k++) sum += analogRead(adcpin[j]); 
    sensor[j] = sum/200;     
    temp[j] = cal(sensor[j]);  // Convert from ADC count to temperature.
#ifdef DEBUG     
    Serial.print("Sensor "); Serial.print(j);
    Serial.print("  : "); Serial.print(sensor[j]);
    Serial.print(", "); Serial.print(temp[j]); Serial.println("°C"); 
#endif
    Send_to_SignalK(signalk_keys[j], temp[j]);  // Send the key with its
                                                // temperature to the SignalK sever.  
  } // End looping through sensors
} // End TemperatureLoop 


// The infinite loop start here, Arduino IDE has no main()

void loop() { 
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on while we transfer the data.
#ifdef DEBUG 
  Serial.println("Read voltage as ADC count");
#endif
  TemperatureLoop();
  
  delay(100); digitalWrite(LED_BUILTIN, LOW); // Turn the LED off by making the voltage LOW.
#ifdef DEBUG     
  Serial.println("wait 1 sec...");  // This high refresh rate put a load in the SignalK server.
  delay(1000);  // 1000 ms delay.
#else
  delay(5000); // 5 sec delay, update only every 5th second.
#endif
}  /* End infinite loop, this is the only thread the controller run  */

// End program.
