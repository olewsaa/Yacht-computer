/*
 *  This sketch uses a ESP 32 and ADS1115 to measure voltage and current and 
 *  sends messages to a SignalK server
 *  
 *  Select board ESP32 , NodeMCU-32S
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
 *  
 */


// Analog Input on the ESP32, this board has many analog inputs.
#define ANALOG_PIN_0 36  // Pin GPIO 36
#define ANALOG_PIN_3 39  // Pin GPIO 39
#define ANALOG_PIN_6 34  // Pin GPIO 34
int analog_value = 0;

 
// Including I2C and ADC board ADS1115 
#include <Wire.h>
#include <Adafruit_ADS1015.h>

// Including WiFi stuff
#include <WiFi.h>
#include <WiFiUdp.h>

/* WiFi network name and password */
const char * ssid = "TeamRocketHQ";
const char * pwd = "blackpearl";

// IP address to send UDP data to.
// it can be ip address of the server or 
// Settings for SignalK port and SignalK server.
const char * udpAddress = "192.168.1.160";
const int udpPort = 55557;


// Instanciate two of a maximum of four (48, 49, 4A & 4B) ads objects.
Adafruit_ADS1115 ads1(0x48); 
Adafruit_ADS1115 ads2(0x49); 

// Instanciate WiFi object
WiFiUDP Udp;


void setup() {
    pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
    Serial.begin(115200);

  //Connect to the WiFi network
    WiFi.begin(ssid, pwd);
    Serial.println("");

  // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  //This initializes udp and transfer buffer
    Udp.begin(udpPort);    
    delay(10);

  // Set up the first ADS 1115 gain
    ads1.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 0.0625mV
    ads1.begin();  
    Serial.println("ADS 1115 nunmber 1 is set up");
    ads2.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 0.0625mV
    ads2.begin(); 
    Serial.println("ADS 1115 number  2 is set up");
 
} /* End setup */



void Send_to_SignalK(String path, double value){
    String cmd;
    char valuestring[6];
    int len;

  // Prepage the SignalK string to send.
    cmd = "{\"updates\": [{\"$source\": \"ESP32\",\"values\":[ {\"path\":\"";
    cmd += path; cmd += "\","; cmd += "\"value\":";
    dtostrf(value,3,2,valuestring); // Convert double to a string
    cmd += valuestring;
    cmd += "}]}]}\0";
    len=cmd.length(); // Get the lenght of message, needed by the udp write.
    Serial.println(cmd); 
    //Serial.println(len);
    
  //data will be sent to server
    uint8_t buffer[120]; // UDP write will only write sequence of bytes. 
    for(int j=0;j<len;j++){buffer[j]=cmd[j];} // Convert from char t bytes. 
    //memcpy(buffer,&cmd,len); // Bør virke, ikke testet.
    //send cmd to server
    Udp.beginPacket(udpAddress, udpPort);
    Udp.write(buffer, len); 
    Udp.endPacket();
    //Serial.println(cmd); 
    delay(10); 
   
} /* End Send_to_SignalK */



void Measure_and_Send_U(int inputno) {
    double U; 
    int val,offset;
    /*
     * Voltage is measured using the single ADC input on the ESP32 chip.
     * However, this routine is generic and it can easily change to ADS1115. 
     */
    if (inputno==1) { 
      offset=-1;    // Differ from board to board, make your own calibration.
      val=analogRead(ANALOG_PIN_0);  // Read the analog input on the ESP32, ADC1_0
      val+=offset;
      U=((double)val/(double)4095.0)*19.3;  // Calibrated using a multimeter, check this value carefully. 
      Send_to_SignalK("electrical.service.voltage",U);
    }
    if (inputno==2) {
      offset=-1;    // Differ from board to board, make your own calibration.
      val=analogRead(ANALOG_PIN_3);  // Read the analog input on the ESP32, ADC1_3
      val+=offset;
      U=((double)val/(double)4095.0)*3.45*2.0; // calibate! max is 3.3V.
      Send_to_SignalK("electrical.5V.voltage",U);
    }   
    if (inputno==3) {
      offset=-1;    // Differ from board to board, make your own calibration.
      val=analogRead(ANALOG_PIN_6);  // Read the analog input on the ESP32, ADC1_6
      val+=offset;
      U=((double)val/(double)4095.0)*3.3; // calibate! max is 3.3V.
      Send_to_SignalK("electrical.3V3.voltage",U);
    }   
} /* End Measure_and_Send_U */

     
void Measure_and_Send_I(int inputno) {
    double I;
    int val, offset;

    if (inputno==1) {      
      // Read ADC and calculate the equivalt current in Amps.
      offset=-5;    // Differ from sensor to sensor, calibration is needed.
      val=ads1.readADC_Differential_0_1();   // There are two possibilities in differential mode, 01 and 23.
      Serial.println(val);
      val+=offset;
      I = (double)val/(double)32766*12.52; // Calibrated with a multimeter.
      Send_to_SignalK("electrical.service.HKS2010",I);
    }
    if (inputno==2) {
      offset=-130;
      val=ads1.readADC_Differential_2_3();   // There are two possibilities in differential mode, 01 and 23.
      Serial.println(val);
      val+=offset;
      I = (double)val/(double)32766*161.5; // Calibrated with a multimeter.
      Send_to_SignalK("electrical.service.ACS712",I);
    }
    if (inputno==3) {
      offset=-300;
      val=ads2.readADC_Differential_0_1();   // There are two possibilities in differential mode 01 and 23.
      Serial.println(val);
      val+=offset;
      I = (double)val/(double)32766*40.0; // Calibrated with a multimeter.
      Send_to_SignalK("electrical.service.WCS1800",I);
    }
    if (inputno==4) {
      offset=-160;
      val=ads2.readADC_Differential_2_3();   // There are two possibilities in differential mode 01 and 23.
      Serial.println(val);
      val+=offset;
      I = (double)val/(double)32766*39.5; // Calibrated with a multimeter.
      Send_to_SignalK("electrical.service.HCS-ES5",I);
    }
}  /* End Measure_and_Send_I */



// The endless loop start here :

void loop() {   
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on while we transfer the data.
    // Send the voltages:
    for (int j=1;j<4;j++) Measure_and_Send_U(j);  // Get the voltage and send it to SignalK.
    // Send the currents :  
    for (int j=1;j<5;j++)  Measure_and_Send_I(j);  // Get the current and send it to SignalK.
   
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage LOW.
    
    Serial.println("wait 2 sec...");   // This high refresh rate put a load in the SignalK server.
    delay(2000);
}  /* End infinite loop */

