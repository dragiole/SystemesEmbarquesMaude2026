#include <HCSR04.h>
#include <AccelStepper.h>
#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);
// Définir le type de moteur, les broches IN1-IN3-IN2-IN4
#define MOTOR_INTERFACE_TYPE 4

#define IN_1 8
#define IN_2 9
#define IN_3 10
#define IN_4 11
#define TRIGGER_PIN 3
#define ECHO_PIN 4

int attente = 2000;
long numeroEtd = 1970812;
int distanceMoteur;
int distanceMoteurMax = 962;
int distanceMoteurMin = 57;
int degreMoteur;
int degreMoteurMax = 170;
int degreMoteurMin = 10;
int vitesseMoteur = 1000;
int accelMoteur = 250;
int maxSpeed = 1000;
int distance;
int distanceMin = 30;
int distanceMax = 60;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const long interval = 100;
const long interval2 = 50;

HCSR04 hc(TRIGGER_PIN, ECHO_PIN);
AccelStepper myStepper(MOTOR_INTERFACE_TYPE, IN_1, IN_3, IN_2, IN_4);

enum Etat {
  FERMEE,
  OUVERTE
};
Etat etatCourant;

void setup() {
  myStepper.setMaxSpeed(vitesseMoteur);
  myStepper.setAcceleration(accelMoteur);
  Serial.begin(115200);
  ecranDepart();
}

void loop() {
  currentMillis = millis();
  evaluationDistance();
  StateManager();
  myStepper.run();
}

void StateManager() {
  etatCourant = FERMEE;

  if (distance < distanceMin) {
    etatCourant = OUVERTE;
  } else if (distance > distanceMax) {
    etatCourant = FERMEE;
  } 

  switch (etatCourant) {
    case FERMEE:
      porteFermee();
      ecranPorte();
      portSerie();

      break;
    case OUVERTE:
      porteOuverte();
      ecranPorte();
      portSerie();
      break;
  }

}

void ecranDepart() {
  lcd.begin();
  lcd.print(numeroEtd);
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.print("Labo 4A");
  delay(attente);
}

int evaluationDistance() {
  if (currentMillis - previousMillis >= interval2) {
    previousMillis=currentMillis;
    distance = hc.dist();
  }
}

void ecranPorte(int distance, int degreMoteur) {
  lcd.setCursor(7, 0);
  lcd.print("         ");
  lcd.setCursor(7, 1);
  lcd.print("         ");

  lcd.setCursor(0, 0);
  lcd.print("Dist  : ");
  lcd.print(distance);
  lcd.print(" cm");

  lcd.setCursor(0, 1);
  lcd.print("Porte : ");

  if (distanceMoteur == distanceMoteurMax) {
    lcd.print("FERMEE");
  } else if (distanceMoteur == distanceMoteurMin) {
    lcd.print("OUVERTE");
  } else if (distanceMoteur < distanceMoteurMax || degreMoteur > distanceMoteurMin) {
    lcd.print(degreMoteur);
    lcd.print(" deg");
  }
}

int porteOuverte() {
  myStepper.enableOutputs();

  distanceMoteur = myStepper.currentPosition();
  distanceMoteur = map(distanceMoteur, distanceMoteurMax, distanceMoteurMin, distanceMoteurMin, distanceMoteurMax);

  myStepper.moveTo(distanceMoteurMax);

  

  if (myStepper.distanceToGo() == 0) {

    myStepper.disableOutputs();
  }
  degreMoteur = distanceMoteur;
  degreMoteur = map(distanceMoteur, distanceMoteurMin, distanceMoteurMax, degreMoteurMin, degreMoteurMax);
}

int porteFermee() {
  myStepper.enableOutputs();

  distanceMoteur = myStepper.currentPosition();
  distanceMoteur = map(distanceMoteur, distanceMoteurMin, distanceMoteurMax, distanceMoteurMax, distanceMoteurMin);
  myStepper.moveTo(distanceMoteurMin);


  if (myStepper.distanceToGo() == 0) {

    myStepper.disableOutputs();
  }
  degreMoteur = map(distanceMoteur, distanceMoteurMin, distanceMoteurMax, degreMoteurMin, degreMoteurMax);
}



void portSerie() {
  if (currentMillis - previousMillis >= interval) {
    previousMillis=currentMillis;
    Serial.print("etd:");
    Serial.print(numeroEtd);
    Serial.print(",dist:");
    Serial.print(distance);
    Serial.print(",deg:");
    Serial.println(degreMoteur);
  }
}