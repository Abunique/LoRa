/*
   RadioLib SX126x Transmit Example

   This example transmits packets using SX1262 LoRa radio module.
   Each packet contains up to 256 bytes of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
   Other modules from SX126x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx126x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
    - arbitrary binary data (byte array)

*/

// include the library
#include <RadioLib.h>


//initializing the sensor communication protocol and other libraries for the fuel gauge sensor
#include <Wire.h>
#include <Arduino-MAX17055_Driver.h>


//creating a class of sensor variable
MAX17055 sensor;

// SX1262 has the following connections:
// MISO pin: 19
// MOSi pin: 23
// sck pin :18
// NSS pin:   5
// DIO1 pin:  34
// NRST pin:  32
// BUSY pin:  35
SX1262 radio = new Module(5, 34, 32, 35);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1262 radio = RadioShield.ModuleA;
String readMAH(){
    Serial.println("Get Capacity");
  float capacity = sensor.getCapacity();
  Serial.print("Capacity of plugged in battery is: ");
  Serial.print(capacity, 4);
  Serial.println(" mAH \n\n");
   return String(capacity);
  delay(2000);
}

String readvolt(){
    Serial.println("Get instantaneous voltage");
  float voltage = sensor.getInstantaneousVoltage();
  Serial.print("Instantaneous Voltage is: ");
  Serial.print(voltage, 4);
  Serial.println(" V \n\n");
   return String(voltage);
  delay(2000);  
}

String readcurrent(){
  Serial.println("Get instantaneous current");
  float current = sensor.getInstantaneousCurrent();
  Serial.print("Instantaneous Current is: ");
  Serial.print(current, 4);
  Serial.println(" mA \n\n");
 return String(current);
  delay(2000);
}

String readbattery_percentage(){
  Serial.println("Get State of Charge");
  float SOC = sensor.getSOC();
  Serial.print("State of Charge value is: %");
  Serial.println(SOC, 4);
  Serial.println("\n\n");
 return String(SOC);
  delay(2000);
}

Char Lora_data[100];

void setup() {
  Serial.begin(9600);

    //Important that Wire and Serial initialize before using the MAX17055 Library
  Wire.begin(21,22); 

  delay(1500); //Just a small delay before first communications to MAX chip
  // wait until serial port opens for native USB devices
  
  // initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin(868.0, 250.0, 7, 5, 0x34, 20, 10, 0, false);

  
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {

  // Create a String to send (data must be sent as a string, so we'll tell our computer friend that we want our integers to be set as strings)
  Lora_data = "Bat_MAH: "+readMAH()+ "MAH  BAT_VOLT:"+readvolt()+"V   BAT_CURRENT: "+readcurrent()+"A  BAT_PERCENT:"+readbattery_percentage()+ "%";
  Serial.print(F("[SX1262] Transmitting packet ... "));

    // Transmit data and units
  int state = radio.transmit(Lora_data);

Serial.println("Length of data sent:");
  Serial.println(strlen(Lora_data));

  // you can also transmit byte array up to 256 bytes long
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x56, 0x78, 0xAB, 0xCD, 0xEF};
    int state = radio.transmit(byteArr, 8);
  */

  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F("success!"));

    // print measured data rate
    Serial.print(F("[SX1262] Datarate:\t"));
    Serial.print(radio.getDataRate());
    Serial.println(F(" bps"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));

  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    // timeout occured while transmitting packet
    Serial.println(F("timeout!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }

  // wait for a second before transmitting again
  delay(1000);
}
