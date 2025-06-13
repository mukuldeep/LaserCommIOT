#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

#define BUFFER_SIZE 12
char lastChars[BUFFER_SIZE];  // Circular buffer
int indexG = 0; 

// Laser Transmitter (ESP32-S3)
const int LASER_PIN = 44;
const char* message = " Hello World!   SUBSCRIBE to DIY Days :) ";
const int bitDelay = 250000; // Microseconds per bit
//const int byteDelay = 1000; // Microseconds per byte



void setup() {
  pinMode(LASER_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("starting");

  tft.init();
  tft.setRotation(1);  // 0–3; try different values if display is upside down
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(20, 0);
  tft.println("# SENDER #");
  
  tft.setCursor(0, 60);
  tft.println("booting up...");

}

void sendByte(char c) {
  // Start bit
  digitalWrite(LASER_PIN, HIGH);
  delayMicroseconds(bitDelay);

  // Send each bit LSB first
  for (int i = 0; i < 8; i++) {
    bool bit = (c >> i) & 1;
    digitalWrite(LASER_PIN, bit ? HIGH : LOW);
    delayMicroseconds(bitDelay);
  }

  // Stop bit
  digitalWrite(LASER_PIN, LOW);
  delayMicroseconds(bitDelay);
  //delayMicroseconds(byteDelay);

}

void loop() {

  Serial.println("sending ");

  for (int i = 0; message[i]; i++) {
    sendByte(message[i]);
    Serial.println(message[i]);

    //to sync 
    lastChars[indexG] = message[i];
    indexG = (indexG + 1) % BUFFER_SIZE;

    // Print full buffer
    //tft.fillScreen(TFT_BLACK);
    tft.fillRect(0, 60, 320, 60, TFT_BLACK);
    tft.setCursor(0, 60);

    Serial.print("Last 10 chars: ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
      int idx = (indexG + i) % BUFFER_SIZE;
      Serial.print(lastChars[idx]);
      tft.print(lastChars[idx]);
    }
    Serial.println();


  }
  delay(5000);


  // digitalWrite(LASER_PIN,HIGH);
  // delay(2000); // Wait and send again
  // digitalWrite(LASER_PIN, LOW);
  // delay(2000);

}
