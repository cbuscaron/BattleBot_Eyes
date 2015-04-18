/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit 16x8 LED Matrix backpacks 
  ----> http://www.adafruit.com/products/2035
  ----> http://www.adafruit.com/products/2036
  ----> http://www.adafruit.com/products/2037
  ----> http://www.adafruit.com/products/2038
  ----> http://www.adafruit.com/products/2039
  ----> http://www.adafruit.com/products/2040
  ----> http://www.adafruit.com/products/2041
  ----> http://www.adafruit.com/products/2042
  ----> http://www.adafruit.com/products/2043
  ----> http://www.adafruit.com/products/2044
  ----> http://www.adafruit.com/products/2052

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_8x16matrix matrix = Adafruit_8x16matrix();
Adafruit_8x16matrix matrix1 = Adafruit_8x16matrix();
Adafruit_8x8matrix matrix2 = Adafruit_8x8matrix();

void setup() {
  Serial.begin(9600);
  Serial.println("16x8 LED Matrix Test");
  
  matrix.begin(0x70);  // pass in the address
  matrix1.begin(0x73);  // pass in the address
  matrix1.setRotation(3);
  matrix2.begin(0x74);
  matrix2.setRotation(4);
}

static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 },
  x1_bmp[] =
  { B11000000,
    B11100000,
    B01110000,
    B00111000,
    B00011100,
    B00001110,
    B00000111,
    B00000011 },
  x2_bmp[] =
  { B00000001,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000001 },
  x3_bmp[] =
  { B00000011,
    B00000111,
    B00001110,
    B00011100,
    B00111000,
    B01110000,
    B11100000,
    B11000000 },
  x4_bmp[] =
  { B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B10000001 },
  x5_bmp[] =
  { B11000011,
    B11100111,
    B01111110,
    B00111100,
    B00111100,
    B01111110,
    B11100111,
    B11000011 },
  x6_bmp[] =
  { B10000001,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000 },
  x7_bmp[] =
  { B00000011,
    B00000111,
    B00001110,
    B00011100,
    B00111000,
    B01110000,
    B11100000,
    B11000000 },
  x8_bmp[] =
  { B10000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B10000000 },
  x9_bmp[] =
  { B11000000,
    B11100000,
    B01110000,
    B00111000,
    B00011100,
    B00001110,
    B00000111,
    B00000011 },
  x1i_bmp[] =
  { B00111111,
    B00011111,
    B10001111,
    B11000111,
    B11100011,
    B11110001,
    B11111000,
    B11111100 },
  x2i_bmp[] =
  { B11111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111110 },
  x3i_bmp[] =
  { B11111100,
    B11111000,
    B11110001,
    B11100011,
    B11000111,
    B10001111,
    B00011111,
    B00111111 },
  x4i_bmp[] =
  { B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110 },
  x5i_bmp[] =
  { B00111100,
    B00011000,
    B10000001,
    B11000011,
    B11000011,
    B10000001,
    B00011000,
    B00111100 },
  x6i_bmp[] =
  { B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111 },
  x7i_bmp[] =
  { B11111100,
    B11111000,
    B11110001,
    B11100011,
    B11000111,
    B10001111,
    B00011111,
    B0011111 },
  x8i_bmp[] =
  { B01111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111111 },
  x9i_bmp[] =
  { B00111111,
    B00011111,
    B10001111,
    B11000111,
    B11100011,
    B11110001,
    B11111000,
    B11111100 };
/*
static const uint32_t
ex_bmp[] =
    {  B11000000000000000000000000000000};
*/
void loop() {
  
  matrix.begin(0x70);
  matrix.clear();
  matrix.drawBitmap(0, 0, x1_bmp, 8, 8, LED_ON);
  matrix.drawBitmap(0, 8, x2_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  matrix.begin(0x71);
  matrix.clear();
  matrix.drawBitmap(0, 0, x4_bmp, 8, 8, LED_ON);
  matrix.drawBitmap(0, 8, x5_bmp, 8,16, LED_ON);
  matrix.writeDisplay();
  matrix.begin(0x72);
  matrix.clear();
  matrix.drawBitmap(0, 0, x7_bmp, 8, 8, LED_ON);
  matrix.drawBitmap(0, 8, x8_bmp, 8,16, LED_ON);
  matrix.writeDisplay();
  matrix1.begin(0x73);
  matrix1.clear();
  matrix1.drawBitmap(0, 0, x6_bmp, 8, 8, LED_ON);
  matrix1.drawBitmap(8, 0, x9_bmp, 8,16, LED_ON);
  matrix1.writeDisplay();
  matrix2.begin(0x74);
  matrix2.clear();
  matrix2.drawBitmap(0, 0, x3_bmp, 8, 8, LED_ON);
  matrix2.writeDisplay();
  delay(5000);
  
  matrix.begin(0x70);
  matrix.clear();
  matrix.drawBitmap(0, 0, x1i_bmp, 8, 8, LED_ON);
  matrix.drawBitmap(0, 8, x2i_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  matrix.begin(0x71);
  matrix.clear();
  matrix.drawBitmap(0, 0, x4i_bmp, 8, 8, LED_ON);
  matrix.drawBitmap(0, 8, x5i_bmp, 8,16, LED_ON);
  matrix.writeDisplay();
  matrix.begin(0x72);
  matrix.clear();
  matrix.drawBitmap(0, 0, x7i_bmp, 8, 8, LED_ON);
  matrix.drawBitmap(0, 8, x8i_bmp, 8,16, LED_ON);
  matrix.writeDisplay();
  matrix1.begin(0x73);
  matrix1.clear();
  matrix1.drawBitmap(0, 0, x6i_bmp, 8, 8, LED_ON);
  matrix1.drawBitmap(8, 0, x9i_bmp, 8,16, LED_ON);
  matrix1.writeDisplay();
  matrix2.begin(0x74);
  matrix2.clear();
  matrix2.drawBitmap(0, 0, x3i_bmp, 8, 8, LED_ON);
  matrix2.writeDisplay();
  delay(5000);
  /*
  matrix.begin(0x70);
  matrix.clear();
  matrix.drawBitmap(0, 8, neutral_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  matrix.begin(0x71);
  matrix.clear();
  matrix.drawBitmap(0, 8, neutral_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  matrix.begin(0x72);
  matrix.clear();
  matrix.drawBitmap(0, 8, neutral_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  matrix.begin(0x73);
  matrix.clear();
  matrix.drawBitmap(0, 8, neutral_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);

  matrix.begin(0x70);
  matrix.clear();
  matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  matrix.begin(0x71);
  matrix.clear();
  matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  matrix.begin(0x72);
  matrix.clear();
  matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  matrix.begin(0x73);
  matrix.clear();
  matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);

  matrix.begin(0x70);
  matrix.clear();      // clear display
  matrix.drawPixel(0, 0, LED_ON);  
  matrix.writeDisplay();  // write the changes we just made to the display
  matrix.begin(0x71);
  matrix.clear();      // clear display
  matrix.drawPixel(0, 0, LED_ON);  
  matrix.writeDisplay();  // write the changes we just made to the display
  matrix.begin(0x72);
  matrix.clear();      // clear display
  matrix.drawPixel(0, 0, LED_ON);  
  matrix.writeDisplay();  // write the changes we just made to the display
  matrix.begin(0x73);
  matrix.clear();      // clear display
  matrix.drawPixel(0, 0, LED_ON);  
  matrix.writeDisplay();  // write the changes we just made to the display
  delay(500);

  matrix.begin(0x70);
  matrix.clear();
  matrix.drawLine(0,0, 7,15, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  matrix.begin(0x71);
  matrix.clear();
  matrix.drawLine(0,0, 7,15, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  matrix.begin(0x72);
  matrix.clear();
  matrix.drawLine(0,0, 7,15, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  matrix.begin(0x73);
  matrix.clear();
  matrix.drawLine(0,0, 7,15, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delay(500);

  matrix.begin(0x70);
  matrix.clear();
  matrix.drawRect(0,0, 8,16, LED_ON);
  matrix.fillRect(2,2, 4,12, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  matrix.begin(0x71);
  matrix.clear();
  matrix.drawRect(0,0, 8,16, LED_ON);
  matrix.fillRect(2,2, 4,12, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  matrix.begin(0x72);
  matrix.clear();
  matrix.drawRect(0,0, 8,16, LED_ON);
  matrix.fillRect(2,2, 4,12, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  matrix.begin(0x73);
  matrix.clear();
  matrix.drawRect(0,0, 8,16, LED_ON);
  matrix.fillRect(2,2, 4,12, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delay(500);

  matrix.begin(0x70);
  matrix.clear();
  matrix.drawCircle(3,8, 3, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  matrix.begin(0x71);
  matrix.clear();
  matrix.drawCircle(3,8, 3, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  matrix.begin(0x72);
  matrix.clear();
  matrix.drawCircle(3,8, 3, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delay(500);
  */
  
  matrix.setTextSize(3);
  matrix1.setTextSize(3);
  matrix2.setTextSize(3);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  for (int8_t x=0; x>=-64; x--) {  
    matrix.begin(0x70);
    matrix.clear();
    matrix.setCursor(x+24,0);
    matrix.print("Hello");
    matrix.writeDisplay();
    matrix.begin(0x71);
    matrix.clear();
    matrix.setCursor(x+16,0);
    matrix.print("Hello");
    matrix.writeDisplay();
    matrix.begin(0x72);
    matrix.clear();
    matrix.setCursor(x+8,0);
    matrix.print("Hello");
    matrix.writeDisplay();
    matrix1.begin(0x73);
    matrix1.setTextSize(3);
    matrix1.clear();
    matrix1.setCursor(x+16,-16);
    matrix1.print("Hello");
    matrix1.writeDisplay();
    matrix2.begin(0x74);
    matrix2.clear();
    matrix2.setCursor(x+24,-16);
    matrix2.print("Hello");
    matrix2.writeDisplay();
    delay(100);
  }
}
