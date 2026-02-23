
enum EtatAppli {ETAT_ALLUMEETEINT , ETAT_VAR , ETAT_CLIGNE} ;
EtatAppli EtatActuel = ETAT_ALLUMEETEINT;
unsigned long numero_etd = 1970812;
unsigned int ledPin=9;
unsigned int brightness = 0;
unsigned int fadeAmount = 1;
unsigned int maxBright= 255;
unsigned int tempsVar= 2048;
unsigned int delayInit=300;
unsigned int delayAllume= 2000;
unsigned int delayFerme = 1000;
unsigned int delayCligne=350;
unsigned int baud = 9600;
void setup() {
  Serial.begin(baud);
  pinMode(ledPin, OUTPUT); 
}


void loop() {
  switch(EtatActuel) {
    case ETAT_ALLUMEETEINT :
      allumeEteint();
      break;
    case ETAT_VAR :
      variation();
      break;
    case ETAT_CLIGNE :
     clignotte();
     break;
  }

}
void allumeEteint(){
  digitalWrite(ledPin, LOW);
  delay(delayInit);

  digitalWrite(ledPin, HIGH);
  delay(delayAllume);

  digitalWrite(ledPin, LOW);
  Serial.println("Etat : Allume - 1970812");
  delay(delayFerme);

  EtatActuel=ETAT_VAR;
}

void variation(){

  brightness = 0;
  while (brightness <= maxBright) {
    analogWrite(ledPin, brightness);
    brightness += fadeAmount;
    delay(tempsVar / maxBright);
  }
  Serial.println("Etat : Variation - 1970812");
  digitalWrite(ledPin,LOW);
  // Pour montrer visuellement une difference entre variation et clignottement.
  unsigned int delayClarete=100;
  delay(delayClarete);
  EtatActuel= ETAT_CLIGNE;
}
void clignotte(){
  // calcul des clignottements = 1 (=ceiling.math(1/2))
  digitalWrite(ledPin, HIGH);
  delay(delayCligne);
  digitalWrite(ledPin, LOW);
  delay(delayCligne);
 Serial.println("Etat : Clignotement - 1970812");
  EtatActuel=ETAT_ALLUMEETEINT;
}