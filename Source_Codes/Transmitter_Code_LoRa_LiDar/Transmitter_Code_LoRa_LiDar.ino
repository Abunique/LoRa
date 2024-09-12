// Peer to Peer: Monitor Sensor Data using LoRa - Transmitter
// SparkFun Electronics, Mariah Kelly, November 2022
// Original transmit file can be found here: https://cdn.sparkfun.com/assets/learn_tutorials/1/4/9/4/Transmit-v3.ino

// Include necessary libraries
#include <RadioLib.h>                  // Transmit & receive - https://github.com/jgromes/RadioLib/archive/refs/heads/master.zip
#include "SparkFun_TMF882X_Library.h"  //http://librarymanager/All#SparkFun_Qwiic_TMPF882X



// SX1262 has the following connections:
// NSS pin:   5
// DIO1 pin:  4
// NRST pin:  2
// BUSY pin:  0
SX1262 radio = new Module(5, 4, 2, 0);

SparkFun_TMF882X myTMF882X;

 String mydata ; 


// Structure to hold the measurement results - this is defined by the TMF882X SDK.

static struct tmf882x_msg_meas_results myResults;

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("");

  Serial.println("In setup");
  Serial.println("==============================");

  // Initialize the TMF882X device
  if (!myTMF882X.begin()) {
    Serial.println("Error - The TMF882X failed to initialize - is the board connected?");
    while (1)
      ;
  } else
    Serial.println("TMF882X started.");

  // The device is now ready for operations

  // Initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));

  int state = radio.begin(868.0, 250.0, 7, 5, 0x34, 20, 10, 0, false);
  

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
}

void loop() {


  Serial.println(F("[SX1262] Transmitting packet ... "));

 
//  myTMF882X.startMeasuring(myResults);
//  for (int i = 0; i < myResults.num_results; ++i) {
//    if(myResults.results[i].channel == 5) mydata= String(myResults.results[i].distance_mm)+"mm"; 
//  }
//  myTMF882X.stopMeasuring();
  Serial.println(mydata);
// mydata= "success";
  // Transmit data and units
  int state = radio.transmit(mydata);

  if (state == RADIOLIB_ERR_NONE) {
    // The packet was successfully transmitted
    Serial.println(F("Success!"));

    // Print measured data rate
    Serial.print(F("[SX1262] Datarate:\t"));
    Serial.print(radio.getDataRate());
    Serial.println(F(" bps"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // The supplied packet was longer than 256 bytes
    Serial.println(F("Packet size too long!"));

  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    // Timeout occurred while transmitting packet
    Serial.println(F("Timeout!"));

  } else {
    // Some other error occurred
    Serial.print(F("Error"));
    Serial.println(state);
  }

  // Wait for a second before transmitting again
  delay(1000);
}
