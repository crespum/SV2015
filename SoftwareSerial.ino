#include <string.h>
#include <stdio.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(10, 11); // RX, TX
char message[700];
char new_char;
int pos = 0; // index of the last received message character

void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);
  
  Serial.print("Welcome to the AGBAR Node\n");
  Serial.print("The Node is scanning all the BLE devices\n");

  delay(500);
  softSerial.print("AT+DISI?"); 

  delay(500);
}

void loop() {
  if (softSerial.available()) {
    new_char = softSerial.read();
    Serial.write(new_char);
    message[pos++] = new_char; // concat new received character with the previous ones

    if (strstr(message, "OK+DISCE")) { // entire message received
      parseDevices(message);
      pos = 0; // initialize message
      memset(message, 0, sizeof(message));
    }
  }
  if (Serial.available())
    softSerial.write(Serial.read());
}

void parseDevices(char *message) {
  char device[80];
  char *idx_start;
  char *idx_end;

  idx_start = strstr(message, "OK+DISC");
  while(strcmp(idx_start, "OK+DISCE")) {
    idx_end = strstr(idx_start + 7, "OK+DISC");
    memcpy(device, idx_start + 7, idx_end - idx_start - 7);
    Serial.println("New device found: ");
    Serial.println(device); // This is what shall be sent through the Internet
    idx_start = idx_end;
  }

  return;
}