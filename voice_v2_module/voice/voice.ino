byte com = 0;

int led1=2;
int led2=3;
int led3=4;
int led4=5;
int led5=6;
void setup() {
  
Serial.begin(9600);

Serial.write(0xAA);

Serial.write(0x37);

delay(1000);

Serial.write(0xAA);

Serial.write(0x21);
pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
pinMode(led3,OUTPUT);
pinMode(led4,OUTPUT);
pinMode(led5,OUTPUT);

}

void loop() {

  while(Serial.available()) {

  com = Serial.read();

  switch(com) {

      case 0x11:   //command 1 is for Green LED 

      digitalWrite(led1,HIGH);

      break;

      case 0x12:  //command 2 is for Red LED

      digitalWrite(led2,HIGH);

      break;

      case 0x13:  //command 3 is for Yellow LED

      digitalWrite(led3,HIGH);

      break;

      case 0x14:  //command 4 is for Servo motor
      
      digitalWrite(led1,LOW);

      break;

      case 0x15:  //command 5 is for Servo Motor
      
      digitalWrite(led2,LOW);

      break;

            }
      }
}
