// Adafruit Feather nRF52840 Express
const int LEDs[3][2] = {
  {5, 10},
  {6, 11},
  {9, 12}
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
  setupLEDs();

}

void loop() {
  for (int i = 0; i < 45; i++){
    display(i);
    delay(500);
  }
}

void setupLEDs(){
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 2; j++){
      pinMode(LEDs[i][j], OUTPUT);
      digitalWrite(LEDs[i][j], LOW);
    }
  }
}

void display(int c){
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 2; j++){
      digitalWrite(LEDs[i][j], BRAILLE[c][i][j]);
    }
  }
}

