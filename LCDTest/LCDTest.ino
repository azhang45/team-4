#include <LiquidCrystal.h>

#define BUTTON_PIN 4

int lastButtonState;
const int rs = 2, en = 3, d4 = 6, d5 = 7, d6 = 8, d7 = 9;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("pls work thx");
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // enable the internal pull-up resistor
  lastButtonState = digitalRead(BUTTON_PIN);

}

void loop() {
  // put your main code here, to run repeatedly:

  int buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState != buttonState) { // state changed
    delay(50); // debounce time

    if (buttonState == LOW) {
      Serial.println("The button pressed event");
      lcd.scrollDisplayLeft();
    }else
      Serial.println("The button released event");
    
    lastButtonState = buttonState;
  }

}