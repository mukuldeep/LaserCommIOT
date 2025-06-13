// Laser Transmitter (ESP32-S3)
const int LASER_PIN = 44;
const char* message = "Hello via LASER!";
const int bitDelay = 1000; // Microseconds per bit

void setup() {
  pinMode(LASER_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("starting");
}

void sendByte(char c) {
  // Start bit
  digitalWrite(LASER_PIN, LOW);
  delayMicroseconds(bitDelay);

  // Send each bit LSB first
  for (int i = 0; i < 8; i++) {
    bool bit = (c >> i) & 1;
    digitalWrite(LASER_PIN, bit ? HIGH : LOW);
    delayMicroseconds(bitDelay);
  }

  // Stop bit
  digitalWrite(LASER_PIN, HIGH);
  delayMicroseconds(bitDelay);
}

void loop() {

  Serial.println("sending ");

  // for (int i = 0; message[i]; i++) {
  //   sendByte(message[i]);
  // }
  digitalWrite(LASER_PIN,HIGH);
  delay(2000); // Wait and send again
  digitalWrite(LASER_PIN, LOW);
  delay(2000);

}
