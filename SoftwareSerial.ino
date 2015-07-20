#include <string.h>
#include <stdio.h>
#include <SoftwareSerial.h>
#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information
#include "GoogleAccount.h" // contains Google credentials

SoftwareSerial softSerial(10, 11); // RX, TX
char message[700];
char new_char;
int pos = 0; // index of the last received message character

void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);
  Bridge.begin();

  Serial.println("Welcome to the AGBAR Node");
  Serial.println("The Node is scanning all the BLE devices");

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
    memcpy(device, idx_start + 7, idx_end - idx_start - 7);//what if different length
    Serial.print("New device found: ");
    Serial.println(device); // This is what shall be sent through the Internet
    String sDevice(device);
    appendRow(sDevice)
    idx_start = idx_end;
  }

  return;
}

void appendRow(String message) {
    // we need a Process object to send a Choreo request to Temboo
    TembooChoreo appendRowChoreo;

    // invoke the Temboo client
    // NOTE that the client must be reinvoked and repopulated with
    // appropriate arguments each time its run() method is called.
    appendRowChoreo.begin();
    
    // set Temboo account credentials
    appendRowChoreo.setAccountName(TEMBOO_ACCOUNT);
    appendRowChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    appendRowChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // identify the Temboo Library choreo to run (Google > Spreadsheets > AppendRow)
    appendRowChoreo.setChoreo("/Library/Google/Spreadsheets/AppendRow");
    
    // set the required Choreo inputs
    // see https://www.temboo.com/library/Library/Google/Spreadsheets/AppendRow/ 
    // for complete details about the inputs for this Choreo
    
    // set Google account
    appendRowChoreo.addInput("ClientID", GOOGLE_CLIENT_ID);
    appendRowChoreo.addInput("ClientSecret", GOOGLE_CLIENT_SECRET);
    appendRowChoreo.addInput("RefreshToken", GOOGLE_REFRESH_TOKEN);
    appendRowChoreo.addInput("SpreadsheetTitle", SPREADSHEET_TITLE);

    // add the RowData input item
    appendRowChoreo.addInput("RowData", message);

    // run the Choreo and wait for the results
    // The return code (returnCode) will indicate success or failure 
    unsigned int returnCode = appendRowChoreo.run();

    // return code of zero (0) means success
    if (returnCode == 0) {
      Serial.println("Device info uploaded to Internet");
    } else {
      // return code of anything other than zero means failure  
      // read and display any error messages
      Serial.println("Unable to upload the device info to Internet")
      while (appendRowChoreo.available()) {
        char c = appendRowChoreo.read();
        Serial.print(c);
      }
    }

    appendRowChoreo.close();  
}