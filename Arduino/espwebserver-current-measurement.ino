

#include <SoftwareSerial.h> 
#include <WiFiEsp.h>   // ESP module library
#include <EmonLib.h>   // Include Emon Library
#include <TM1638plus.h>    // TM1639 Display module

SoftwareSerial Serial1(6, 7);  // Create a serial instance using pin 6 & 7.


char ssid[] = "Mynet";
char passwd[] = "password";
int inPinI = 5; // Analog in pin 
int status = WL_IDLE_STATUS; 

WiFiEspServer server(80); // Create a server instance 
EnergyMonitor emon1;      // Create an instance of the energy monitor type
TM1638plus module(8, 10, 9);   // define a module on data pin 9, clock pin 10 and strobe pin 8

void setup() {
  pinMode(inPinI,INPUT);
  
  Serial.begin(115200); // Serial over USB to Arduino IDE
  Serial1.begin(9600);  // Serial communication to ESP module

  WiFi.init(&Serial1); // Set up serial communication to ESP 8266
  emon1.current(inPinI,0);  // Current: input pin, calibration.

  Serial.println("Start");
  delay(500);


  if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("Wifi shield not present");
      while (true) { 
          delay(500);
          Serial.println("No wifi module found");
      }
  }

  Serial.println("Trying to connect");
  while (status != WL_CONNECTED) {
      Serial.println("Attempt to conncect");
      Serial.println("ssid");

      status = WiFi.begin(ssid, passwd);
  }

  Serial.println("Connected with the following information:");
  printWifiStatus();
  

  Serial.println("Start the web server");
  server.begin();
  
}


void loop() {

  WiFiEspClient client = server.available();

  double Irms, power, voltage=230.0;
  int pwdispl; 

  Irms = emon1.calcIrms(300);    // 1480 samples per sec
  pwdispl = (int) (Irms-0.)*voltage; // Read about 1.0 when trafo off
  power = (Irms-0.)*voltage;
  char cstr[8];
  sprintf(cstr, "%8d", pwdispl);
  module.displayText(cstr);
  //Serial.println(String(cstr));

  if (client) {
    Serial.println("New client");

    boolean currentLineIsBlank = true;
 
    while(client.connected()) {
        if (client.available()) {
            char c = client.read();
            //Serial.write(c);

            if (c == '\n' && currentLineIsBlank) {
                //Serial.println(power);     
                //Serial.println("Sending response");
                client.print(
                  "HTTP/1.1 200 OK\r\n"
                  "Content-Type: text/html\r\n"
                  "Connection: close\r\n"
                  "\r\n");
                client.print("<!DOCTYPE HTML>\r\n");
                client.print("<html>\r\n");
                client.print("<head>\r\n");
                client.print("<title>H4K</title>\r\n");
                client.print("</head)>\r\n");
                client.print("<body>\r\n");
                client.print("<h1>H4k effekt</h1>\r\n");
                client.print("<p>Effekt :</p>\r\n");
                client.print(power);
                client.print(" Watt\r\n");
                client.print("</body>\r\n");
                client.print("</html>\r\n");
                break;
            }
            if (c == '\n') {
                currentLineIsBlank = true;
            }
            else if (c != '\r') {
                 currentLineIsBlank = false;
            } 
        }
    }
    delay(10);
    client.stop();
    //Serial.println("Client disconnected");
  }

}




void printWifiStatus() {
    Serial.print("SSID "); Serial.println(WiFi.SSID());
    IPAddress ip = WiFi.localIP();
    Serial.print("IP address "); Serial.println(ip);
}


