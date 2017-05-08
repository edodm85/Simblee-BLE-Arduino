/*
 * Author: Edoardo De Marchi
 * Date: 08-05-2017
 * Notes: Example code for Simblee bluetooth communication
*/

#include <SimbleeBLE.h>


boolean stringComplete = false;
char buf[20];
int i = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Waiting for connection...");
  
  SimbleeBLE.deviceName = "Simblee";
  SimbleeBLE.advertisementData = "data";
  SimbleeBLE.advertisementInterval = MILLISECONDS(300);
  SimbleeBLE.txPowerLevel = -20;  // (-20dbM to +4 dBm)

  // start the BLE stack
  SimbleeBLE.begin();
}

void loop()
{
  // print the string when a newline arrives:
  if (stringComplete) 
  {
    Serial.println(buf);
    
    // send is queued (the ble stack delays send to the start of the next tx window)
    while (! SimbleeBLE.send(buf, i))
      ;  // all tx buffers in use (can't send - try again later)

    memset(buf, 0, sizeof(buf));
    i = 0;
    stringComplete = false;
  }
}


void SimbleeBLE_onConnect()
{
  Serial.println("Connect");
}

void SimbleeBLE_onDisconnect()
{
  Serial.println("Disconnect");
}


void SimbleeBLE_onReceive(char *data, int len)
{
  printf("%s\n", data);
}





void serialEvent() 
{
  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    buf[i] += inChar;
    i++;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n' || i == sizeof(buf)) 
    {
      stringComplete = true;
    }
  }
}
