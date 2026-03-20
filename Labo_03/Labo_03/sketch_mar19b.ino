#include <LCD_I2C.h>

#define BTN_PIN 2
#define LED_PIN 8
#define THERM_PIN A0
#define JOY_X A1
#define JOY_Y A2

LCD_I2C lcd(0x27, 16, 2);


int valeurX = 0;
int valeurY = 0;

int altitude = 0;
int angle = 0;

bool AC = false;

unsigned long previousMillis = 0;
unsigned long previousSerial = 0;

const long interval = 5000;  
const long intervalSerial = 100;

int limiteON = 35;
int limiteOFF = 30;

int etatBouton = 0;
int dernierEtatBouton = 0;
int page = 0;


float R1 = 10000;
float logR2, R2, T;
float c1 = 1.129148e-03, c2 = 2.34125e-04, c3 = 8.76741e-08;
int Tc;

long numeroEtd = 1970812;


void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);

  ecran_depart();
}


void loop() {
  lireJoystick();
  gestionBouton();

  if (page == 0) {
    afficherJoystick();
  } else {
    lireTemperature();
    afficherTemperature();
  }

  envoyerSerie();
}


void ecran_depart() {

  uint8_t symbole12[8]=
{
    0b01000,
    0b11000,
    0b01000,
    0b01111,
    0b11001,
    0b11101,
    0b00010,
    0b01111,
};

uint8_t dessin[8]=
{
    0b00000,
    0b01110,
    0b11111,
    0b10101,
    0b11111,
    0b01110,
    0b01110,
    0b00000,
};


  lcd.clear();
  lcd.print("Toure");
  lcd.createChar(0, symbole12);
  lcd.createChar(1, dessin);
  lcd.setCursor(0,1);
  lcd.write(1);
  lcd.setCursor(15,1);
  lcd.write(0);
  delay(3000);
  lcd.clear();
}


void lireTemperature() {

  int Vo = analogRead(THERM_PIN);

  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*pow(logR2,3)));
  Tc = T - 273.15;

  unsigned long currentMillis = millis();

  if (Tc > limiteON) {
    if (currentMillis - previousMillis >= interval) {
      AC = true;
      digitalWrite(LED_PIN, HIGH);
    }
  } 
  else if (Tc < limiteOFF) {
    if (currentMillis - previousMillis >= interval) {
      AC = false;
      digitalWrite(LED_PIN, LOW);
    }
  } 
  else {
    previousMillis = currentMillis;
  }
}


void afficherTemperature() {
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(Tc);
  lcd.print(" C   ");

  lcd.setCursor(0,1);
  lcd.print("COOLING: ");
  lcd.print(AC ? "ON " : "OFF");
}


void lireJoystick() {
  valeurX = analogRead(JOY_X);
  valeurY = analogRead(JOY_Y);

  altitude = map(valeurY, 0, 1023, 0, 200);
  angle = map(valeurX, 0, 1023, -90, 90);
}


void afficherJoystick() {

  lcd.setCursor(0,0);

  if (valeurY > 550) {
    lcd.print("UP   ");
  } else if (valeurY < 470) {
    lcd.print("DOWN ");
  } else {
    lcd.print("STABLE");
  }

  lcd.print(" ");
  lcd.print(altitude);
  lcd.print("m   ");

  lcd.setCursor(0,1);
  lcd.print("DIR :");
  if (angle >= 0) {
    lcd.print(angle);
    lcd.print("(D)");
  } else {
    lcd.print(-angle);
    lcd.print("(G)");
  }

  lcd.print((char)223); 
  lcd.print("     ");
}


void gestionBouton() {
  etatBouton = digitalRead(BTN_PIN);

  if (etatBouton != dernierEtatBouton) {
    delay(50);

    if (etatBouton == HIGH) {
      page = !page;
      lcd.clear();
    }
  }
  dernierEtatBouton = etatBouton;
}


void envoyerSerie() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousSerial >= intervalSerial) {
    previousSerial = currentMillis;

    Serial.print("etd:");
    Serial.print(numeroEtd);
    Serial.print(",x:");
    Serial.print(valeurX);
    Serial.print(",y:");
    Serial.print(valeurY);
    Serial.print(",sys:");
    Serial.println(AC ? 1 : 0);
  }
}