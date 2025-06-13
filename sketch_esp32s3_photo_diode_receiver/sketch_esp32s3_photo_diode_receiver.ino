#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

#define BUFFER_SIZE 12
char lastChars[BUFFER_SIZE];  // Circular buffer
int indexG = 0; 

// Photodiode Receiver (ESP32-S3)
const int PHOTODIODE_PIN = 3;
const int threshold = 4000; // Adjust based on ambient light
const int bitDelay = 250000;  // Microseconds (must match transmitter)
char zero = 0;



void setup() {
  Serial.begin(115200);
  Serial.println("starting");

  tft.init();
  tft.setRotation(1);  // 0–3; try different values if display is upside down
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.setCursor(20, 0);
  tft.println("# RECEIVER #");
  
  tft.setCursor(0, 60);
  tft.println("booting up...");

  for (int i = 0; i < BUFFER_SIZE; i++) {
    lastChars[i] = ' ';
  }


}

bool readBit() {
  int value = analogRead(PHOTODIODE_PIN);
  return value > threshold;
}

char readByte() {
  while (!readBit()); // Wait for start bit (HIGH)
  delayMicroseconds(bitDelay);

  char c = 0;
  for (int i = 0; i < 8; i++) {
    bool bit = readBit();
    c |= (bit << i);
    delayMicroseconds(bitDelay);
  }

  // Wait for stop bit
  delayMicroseconds(bitDelay);

  return c;
}

void loop() {
  char received = readByte();
  
  if(received != zero){
    //Serial.println(received);

    lastChars[indexG] = received;
    indexG = (indexG + 1) % BUFFER_SIZE;

    // Print full buffer
    tft.fillRect(0, 60, 320, 60, TFT_BLACK);
    //tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 60);

    Serial.print("Last 10 chars: ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
      int idx = (indexG + i) % BUFFER_SIZE;
      Serial.print(lastChars[idx]);
      tft.print(lastChars[idx]);
    }
    Serial.println();
  }

  // int r_int = received;
  // if( r_int > 0){
  //   Serial.println(received);
  // }

  // int value = analogRead(PHOTODIODE_PIN);
  // Serial.println(value);

  // delay(50);

}
