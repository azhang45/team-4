//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(3,5,4,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=800;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,1);
  /* and clear the display */
  lc.clearDisplay(0);
  Serial.begin(9600);
}

const char brailleCharacters[] = {
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
  '.', ',', '!', '?', ':', ';', ' ', '-', '/'
};

int numDefs = 45; // length of definition array bc apparently sizeof() method doesnt work???
  // braille definitions
  byte bDefs[45][5] = {{B00000000,B10000000,B00000000,B00000000,B00000000}, //a
  {B00000000,B10000000,B10000000,B00000000,B00000000},  //b
  {B00000000,B11000000,B00000000,B00000000,B00000000},  //c
  {B00000000,B11000000,B01000000,B00000000,B00000000},  //d
  {B00000000,B10000000,B01000000,B00000000,B00000000}, //e
  {B00000000,B11000000,B10000000,B00000000,B00000000},  //f
  {B00000000,B11000000,B11000000,B00000000,B00000000},  //g
  {B00000000,B10000000,B11000000,B00000000,B00000000},  //h
  {B00000000,B01000000,B10000000,B00000000,B00000000},  //i
  {B00000000,B01000000,B11000000,B00000000,B00000000},  //j
  {B00000000,B10000000,B00000000,B10000000,B00000000},  //k
  {B00000000,B10000000,B10000000,B10000000,B00000000},  //l
  {B00000000,B11000000,B00000000,B10000000,B00000000},  //m
  {B00000000,B11000000,B01000000,B10000000,B00000000},  //n
  {B00000000,B10000000,B01000000,B10000000,B00000000},  //o
  {B00000000,B11000000,B10000000,B10000000,B00000000},  //p
  {B00000000,B11000000,B11000000,B10000000,B00000000},  //q
  {B00000000,B10000000,B11000000,B10000000,B00000000},  //r
  {B00000000,B01000000,B10000000,B10000000,B00000000},  //s
  {B00000000,B01000000,B11000000,B10000000,B00000000},  //t
  {B00000000,B10000000,B00000000,B11000000,B00000000},  //u
  {B00000000,B10000000,B10000000,B11000000,B00000000},  //v
  {B00000000,B01000000,B11000000,B01000000,B00000000},  //w
  {B00000000,B11000000,B00000000,B11000000,B00000000},  //x
  {B00000000,B11000000,B01000000,B11000000,B00000000},  //y
  {B00000000,B10000000,B01000000,B11000000,B00000000},  //z
  {B00000000,B01010000,B01000000,B11000000,B00000000},  //1
  {B00000000,B01010000,B01010000,B11000000,B00000000},  //2
  {B00000000,B01011000,B01000000,B11000000,B00000000},  //3
  {B00000000,B01011000,B01001000,B11000000,B00000000},  //4
  {B00000000,B01010000,B01001000,B11000000,B00000000},  //5
  {B00000000,B01011000,B01010000,B11000000,B00000000},  //6
  {B00000000,B01011000,B01011000,B11000000,B00000000},  //7
  {B00000000,B01010000,B01011000,B11000000,B00000000},  //8
  {B00000000,B01001000,B01010000,B11000000,B00000000},  //9
  {B00000000,B01001000,B01011000,B11000000,B00000000},  //0
  {B00000000,B00000000,B11000000,B01000000,B00000000},  //.
  {B00000000,B00000000,B10000000,B00000000,B00000000},  //,
  {B00000000,B00000000,B11000000,B10000000,B00000000},  //!
  {B00000000,B00000000,B10000000,B11000000,B00000000},  //?
  {B00000000,B00000000,B11000000,B00000000,B00000000},  //:
  {B00000000,B00000000,B10000000,B10000000,B00000000},  //;
  {B00000000,B00000000,B00000000,B00000000,B00000000},  // space
  {B00000000,B00000000,B00000000,B11000000,B00000000},  //-
  {B00000000,B01000000,B00000000,B10000000,B00000000}  //slash
  };



/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void writeArduinoOnMatrix() {
  /* here is the data for the characters */
  byte a[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
  byte r[5]={B00111110,B00010000,B00100000,B00100000,B00010000};
  byte d[5]={B00011100,B00100010,B00100010,B00010010,B11111110};
  byte u[5]={B00111100,B00000010,B00000010,B00000100,B00111110};
  byte i[5]={B00000000,B00100010,B10111110,B00000010,B00000000};
  byte n[5]={B00111110,B00010000,B00100000,B00100000,B00011110};
  byte o[5]={B00011100,B00100010,B00100010,B00100010,B00011100};

  /* now display them one by one with a small delay */
  lc.setRow(0,0,a[0]);
  lc.setRow(0,1,a[1]);
  lc.setRow(0,2,a[2]);
  lc.setRow(0,3,a[3]);
  lc.setRow(0,4,a[4]);
  delay(delaytime);
  lc.setRow(0,0,r[0]);
  lc.setRow(0,1,r[1]);
  lc.setRow(0,2,r[2]);
  lc.setRow(0,3,r[3]);
  lc.setRow(0,4,r[4]);
  delay(delaytime);
  lc.setRow(0,0,d[0]);
  lc.setRow(0,1,d[1]);
  lc.setRow(0,2,d[2]);
  lc.setRow(0,3,d[3]);
  lc.setRow(0,4,d[4]);
  delay(delaytime);
  lc.setRow(0,0,u[0]);
  lc.setRow(0,1,u[1]);
  lc.setRow(0,2,u[2]);
  lc.setRow(0,3,u[3]);
  lc.setRow(0,4,u[4]);
  delay(delaytime);
  lc.setRow(0,0,i[0]);
  lc.setRow(0,1,i[1]);
  lc.setRow(0,2,i[2]);
  lc.setRow(0,3,i[3]);
  lc.setRow(0,4,i[4]);
  delay(delaytime);
  lc.setRow(0,0,n[0]);
  lc.setRow(0,1,n[1]);
  lc.setRow(0,2,n[2]);
  lc.setRow(0,3,n[3]);
  lc.setRow(0,4,n[4]);
  delay(delaytime);
  lc.setRow(0,0,o[0]);
  lc.setRow(0,1,o[1]);
  lc.setRow(0,2,o[2]);
  lc.setRow(0,3,o[3]);
  lc.setRow(0,4,o[4]);
  delay(delaytime);
  lc.setRow(0,0,0);
  lc.setRow(0,1,0);
  lc.setRow(0,2,0);
  lc.setRow(0,3,0);
  lc.setRow(0,4,0);
  delay(delaytime);
}

void writeAllLetters(){
  for(int i = 0; i < numDefs; i++){
    actualDisplay(bDefs[i][0], bDefs[i][1], bDefs[i][2], bDefs[i][3], bDefs[i][4]);
  } 
}

void actualDisplay(byte b0, byte b1, byte b2, byte b3, byte b4){
  lc.setRow(0,0,b0);
  lc.setRow(0,1,b1);
  lc.setRow(0,2,b2);
  lc.setRow(0,3,b3);
  lc.setRow(0,4,b4);
  delay(delaytime);

  lc.setRow(0,0,B00000000);
  lc.setRow(0,1,B00000000);
  lc.setRow(0,2,B00000000);
  lc.setRow(0,3,B00000000);
  lc.setRow(0,4,B00000000);
  delay(100);
}

int contains(char c){ // p sure there isnt a built in C function for this?
  for(int i = 0; i < numDefs; i++){
    if(brailleCharacters[i] == c){    // this gives error bc im too lazy to deal w pointers and char arrays so im ignoring this for now
      return i;
    }
  }
  return -1;
}

/*
  This function lights up a some Leds in a row.
 The pattern will be repeated on every row.
 The pattern will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void rows() {
  for(int row=0;row<8;row++) {
    delay(delaytime);
    lc.setRow(0,row,B10100000);
    delay(delaytime);
    lc.setRow(0,row,(byte)0);
    for(int i=0;i<row;i++) {
      delay(delaytime);
      lc.setRow(0,row,B10100000);
      delay(delaytime);
      lc.setRow(0,row,(byte)0);
    }
  }
}

/*
  This function lights up a some Leds in a column.
 The pattern will be repeated on every column.
 The pattern will blink along with the column-number.
 column number 4 (index==3) will blink 4 times etc.
 */
void columns() {
  for(int col=0;col<8;col++) {
    delay(delaytime);
    lc.setColumn(0,col,B10100000);
    delay(delaytime);
    lc.setColumn(0,col,(byte)0);
    for(int i=0;i<col;i++) {
      delay(delaytime);
      lc.setColumn(0,col,B10100000);
      delay(delaytime);
      lc.setColumn(0,col,(byte)0);
    }
  }
}

/* 
 This function will light up every Led on the matrix.
 The led will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void single() {
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      delay(delaytime);
      lc.setLed(0,row,col,true);
      delay(delaytime);
      for(int i=0;i<col;i++) {
        lc.setLed(0,row,col,false);
        delay(delaytime);
        lc.setLed(0,row,col,true);
        delay(delaytime);
      }
    }
  }
}

void loop() { 
  String s = "finally! 123";

  int slength = s.length() + 1;
  Serial.print(slength);
  char char_array[slength];
  s.toCharArray(char_array, slength);
  for(int c = 0; c < slength; c++){
    int index = contains(char_array[c]);
    if(index >= 0){
      actualDisplay(bDefs[index][0], bDefs[index][1], bDefs[index][2], bDefs[index][3], bDefs[index][4]);
    }
  }
  // writeAllLetters();
  // rows();
  // columns();
  // single();
}
