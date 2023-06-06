byte com = 0;
#define red 2
#define yellow 4
#define green 3

void setup() {
  Serial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  Serial.write(0xAA);
  Serial.write(0x37);
  delay(1000);
  Serial.write(0xAA);
  Serial.write(0x21);
}

void loop() {
  while (Serial.available()) {
    com = Serial.read();

    if (com == 0x11) {  // Month
      // digitalWrite(green, HIGH);
      Serial.println("Alarm");
    } else if (com == 0x12) {  // Temperature
      // digitalWrite(yellow, HIGH);
      Serial.println("Temperature");
    } else if (com == 0x13) {  // Humidity
      // digitalWrite(red, HIGH);
      Serial.println("Humidity");
    } else if (com == 0x14) {  // command 4 is for Servo motor
      digitalWrite(green, LOW);
      Serial.println("Date");
    } else if (com == 0x15) {  // Date
      digitalWrite(red, LOW);
      Serial.println("Month");
    }
  }
}
