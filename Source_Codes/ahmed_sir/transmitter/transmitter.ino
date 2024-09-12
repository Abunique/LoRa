//#include <Wire.h>
#include "SparkFun_TMF882X_Library.h"  //http://librarymanager/All#SparkFun_Qwiic_TMPF882X
#include <SPI.h>
#include <LoRa.h>

#define LORA_SS     5
#define LORA_RESET  2
#define LORA_DIO1   4

#define FREQUENCY 868E6 // LoRa frequency 868 MHz

String data1;
SparkFun_TMF882X myTMF882X;


// Structure to hold the measurement results - this is defined by the TMF882X SDK.

static struct tmf882x_msg_meas_results myResults;

//TMF8x21 sensor; // Create a TMF8x21 object

void setup() {
  Serial.begin(9600);
  
 // Initialize the TMF882X device
  if (!myTMF882X.begin()) {
    Serial.println("Error - The TMF882X failed to initialize - is the board connected?");
    while (1)
     ;
  } else
    Serial.println("TMF882X started.");

  
  // Initialize LoRa module
  SPI.begin(18, 19, 23, 5); // SCK, MISO, MOSI, SS
  LoRa.setPins(LORA_SS, LORA_RESET, LORA_DIO1);
  if (!LoRa.begin(FREQUENCY)) {
    Serial.println("LoRa initialization failed. Check your connections.");
    while (true); // Stop further execution
  }
  Serial.println("LoRa Initialized");
}

void loop() {
    myTMF882X.startMeasuring(myResults);
  for (int i = 0; i < myResults.num_results; ++i) {
    if(myResults.results[i].channel == 5) data1= String(myResults.results[i].distance_mm)+"mm"; 
  }
  myTMF882X.stopMeasuring();
//  Serial.println(data);
  
  // Send data via LoRa
  LoRa.beginPacket();
  LoRa.print(data1);
  LoRa.endPacket();
  Serial.println("Data Sent: " + data1);
  delay(1000); // Adjust delay as needed
}
