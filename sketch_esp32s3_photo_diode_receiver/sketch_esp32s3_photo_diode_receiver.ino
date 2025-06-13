// Photodiode Receiver (ESP32-S3)
const int PHOTODIODE_PIN = 3;
const int threshold = 1800; // Adjust based on ambient light
const int bitDelay = 1000;  // Microseconds (must match transmitter)

void setup() {
  Serial.begin(115200);
  Serial.println("starting");
}

bool readBit() {
  int value = analogRead(PHOTODIODE_PIN);
  return value > threshold;
}

char readByte() {
  while (readBit()); // Wait for start bit (LOW)
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
  //char received = readByte();
  //Serial.print(received);
  int value = analogRead(PHOTODIODE_PIN);
  Serial.println(value);

  delay(50);

}
