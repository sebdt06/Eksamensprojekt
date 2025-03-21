#include <LiquidCrystal_I2C.h>
#include  <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27,  16, 2);

const int potmetertid = A0; // Potentiometer på A0
const int buttonPin = 2; // Startknap på digital pin 2
const int buzzer = 13; // Buzzer på pin 12
// int speed = 0;
int tid = 0;
// int potmeterspeed = A1;
bool started = false;


void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Knappen er HIGH når ikke trykket
  pinMode(buzzer, OUTPUT); // Buzzer som output
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
  sensors.begin();
}

void loop() {
  if (!started) {
    // Læs potentiometeret og omregn til sekunder (0-300 sek)
    int potValuetime = analogRead(potmetertid);
    tid = map(potValuetime, 0, 1023, 0, 3600); // 0-60 minutter (0-3600 sek)
    
    // int potValuespeed = analogRead(potmeterspeed);
    // speed = map(potValuespeed, 0, 1023, 0, 200); 
    // Vis valgt tid
    sensors.requestTemperatures();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Fart:");
    lcd.setCursor(11,0);
    lcd.print("Temp:");
    lcd.setCursor(0,0);
    lcd.print("Tid:");
    // lcd.setCursor(6, 1);
    // lcd.print(speed);
    lcd.setCursor(11,1);
    lcd.print(sensors.getTempCByIndex(0));
    lcd.setCursor(0, 1);
    if (tid >= 60) {
      lcd.print(tid / 60);
      lcd.print(".");
      lcd.print(tid % 60);
      lcd.print("m");
    } else {
      lcd.print(tid);
      lcd.print("s");
    }

    delay(500); // Opdateringshastighed

    // Hvis knappen trykkes, start nedtælling
    if (digitalRead(buttonPin) == LOW) {
      started = true;
      delay(500); // Debounce knap
    }
  } 
  else {
    // Nedtælling starter
    while (tid >= 0) {
    sensors.requestTemperatures();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Fart:");
    lcd.setCursor(11,0);
    lcd.print("Temp:");
    lcd.setCursor(0,0);
    lcd.print("Tid:");
    // lcd.setCursor(6, 1);
    // lcd.print(speed);
    lcd.setCursor(11,1);
    lcd.print(sensors.getTempCByIndex(0));
    lcd.setCursor(0, 1);
      
      if (tid >= 60) {
        lcd.print(tid / 60);
        lcd.print(".");
        lcd.print(tid % 60);
        lcd.print("m");
      } else {
        lcd.print(tid);
        lcd.print("s");
      }
      
      delay(1000);
      tid--;

      if (tid < 0) {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Done");


          digitalWrite(buzzer, HIGH);
          delay(300);
          digitalWrite(buzzer, LOW);
          delay(200);
          digitalWrite(buzzer, HIGH);
          delay(300);
          digitalWrite(buzzer, LOW);
          delay(200);
          digitalWrite(buzzer, HIGH);
          delay(300);
          digitalWrite(buzzer, LOW);
        

        // Vent 2,5 sekunder
        delay(2500);

        // Nulstil til startskærm
        started = false;
      }
    }
    // Skriv kode herunder for at det er i loopet:
    sensors.requestTemperatures();
  }
}
