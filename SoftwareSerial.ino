#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
String data = "";
char character;
/*
void setup()  
{
  // Open serial cOmmunications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("AGBAR Node\n");
  Serial.println("Receiving data from Agbar BLE Module \n");

  mySerial.begin(9600);
}
/*
void loop() // run over and over
{
  while(mySerial.available()==0)
  {}

  char character;
  while(mySerial.available()>0)
   {
     character = mySerial.read();
     mySerial.write(character);
   }

   data = data + character;

   if (character == 13) {  
     mySerial.print("Received: ");
     mySerial.println(data);
     data = "";
   } 
}*/

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is completeString inputString = "";         // a string to hold incoming data

float time;
float oldTime;

void setup() {
// initialize serial:
Serial.begin(9600);
pinMode(4, OUTPUT);
digitalWrite(4, LOW);
}

void HM10Reset(){
  Serial.print("AT+RESET");
  delay(220);
  digitalWrite(4, HIGH);
  delay(100);
  digitalWrite(4, LOW);
  delay(150);
}

void loop() {
  // print the string when a newline arrives:
  Serial.write("AT+CON883314DD8016");
  delay(150);
  Serial.print("From first node!  Seconds ago last here: ");
  time = millis();
  Serial.println((time-oldTime)/1000);
  oldTime = millis();
  delay(250);
  HM10Reset();
  Serial.write("AT+CON883314DD8015");
  delay(150);  
  Serial.println("From second node!");
  delay(150);
  HM10Reset();
}

void serialEvent() {
  while (Serial.available()) {
  // get the new byte:
  char inChar = (char)Serial.read(); 
  // add it to the inputString:
  delay(50);
  inputString += inChar;
  stringComplete = true;
}

}
    
void loop() // run over and over
{
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());  
}
