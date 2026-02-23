const int ledPins[4] ={10,9,8,7};
const int bouttonPin=2;
const int potPin=A1;
unsigned long numeroEtd=1970812;
int valeurPotMax=1023;
int valeurEchelle=20;
int etatBoutton;
int dernierEtatBoutton=LOW;
void setup() {
  Serial.begin(9600);

  pinMode(bouttonPin, INPUT_PULLUP);

  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void gestionLed(int pourcent) {
  if (pourcent < 0) pourcent = 0;
  if (pourcent > 100) pourcent = 100;
  int zone = pourcent / 25;   // 0 à 4
  if (zone == 4) zone = 3;   
  for (int i = 0; i < 4; i++) {
    if (i == zone)
      digitalWrite(ledPins[i], HIGH);
    else
      digitalWrite(ledPins[i], LOW);
  }
}

void afficherBarre(int pourcent) {
  int remplir = map(pourcent, 0, 100, 0, 20);
  Serial.print(pourcent);
  Serial.print("% [");
  for (int i = 0; i < remplir; i++) Serial.print("-");
  for (int i = remplir; i < 20; i++) Serial.print(".");
  Serial.println("]");
}

void loop() {
  int potValeur=analogRead(potPin);
  int etatBoutton = digitalRead(bouttonPin);
  unsigned long previousMillis=0;
  int interval=map(potValeur,0,valeurPotMax,0,valeurEchelle);
  int pourcent=map(interval,0,valeurEchelle,0,100);
  gestionLed(pourcent);
  if (dernierEtatBoutton == HIGH && etatBoutton == LOW) {
    afficherBarre(pourcent);
  }
  dernierEtatBoutton = etatBoutton;
}









