
#include <Arduino.h>
#include <Wire.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;
char b[5];
String str;

const int xPin = A1;
const int yPin = A0;
const int buttonPin = 12;

// propulsion
const int motor1 = 6;
const int motor2 = 7;
const int PWMPropulsionPin = 5;

// direction
const int motor3 = 8;
const int motor4 = 9;

int xPosition ;
int yPosition ;
int buttonState ;



int sensiAvant = 600;
int sensiArriere = 400;
int sensiDroite = 600;
int sensiGauche = 400;

void writeI2CRegister8bit(int addr, int value) {
  Wire.beginTransmission(addr);
  Wire.write(value);
  Wire.endTransmission();
}

unsigned int readI2CRegister16bit(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(20);
  Wire.requestFrom(addr, 2);
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);
  pinMode(PWMPropulsionPin, OUTPUT);

  Wire.begin();
  Serial.begin(9600);
  writeI2CRegister8bit(0x20, 6); //reset
  if (!driver.init())
      Serial.println("TX init failed");
}

void loop() {
  xPosition = analogRead(xPin);
  yPosition = analogRead(yPin);
  buttonState = digitalRead(buttonPin);

  int vitesseAvant = map(yPosition, 0, sensiAvant, 0, 255);
  int vitesseArriere = map(yPosition, sensiArriere, 0, 0, 255);

  if (yPosition < sensiArriere) {
    // backward
    digitalWrite(motor1, HIGH);
    digitalWrite(motor2, LOW);
    uint16_t analog = readI2CRegister16bit(0x20, vitesseArriere);
    Serial.println("arriere");
    int data[3];
    data[0]=1;
    data[1]=analog;
    data[2]=99;
    driver.send((uint8_t*)data,6); //6 == 3*sizeof(data[0])
    if (driver.send((uint8_t*)data,6)){
    Serial.print("Sent: ");Serial.println(data[1]);
  }

  }
  else if (yPosition > sensiAvant) {
      // forward
      digitalWrite(motor1, LOW);
      digitalWrite(motor2, HIGH);
      uint16_t analog = readI2CRegister16bit(0x20, vitesseAvant);
  Serial.println("avant");
  int data[3];
  data[0]=1;
  data[1]=analog;
  data[2]=99;
  driver.send((uint8_t*)data,6); //6 == 3*sizeof(data[0])
  if (driver.send((uint8_t*)data,6)){
  Serial.print("Sent: ");Serial.println(data[1]);
}

    }

//  uint8_t data[2] = {analog, (analog >> 8)}; // extract as {lower byte, upper byte)
  // int data[3];
  // data[0]=1;
  // data[1]=analog;
  // data[2]=99;

  // Serial.print(analog); //read capacitance register
  Serial.print(", ");
  Serial.print(readI2CRegister16bit(0x20, 5)); //temperature register
  Serial.print(", ");
  writeI2CRegister8bit(0x20, 3); //request light measurement
  Serial.println(readI2CRegister16bit(0x20, 4)); //read light register

//  driver.send(data, 2);
  // driver.send((uint8_t*)data,6); //6 == 3*sizeof(data[0])

  driver.waitPacketSent();
 // uint16_t merged = (data[1] << 8) | data[0]; //merge them back together
//  Serial.print("Sent: ");Serial.println(merged);
  // Serial.print("Sent: ");Serial.println(data[1]);

  delay(1000);
}
