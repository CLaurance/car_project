#include <Arduino.h>




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


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);
  pinMode(PWMPropulsionPin, OUTPUT);

}

void loop() {
  xPosition = analogRead(xPin);
  yPosition = analogRead(yPin);
  buttonState = digitalRead(buttonPin);

  int vitesseAvant = map(yPosition, 0, sensiAvant, 0, 255);
  int vitesseArriere = map(yPosition, sensiArriere, 0, 0, 255);

  Serial.print("X: ");
  Serial.print(xPosition);
  Serial.print(" | Y: ");
  Serial.print(yPosition);
  Serial.print(" | Button: ");
  Serial.println(buttonState);

  delay(100); // add some delay between reads

  if (yPosition < sensiArriere) {
    // backward
    digitalWrite(motor1, HIGH);
    digitalWrite(motor2, LOW);
    analogWrite(PWMPropulsionPin, vitesseArriere);
Serial.println("arriere");
  }

else if (yPosition > sensiAvant) {
    // forward
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, HIGH);
    analogWrite(PWMPropulsionPin, vitesseAvant);
Serial.println("avant");
  }
else {
//   free wheel
    digitalWrite(motor1, LOW);
   digitalWrite(motor2, LOW);
    Serial.println("roue libre");
 }
}
