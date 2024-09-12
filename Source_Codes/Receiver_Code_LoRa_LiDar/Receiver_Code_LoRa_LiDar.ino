// Peer to Peer: Monitor Sensor Data using LoRa - Receiver
// SparkFun Electronics, Mariah Kelly, November 2022
// Original receive file can be found here: https://cdn.sparkfun.com/assets/learn_tutorials/1/4/9/4/Receive-v3.ino

// Include necessary libraries
#include <RadioLib.h>


// SX1262 has the following connections:
// NSS pin:   5
// DIO1 pin:  4
// NRST pin:  2
// BUSY pin:  0
SX1262 radio = new Module(5, 4, 2, 0);

void setup() {

  Serial.begin(115200);

  // initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
  //int state = radio.begin();
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

  //Serial.print(F("[SX1262] Waiting for incoming transmission ... "));

  String str;                      // Declare a data type to receive (this must be a string)
  int state = radio.receive(str);  // Receive data from transmitter

  if (state == RADIOLIB_ERR_NONE) {
    // The packet was successfully received
    Serial.println(F("Success!"));


    // Print the data of the packet to the serial monitor
    Serial.print(F("[SX1262] Data:\t\t"));
    Serial.println(str);

    // Print the RSSI (Received Signal Strength Indicator) of the last received packet
    Serial.print(F("[SX1262] RSSI:\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    // Print the SNR (Signal-to-Noise Ratio) of the last received packet
    Serial.print(F("[SX1262] SNR:\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    // Timeout occurred while waiting for a packet
   // Serial.println(F("Timeout!"));  // We won't print this to the screen since the Success/data print will freeze on its own if a timeout occurs

  } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    // The packet was received, but is malformed
    Serial.println(F("CRC error!"));

  } else {
    // Some other error occurred
    Serial.print(F("Error"));
    Serial.println(state);
  }
}
