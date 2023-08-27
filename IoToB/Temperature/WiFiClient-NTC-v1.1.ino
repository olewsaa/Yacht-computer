/*
 *  This sketch uses a ESP 32 measure temperatures using NTC and  
 *  sends messages to a SignalK server
 * 
 *  Ole W. Saastad 
 *  v1.00 23 Aug 2022
 *  v1.01 26 Aug 2022 Polynomial eval macro.
 *  v1.02 27 Aug 2022 Fixed Temp to Kelvin, change width in dtostrf, check size.
 *  v1.03 16 Oct 2022 Added reset at regular intervals
 *  v1.04 24 Oct 2022 Cleaned code
 *  v1.05 29 Oct 2022 Added wifi disconnect before restart
 *  v1.06 01 Nov 2022 None debug build, no restart.
 *  v1.1  18 Aug 2023 New calibration of NTC, done by author using water and oven.
 *  v1.2  27 Aug 2023 Calibration onboard, using IR thermometer and live ADC count.
 * 
 *  Select board ESP32 Arduino , NodeMCU-32S, Node32s, 
 *  this project uses a 30 pin ESP32.
 *  
 *  IDE Arduino 1.8.19 
 *  ESP modules can be added with this string in settings "boards manager"
 *  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 *
 *  Analog voltage is single ended and read by the ESP analog inputs. 
 *  
 *  SignalK uses UDP. The syntax is differ between ESP8622 and ESP32.
 *  
 *  
 *  Analog input and NTC negative temperature themistors. 
 *  
 *  https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
 *  https://www.electronics-tutorials.ws/io/thermistors.html
 *  
 *  To restart the ESP32 module:  ESP.restart();
 *
 */
//#define DEBUG ;
 
#define POLY2(x,a,b,c)     (a + b*x + c*x*x)
#define POLY3(x,a,b,c,d)   (a + b*x + c*x*x + d*x*x*x)
#define POLY4(x,a,b,c,d,e) (a + b*x + c*x*x + d*x*x*x + e*x*x*x*x)

// Analog Input on the ESP32, this board has many analog inputs.
#define ANALOG_PIN_0 36  // Pin GPIO D36 also VP
#define ANALOG_PIN_3 39  // Pin GPIO D39 also UN
#define ANALOG_PIN_4 32  // Pin GPIO D32
#define ANALOG_PIN_5 33  // Pin GPIO D33
#define ANALOG_PIN_6 34  // Pin GPIO D34
#define ANALOG_PIN_7 35  // Pin GPIO D35

#define MAX_SENSORS 6 // Maximum number of ADCs on channel 1 
                      // ADC 2 cannot be used concurrent with wifi.
// Define the pins to use. Numbered to match pins consequtively.
const int adcpin[MAX_SENSORS] = {ANALOG_PIN_0, ANALOG_PIN_3, ANALOG_PIN_4, ANALOG_PIN_7, ANALOG_PIN_6, ANALOG_PIN_5}; 

/*
 * Signal K paths/names name of the temperature sensors, description of what 
 * they measure. Temperatures in K.
 *
 * Set up the different SignalK names/paths which must be spec      
 * compliant (see: https://signalk.org/specification/1.7.0/doc/vesselsBranch.html )
 * 
 */
const char * signalk_keys[MAX_SENSORS] = {  // Only the N first is used if SENSORS (se below) < 6.
//********************************* SignalK path names  ***************************************************** 
  
   "propulsion.engine.coolantTemperature",        // propulsion/<RegExp>/coolantTemperature
   "propulsion.engine.temperature",               // propulsion/<RegExp>/temperature    
   "propulsion.engine.exhaustTemperature",        // /vessels/<RegExp>/propulsion/<RegExp>/exhaustTemperature
   "environment.inside.engineRoom.temperature",   // environment/inside/[A-Za-z0-9]+/temperature	  
   "electrical.alternators.1.temperature"         // electrical/alternators/<RegExp>/temperature 
  
//********************************* SignalK path names  ***************************************************** 
   };  
  
/* 
 * How many sensors are installed ? 
*/
#define SENSORS 5 // Number of sensors currently. 6 sensors is the max when running wifi.


// ADC stuff
#include "driver/adc.h"
#include "esp_adc_cal.h"


// Including WiFi stuff
#include <WiFi.h>
#include <WiFiUdp.h>

/* WiFi network name and password */
const char * ssid = "openplotter";
const char * pwd  = "password";

// IP address to send UDP data to.
// it can be ip address of the server or 
// Settings for SignalK port and SignalK server.
//const char * udpAddress = "192.168.1.160";
const char * udpAddress = "10.10.10.1"; // This is the openplotter address.
const int udpPort = 55558;  // Openplotter is listening on this port,
                            // port is set in Openplotter connections.
                            // Each of the wifi clients can have a unique user selectable port.

// Instanciate WiFi object
WiFiUDP Udp;

// 
short Resetcount=60; // at 5 sek interval 60*5=300 sek or  5 min 

/*
 * Init start. The setup function is run at start and all initialisations must be done here.
 */


void setup() {
    pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
#ifdef DEBUG 
    Serial.begin(115200);
#endif


#ifdef DEBUG
    Serial.println("Setting up ADC");
#endif       
 // Set up ADC1 and its channels, restart ESP if errors.
    // Set 12 bits (4096) values
    if (adc1_config_width(ADC_WIDTH_BIT_12) != ESP_OK) ESP.restart();
    // Set all channels to 11 dB attenuation,
    // 11 dB attenuation (ADC_ATTEN_DB_11) gives full-scale voltage 3.9 V
    
    // esp_err_t status;
    // status = adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    // if (status != ESP_OK) ESP.restart();
    // if (adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11) != ESP_OK) ESP.restart();
    
    if (adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11) != ESP_OK) ESP.restart();
    if (adc1_config_channel_atten(ADC1_CHANNEL_1, ADC_ATTEN_DB_11) != ESP_OK) ESP.restart();
    if (adc1_config_channel_atten(ADC1_CHANNEL_2, ADC_ATTEN_DB_11) != ESP_OK) ESP.restart();
    if (adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11) != ESP_OK) ESP.restart();
    if (adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11) != ESP_OK) ESP.restart();
    if (adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11) != ESP_OK) ESP.restart();

#ifdef DEBUG
    Serial.println("ADC setup OK");
#endif



  //Connect to the WiFi network
    WiFi.begin(ssid, pwd);
#ifdef DEBUG 
    Serial.println("");
#endif

  // Wait for connection
   while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on while we try to connect
      delay(250);
      digitalWrite(LED_BUILTIN, LOW); // Turn the LED off by making the voltage LOW.
      delay(250);
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
   

} /* End function setup */



void Send_to_SignalK(String path, float value){
    String cmd;
    char valuestring[6]; // Must be long enough to accomodate the number. Length of "289.2" is 5 so this is ok with dtostrf().     
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
    uint8_t buffer[len];        // Buffer to hold the cmd string characters. 
    memcpy(buffer,&cmd[0],len); // Convert from string to bytes, udp.write don't
                                // accept string type buffer as input.
    //send buffer to server
    Udp.beginPacket(udpAddress, udpPort);
    Udp.write(buffer, len); // UDP write will only write sequence of bytes. 
    Udp.endPacket();
    delay(10);    
} /* End Send_to_SignalK */


// Calibration from ADC count to degree °C. 
float cal(uint16_t adc) {

  float result;
  if (adc <  1160) { // For low temperatures, allow extrapolation below freezing. 
    const double a1 =  0.0249230111206159 ; // even without calibration points. 
    const double b1 = -9.75697177074422 ;   // Not perfect but better than just set to 0.

    result = (float) (a1*(double)adc + b1) ; // ax+b
    
  }
  else {  // If adc > 1160, temperatures of interest for operating the engine. 
  // Second order polynomial a + bx + cx² one for each sensor.
  const double a2 =  45.1020401080492;    // Constant, a for 0,1,2,3,.. th sensor
  const double b2 =  -0.0554304542547959; // 1. order coef, b for 0,1,2,3,.. th sensor
  const double c2 =   2.94317866506764e-5; // 2. order coef, c for 0,1,2,3,.. th sensor
 
  
  result = (float)POLY2((double)adc, a2, b2, c2); // a+bx+bx²
  }

  // Used for calibration, this is the read ADC count at measured temerature.
  // result=(float)adc/10.0; 
  
#ifdef DEBUG
  Serial.print("adc value");
  Serial.print(adc);
  Serial.print("  Return value, cal ");
  Serial.println(result);
#endif 

  return result;  // Return the temperature calulated.
}


void TemperatureLoop() {    // Loop through all the sensors.
  uint16_t sensor[SENSORS]; // Maximum number of sensors used
  float temp[SENSORS];      // Keep temperatures in an array
   // Loop through the NTC sensors
  for(int j=0;j<SENSORS;j++) {
    // Average over a number of samples, 200 samples seems to work nice.
    uint32_t  sum=0; // ESP32 uses 32 bit int.
    for(int k=0;k<200;k++) sum += analogRead(adcpin[j]); // sum need 32 bit int.
    sensor[j] = sum/200;     
    temp[j] = cal(sensor[j]);  // Convert from ADC count to temperature.
#ifdef DEBUG     
    Serial.print("Sensor "); Serial.print(j);
    Serial.print("  : "); Serial.print(sensor[j]);
    Serial.print(", "); Serial.print(temp[j]); Serial.println("°C"); 
#endif
    Send_to_SignalK(signalk_keys[j], temp[j]);  // Send the key with its
                                                // temperature to the SignalK sever.  
  } // End for, looping through sensors
} // End function TemperatureLoop 


// The infinite loop start here, Arduino IDE has no main()

void loop() { 
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on while we transfer the data.
#ifdef DEBUG 
  Serial.println("Read voltage as ADC count");
#endif
  TemperatureLoop();
  
  delay(100); digitalWrite(LED_BUILTIN, LOW); // Turn the LED off by making the voltage LOW.
#ifdef DEBUG     
  Serial.println("wait 1 sec...");  // This high refresh rate (1sec) put a load in the SignalK server.
  delay(1000);  // 
#else
  delay(5000); // 5 sec delay, update only every 5th second.
#endif
/*
  if ((Resetcount--) == 0){
    WiFi.disconnect();
    delay(10000);
    ESP.restart(); // Reset at some intervals to keep it up at all times. 
  }
*/
}  /* End loop function, this is the only thread the controller run  */


// End sketch.
