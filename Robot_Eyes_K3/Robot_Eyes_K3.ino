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

/*
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();
Adafruit_8x16matrix matrix1 = Adafruit_8x16matrix();
Adafruit_8x8matrix matrix2 = Adafruit_8x8matrix();
*/

Adafruit_8x16matrix matrix[4] = { // Array of Adafruit_8x8matrix objects
  Adafruit_8x16matrix(), Adafruit_8x16matrix(),
  Adafruit_8x16matrix(), Adafruit_8x16matrix()};
  
Adafruit_8x8matrix matrix4 = Adafruit_8x8matrix();

static const uint8_t matrixAddr[] = { 0x70, 0x71, 0x72, 0x73, 0x74};



#define MATRIX_EYES         0

static const uint8_t PROGMEM // Bitmaps are stored in program memory
  blinkImg[][8] = {    // Eye animation frames
  { B00111100,         // Fully open eye
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,
    B00000000,
    B00111100,
    B11111111,
    B11111111,
    B11111111,
    B00111100,
    B00000000 },
  { B00000000,
    B00000000,
    B00000000,
    B00111100,
    B11111111,
    B01111110,
    B00011000,
    B00000000 },
  { B00000000,         // Fully closed eye
    B00000000,
    B00000000,
    B00000000,
    B10000001,
    B01111110,
    B00000000,
    B00000000 } };
    
uint8_t
  blinkIndex[] = { 1, 2, 3, 4, 3, 2, 1 }, // Blink bitmap sequence
  blinkCountdown = 100, // Countdown to next blink (in frames)
  gazeCountdown  =  75, // Countdown to next eye movement
  gazeFrames     =  50, // Duration of eye movement (smaller = faster)
  mouthPos       =   0, // Current image number for mouth
  mouthCountdown =  10; // Countdown to next mouth change
int8_t
  eyeX = 3, eyeY = 3,   // Current eye position
  newX = 3, newY = 3,   // Next eye position
  dX   = 0, dY   = 0;   // Distance from prior to new position
    
    
void setup() {
  Serial.begin(9600);
  Serial.println("16x8 LED Matrix Test");
  
  /*
  matrix.begin(0x70);  // pass in the address
  matrix1.begin(0x73);  // pass in the address
  matrix1.setRotation(3);
  matrix2.begin(0x74);
  matrix2.setRotation(4);
  */
  
  for(uint8_t i=0; i<4; i++) {
    matrix[i].begin(matrixAddr[i]);
    
    if(i==3) matrix.setRotation(3);
  }
  
  matrix4.begin(matrixAddr[4]);
  matrix4.setRotation(4);
}


/*
static const uint32_t
ex_bmp[] =
    {  B11000000000000000000000000000000};
*/
void loop() {
  
  
  // Draw eyeball in current state of blinkyness (no pupil).  Note that
  // only one eye needs to be drawn.  Because the two eye matrices share
  // the same address, the same data will be received by both.
  //matrix[MATRIX_EYES].clear();
  
  for(uint8_t i=0; i<4; i++){matrix[i].clear();}   
  matrix4.clear();
  
  // When counting down to the next blink, show the eye in the fully-
  // open state.  On the last few counts (during the blink), look up
  // the corresponding bitmap index.
  
  for(uint8_t i=0; i<4; i++){ matrix[MATRIX_EYES].drawBitmap(0, 0,
    blinkImg[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
      blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
      0                                       // No, show bitmap 0
    ][], 8, 8, LED_ON);}   
  //matrix4.clear();
  
  /*
 
  matrix[MATRIX_EYES].drawBitmap(0, 0,
    blinkImg[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
      blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
      0                                       // No, show bitmap 0
    ], 8, 8, LED_ON);*/
    
    
  // Decrement blink counter.  At end, set random time for next blink.
  if(--blinkCountdown == 0) blinkCountdown = random(5, 180);

  // Add a pupil (2x2 black square) atop the blinky eyeball bitmap.
  // Periodically, the pupil moves to a new position...
  if(--gazeCountdown <= gazeFrames) {
    // Eyes are in motion - draw pupil at interim position
    matrix[MATRIX_EYES].fillRect(
      newX - (dX * gazeCountdown / gazeFrames),
      newY - (dY * gazeCountdown / gazeFrames),
      2, 2, LED_OFF);
    if(gazeCountdown == 0) {    // Last frame?
      eyeX = newX; eyeY = newY; // Yes.  What's new is old, then...
      do { // Pick random positions until one is within the eye circle
        newX = random(7); newY = random(7);
        dX   = newX - 3;  dY   = newY - 3;
      } while((dX * dX + dY * dY) >= 10);      // Thank you Pythagoras
      dX            = newX - eyeX;             // Horizontal distance to move
      dY            = newY - eyeY;             // Vertical distance to move
      gazeFrames    = random(3, 15);           // Duration of eye movement
      gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
    }
  } else {
    // Not in motion yet -- draw pupil at current static position
    matrix[MATRIX_EYES].fillRect(eyeX, eyeY, 2, 2, LED_OFF);
  }
  
  
   for(uint8_t i=0; i<4; i++){
     matrix[i].writeDisplay();
     
   }
   
   matrix4.writeDisplay();

  delay(20); // ~50 FPS
  /*
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
  }*/
}
