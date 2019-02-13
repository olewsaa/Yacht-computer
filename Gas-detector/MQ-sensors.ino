
/*
  Gas alarm Arduino skectch.

  For Arduino NANO (Chineese make)
  Select Arduino NANO board
  Processor ATmega328P (old bootloader)
  115200 baud
  
*/

int value;
int Alarm=0;
const int analog_ip0 = A0;   // Naming analog input pin
const int analog_ip1 = A1;   // Naming analog input pin

void setup() {
  Serial.begin(115200);
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  // initalize a pin for output
  // pinMode();
}

void loop() {
  digitalWrite(13, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait n ms
  digitalWrite(13, LOW);   // turn the LED off by making the voltage LOW
  delay(100);              // wait n ms 
 
  value = analogRead(analog_ip0);
  if (value>500) Alarm=1;
  //Serial.print("Detector value MQ2 :"); 
  //Serial.print(value);
 
  value = analogRead(analog_ip1);
  if (value>500) Alarm=1;
  //Serial.print("  Detector value MQ9 :"); 
  //Serial.print(value);
  //Serial.println();
  if (Alarm>0) Serial.println("ALARM ALARM ALARM ALARM");
  /*
    Insert code to set of some audio alarm, buzzer, bell or klaxon.
  */
}
/*

The Alarm will stay until the reset button or power off.
Maybe add a reset button to the box.

*/

