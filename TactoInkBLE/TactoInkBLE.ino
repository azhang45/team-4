// Adafruit Feather nRF52840 Express

//Bluetooth libs
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

// OLED libs
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// BLE Service
BLEDfu  bledfu;  // OTA DFU service
BLEDis  bledis;  // device information
BLEUart bleuart; // uart over ble
BLEBas  blebas;  // battery

// OLED setup
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3D
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// BUTTON setup
#define BUTTON_PIN 5
int buttonState = HIGH, prevButtonState = HIGH;

// DATA setup
String initial = "Enter: ";
String text = initial;
bool firstData = true;
bool newDataReceived = false; 

const int PIN[3][2] = {
  {12, 13},
  {11, 6},
  {10, 9}
};

const char ASCII[45] = {
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
  '.', ',', '!', '?', ':', ';', ' ', '-', '/'
};

const bool BRAILLE[45][3][2] = {
  {{HIGH, LOW}, {LOW, LOW}, {LOW, LOW}},   // a
  {{HIGH, LOW}, {HIGH, LOW}, {LOW, LOW}},  // b
  {{HIGH, HIGH}, {LOW, LOW}, {LOW, LOW}},  // c
  {{HIGH, HIGH}, {LOW, HIGH}, {LOW, LOW}}, // d
  {{HIGH, LOW}, {LOW, HIGH}, {LOW, LOW}},  // e
  {{HIGH, HIGH}, {HIGH, LOW}, {LOW, LOW}}, // f
  {{HIGH, HIGH}, {HIGH, HIGH}, {LOW, LOW}},// g
  {{HIGH, LOW}, {HIGH, HIGH}, {LOW, LOW}}, // h
  {{LOW, HIGH}, {HIGH, LOW}, {LOW, LOW}},  // i
  {{LOW, HIGH}, {HIGH, HIGH}, {LOW, LOW}}, // j
  {{HIGH, LOW}, {LOW, LOW}, {HIGH, LOW}},  // k
  {{HIGH, LOW}, {HIGH, LOW}, {HIGH, LOW}}, // l
  {{HIGH, HIGH}, {LOW, LOW}, {HIGH, LOW}}, // m
  {{HIGH, HIGH}, {LOW, HIGH}, {HIGH, LOW}},// n
  {{HIGH, LOW}, {LOW, HIGH}, {HIGH, LOW}}, // o
  {{HIGH, HIGH}, {HIGH, LOW}, {HIGH, LOW}},// p
  {{HIGH, HIGH}, {HIGH, HIGH}, {HIGH, LOW}},// q
  {{HIGH, LOW}, {HIGH, HIGH}, {HIGH, LOW}}, // r
  {{LOW, HIGH}, {HIGH, LOW}, {HIGH, LOW}},  // s
  {{LOW, HIGH}, {HIGH, HIGH}, {HIGH, LOW}}, // t
  {{HIGH, LOW}, {LOW, LOW}, {HIGH, HIGH}},  // u
  {{HIGH, LOW}, {HIGH, LOW}, {HIGH, HIGH}}, // v
  {{LOW, HIGH}, {HIGH, HIGH}, {LOW, HIGH}}, // w
  {{HIGH, HIGH}, {LOW, LOW}, {HIGH, HIGH}}, // x
  {{HIGH, HIGH}, {LOW, HIGH}, {HIGH, HIGH}},// y
  {{HIGH, LOW}, {LOW, HIGH}, {HIGH, HIGH}}, // z
  {{LOW, HIGH}, {HIGH, HIGH}, {LOW, HIGH}}, // 1
  {{HIGH, LOW}, {LOW, HIGH}, {HIGH, HIGH}}, // 2
  {{HIGH, HIGH}, {LOW, LOW}, {HIGH, HIGH}}, // 3
  {{HIGH, HIGH}, {LOW, HIGH}, {HIGH, HIGH}},// 4
  {{HIGH, LOW}, {HIGH, HIGH}, {HIGH, HIGH}},// 5
  {{HIGH, HIGH}, {HIGH, LOW}, {HIGH, HIGH}},// 6
  {{HIGH, HIGH}, {HIGH, HIGH}, {HIGH, HIGH}},// 7
  {{HIGH, LOW}, {HIGH, HIGH}, {LOW, HIGH}}, // 8
  {{LOW, HIGH}, {HIGH, HIGH}, {HIGH, LOW}}, // 9
  {{LOW, HIGH}, {HIGH, HIGH}, {HIGH, HIGH}},// 0
  {{LOW, LOW}, {HIGH, HIGH}, {LOW, LOW}},   // .
  {{LOW, LOW}, {HIGH, LOW}, {LOW, LOW}},    // ,
  {{LOW, HIGH}, {LOW, LOW}, {LOW, HIGH}},   // !
  {{LOW, HIGH}, {HIGH, LOW}, {HIGH, HIGH}}, // ?
  {{LOW, LOW}, {HIGH, HIGH}, {HIGH, HIGH}}, // :
  {{LOW, LOW}, {LOW, HIGH}, {LOW, HIGH}},   // ;
  {{LOW, LOW}, {LOW, LOW}, {LOW, LOW}},     // (space)
  {{HIGH, LOW}, {HIGH, HIGH}, {HIGH, LOW}}, // -
  {{LOW, HIGH}, {HIGH, LOW}, {HIGH, LOW}}   // /
};

void setup() {
  Serial.begin(9600);
  
  // pins
  setupPins();
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // display
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
  
  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behavior, but provided
  // here in case you want to control this LED manually via PIN 19
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth 
  // more SRAM required by SoftDevice
  // Note: All config***() function must be called before begin()
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  //Bluefruit.setName(getMcuUniqueID()); // useful testing with multiple central connections
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // To be consistent OTA DFU should be added first if it exists
  bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);

  // Set up and start advertising
  startAdv();

  Serial.println("Please use Adafruit's Bluefruit LE app to connect in UART mode");
  Serial.println("Once connected, enter character(s) that you wish to send");
}

void loop() {
  if (bleuart.available()) {
    if (firstData){
      initial = text = "";
      firstData = false;
    }
    char c = bleuart.read();
    if (c == '\n') {
      newDataReceived = true;
    } else {
      initial += c;
    }
  }
  if (newDataReceived) {
    Serial.print("Received: ");
    Serial.println(initial);
    text = " " + initial;
    newDataReceived = false;
  }
  display.setCursor(0, 0);
  show(text[0]);
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW && prevButtonState == HIGH) {
    shiftTextLeft();
    delay(125);
  }
  prevButtonState = buttonState;
  if (text == ""){
    text = " " + initial;
  }
}

void setupPins(){
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 2; j++){
      pinMode(PIN[i][j], OUTPUT);
      digitalWrite(PIN[i][j], LOW);
    }
  }
}

int index(char c){
  for (int i = 0; i < 45; i++){
    if (ASCII[i] == c){
      // Serial.println(i);
      return i;
    }
  }
  return -1;
}

void show(char c){
  int ind = index(tolower(c));
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 2; j++){
      digitalWrite(PIN[i][j], BRAILLE[ind][i][j]);
    }
  }
}

// GFX FUNCTIONS
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

// BLE FUNCTIONS
void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println();
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}