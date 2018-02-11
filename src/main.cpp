//https://forum.arduino.cc/index.php?topic=331312.0
#include <Arduino.h>
#include <Wire.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;

// joystick
const int xPin = A1;
const int yPin = A0;
const int buttonPin = 11;

// direction
const int NO_ORDER = 0;
const int FORWARD = 1;
const int BACKWARD = 2;
const int TURN_RIGHT = 4;
const int TURN_LEFT = 8;

const int sensiAvant = 600;
const int sensiArriere = 400;
const int sensiDroite = 600;
const int sensiGauche = 400;

// variables
int xPosition ;
int yPosition ;
int buttonState ;


void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  Wire.begin(); // needed ?
  Serial.begin(9600);
  if (!driver.init())
      Serial.println("TX init failed");
}

void loop() {
  xPosition = analogRead(xPin);
  yPosition = analogRead(yPin);
  buttonState = digitalRead(buttonPin);

  int vitesseAvant = map(yPosition, 0, sensiAvant, 0, 255);
  int vitesseArriere = map(yPosition, sensiArriere, 0, 0, 255);

  int propulsion;
  int vitesse;
  int direction;
  int braquage;

  if (vitesseAvant>0) {
    propulsion=FORWARD;
    vitesse=vitesseAvant;
  }
  else if (vitesseArriere>0) {
    propulsion=BACKWARD;
    vitesse=vitesseArriere;
  }
  else {
    propulsion=NO_ORDER;
    vitesse=0;
  }

  direction = TURN_RIGHT;

  int instructions[4]; //avant/arriere, vitesse, droite/gauche, vitesse

  // force for test
  propulsion = 1;
  vitesse = 128;
  direction = 8;
  braquage = 155;

  instructions[0]=propulsion;
  instructions[1]=vitesse;
  instructions[2]=direction;
  instructions[3]=braquage;

  driver.send((uint8_t*)instructions,8); //8 == 4*sizeof(int)
  driver.waitPacketSent(); // needed ?

  Serial.println(instructions[0]);
  Serial.println(instructions[1]);
  Serial.println(instructions[2]);
  Serial.println(instructions[3]);
  Serial.println("-------SENT------------");

  delay(1000);
}
