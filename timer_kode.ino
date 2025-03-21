#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int potPin = A0; // Potentiometer på A0
const int buttonPin = 2; // Startknap på digital pin 2
const int buzzer = 12; // Buzzer på pin 12

int tid = 0;
bool started = false; // Bool til at tjekke om timeren er startet

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Knappen er HIGH når ikke trykket
  pinMode(buzzer, OUTPUT); // Buzzer som output
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
  Serial.println("--------- Klar ---------");
}

void loop() {
  if (!started) {
    // Læs potentiometeret og omregn til sekunder (0-300 sek)
    int potValue = analogRead(potPin);
    tid = map(potValue, 0, 1023, 0, 3600); // 0-5 minutter (0-300 sek)
    
    // Vis valgt tid
    lcd.clear();
    lcd.setCursor(0, 1);
    if (tid >= 60) {
      lcd.print(tid / 60);
      lcd.print(",");
      lcd.print(tid % 60);
      lcd.print(" m");
    } else {
      lcd.print(tid);
      lcd.print(" s");
    }
    
    Serial.print("Valgt tid: ");
    Serial.println(tid);
    
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
      lcd.clear();
      
      if (tid >= 60) {
        lcd.print(tid / 60);
        lcd.print(",");
        lcd.print(tid % 60);
        lcd.print(" m");
      } else {
        lcd.print(tid);
        lcd.print(" s");
      }

      Serial.print("Tid: ");
      Serial.println(tid);
      
      delay(1000);
      tid--;

      if (tid < 0) {
        lcd.clear();
        lcd.print("Done");
        Serial.println("Done");

        // Buzzer alarm (3 bip)
        for (int i = 0; i < 3; i++) {
          digitalWrite(buzzer, HIGH);
          delay(300);
          digitalWrite(buzzer, LOW);
          delay(200);
        }

        // Vent 2,5 sekunder
        delay(2500);

        // Nulstil til startskærm
        started = false;
      }
    }
  }
}
