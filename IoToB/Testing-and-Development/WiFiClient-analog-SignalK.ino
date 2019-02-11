/*
 *  This sketch sends a message to the SignalK server
 *   Ole W. Saastad, 15th Oct 2017.
 */
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>

ESP8266WiFiMulti WiFiMulti;
WiFiUDP Udp;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("TeamRocketHQ", "password");

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);
} /* End setup */


void loop() {
    const uint16_t port = 55557;
    const char * host = "192.168.1.160"; // ip or dns
    double U;
    int val;
    char Us[6];
    String cmd;


    val=analogRead(A0);  // Read the analog input on the ESP2866.
    U=((double)val/(double)1023.0)*19.576;  // Calibrated using a multimeter, check this value carefully. 
    dtostrf(U,3,2,Us);
    /*
    Serial.println(Us);
    Serial.println(U,2);
    Serial.print("SignalK command");
    */
    cmd =  "{\"updates\": [{\"$source\": \"ESP8266\",\"values\":[ {\"path\":\"electrical.service.voltage\",\"value\":"; 
    cmd += Us; cmd+="}]}]}\0"; // Not sure if NULL is needed or not, but it's good custom to terminate with NULL:
    /*   
    Serial.println(cmd);
    Serial.println(cmd.length());
    Serial.print("connecting to using UDP"); Serial.print(host); Serial.print(":");Serial.println(port);
    */
    
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on while we transfer the data.
    // Some tricks to convert from String to character array as Udp.write will not accept string type argument.
    char cmd2[cmd.length()+1]; // Counting from zero, add one.
    Udp.beginPacket(host,port); // Connect to to server and prepare for UDP transfer.
    strncpy(cmd2,cmd.c_str(),sizeof(cmd2));  // Convert from String to array of characters. 
    // Serial.println(cmd2); Serial.print(" Message har length: "); Serial.println(sizeof(cmd2));
    Udp.write(cmd2); // Send the message to the SignalK server. 
    Udp.endPacket(); // End the connection.
    
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH.
    
    Serial.println("wait 2 sec...");
    delay(2000);
}  /* End infinite loop */

