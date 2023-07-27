#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String initial = "I love Rohan Panday!";
String text = initial;

const int buttonPin = 5;
int buttonState = HIGH, prevButtonState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.dim(false);
  display.display();
  delay(2000);

  // opening sequence
  logo();
  delay(4500);
  display.clearDisplay();

  // start streaming text
  display.setTextWrap(false);
  display.setCursor(0, verticalCenter(text));
  display.print(text);
  display.display();
}

void loop() {
  display.setCursor(0, 0);
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && prevButtonState == HIGH) {
    shiftTextLeft();
    Serial.println(text);
    delay(125);
  }
  Serial.println(buttonState);
  prevButtonState = buttonState;
  if (text == ""){
    text = " " + initial;
  }
}

void logo(){
  int x, y;
  int16_t textWidth, textHeight;
  uint16_t boundaryWidth, boundaryHeight;
  // line 1
  display.getTextBounds("tacto", 0, 0, &textWidth, &textHeight, &boundaryWidth, &boundaryHeight);
  x = (SCREEN_WIDTH - boundaryWidth) / 2 - textWidth;
  y = (SCREEN_HEIGHT - boundaryHeight) / 2 - textHeight;
  display.clearDisplay();
  display.setCursor(x, 0);
  display.print("tacto");
  // line 2
  display.fillCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 3, SSD1306_WHITE);
  // line 3
  display.getTextBounds("ink", 0, 0, &textWidth, &textHeight, &boundaryWidth, &boundaryHeight);
  x = (SCREEN_WIDTH - boundaryWidth) / 2 - textWidth;
   y = (SCREEN_HEIGHT - boundaryHeight) / 2 - textHeight;
  display.setCursor(x, 2 * y);
  display.print("ink");
  display.display();
}

void shiftTextLeft() {
  text = text.substring(1);
  display.clearDisplay();
  display.setCursor(0, verticalCenter(text));
  display.print(text);
  display.display();
}

int16_t verticalCenter(String s){
  int16_t tW, tH;
  uint16_t bW, bH;
  display.getTextBounds(s, 0, 0, &tW, &tH, &bW, &bH);
  return ((SCREEN_HEIGHT - bH) / 2) - tH;
}