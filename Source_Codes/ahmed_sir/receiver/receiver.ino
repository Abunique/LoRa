#include <SPI.h>
#include <LoRa.h>

#define LORA_SS     5
#define LORA_RESET  2
#define LORA_DIO0   4

#define FREQUENCY     868E6  // LoRa frequency 868 MHz for India 
#define BANDWIDTH     125E3  // Bandwidth: 125 kHz
#define SPREADING_FACTOR 12  // Spread factor: 12
#define PREAMBLE_LENGTH  8   // Preamble length: 8 symbols
#define CODING_RATE   5      // Coding rate: 4/5
#define CRC_ENABLED   true   // Enable CRC

void setup() {
  Serial.begin(9600);
  
  // Initialize LoRa module
  SPI.begin(5, 19, 27, 18); // SCK, MISO, MOSI, SS
  LoRa.setPins(LORA_SS, LORA_RESET, LORA_DIO0);
  if (!LoRa.begin(FREQUENCY)) {
    Serial.println("LoRa initialization failed. Check your connections.");
    while (true); // Stop further execution
  }
  LoRa.setSpreadingFactor(SPREADING_FACTOR);
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setPreambleLength(PREAMBLE_LENGTH);
  LoRa.setCodingRate4(CODING_RATE);
  LoRa.enableCrc();
  Serial.println("LoRa Initialized");
}

void loop() {
  // Receive packet
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    // Read packet
    while (LoRa.available()) {
      String receivedData = LoRa.readString();
      Serial.println("Received data: " + receivedData);
    }
  }
}
