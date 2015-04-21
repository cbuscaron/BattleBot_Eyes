// 'roboface' example sketch for Adafruit I2C 8x8 LED backpacks:
//
//  www.adafruit.com/products/870   www.adafruit.com/products/1049
//  www.adafruit.com/products/871   www.adafruit.com/products/1050
//  www.adafruit.com/products/872   www.adafruit.com/products/1051
//  www.adafruit.com/products/959   www.adafruit.com/products/1052
//
// Requires Adafruit_LEDBackpack and Adafruit_GFX libraries.
// For a simpler introduction, see the 'matrix8x8' example.
//
// This sketch demonstrates a couple of useful techniques:
// 1) Addressing multiple matrices (using the 'A0' and 'A1' solder
//    pads on the back to select unique I2C addresses for each).
// 2) Displaying the same data on multiple matrices by sharing the
//    same I2C address.
//
// This example uses 5 matrices at 4 addresses (two share an address)
// to animate a face:
//
//     0     0
//
//      1 2 3
//
// The 'eyes' both display the same image (always looking the same
// direction -- can't go cross-eyed) and thus share the same address
// (0x70).  The three matrices forming the mouth have unique addresses
// (0x71, 0x72 and 0x73).
//
// The face animation as written is here semi-random; this neither
// generates nor responds to actual sound, it's simply a visual effect
// Consider this a stepping off point for your own project.  Maybe you
// could 'puppet' the face using joysticks, or synchronize the lips to
// audio from a Wave Shield (see wavface example).  Currently there are
// only six images for the mouth.  This is often sufficient for simple
// animation, as explained here:
// http://www.idleworm.com/how/anm/03t/talk1.shtml
//
// Adafruit invests time and resources providing this open source code,
// please support Adafruit and open-source hardware by purchasing
// products from Adafruit!
//
// Written by P. Burgess for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

// Because the two eye matrices share the same address, only four
// matrix objects are needed for the five displays:
#define MATRIX_EYES         0
#define MATRIX_MOUTH_LEFT   1
#define MATRIX_MOUTH_MIDDLE 2
#define MATRIX_MOUTH_RIGHT  3
Adafruit_8x16matrix matrix[4] = { // Array of Adafruit_8x8matrix objects
  Adafruit_8x16matrix(), Adafruit_8x16matrix(),
  Adafruit_8x16matrix(), Adafruit_8x16matrix()};

Adafruit_8x8matrix matrix4 = Adafruit_8x8matrix();

static const uint8_t matrixAddr[] = { 0x70, 0x71, 0x72, 0x73, 0x74};

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
    B00000000 } },
  mouthImg[][24] = {                 // Mouth animation frames
  { B00000011, B11111111, B11000000, // Mouth position A
    B00000011, B11111111, B11000000,
    B00000011, B11111111, B11000000,
    B00011111, B11111111, B11111000,
    B00011111, B11111111, B11111000,
    B00011111, B11111111, B11111000,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111 },
  { B00000000, B00000000, B00000000, // Mouth position B
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00011111, B11111111, B11111000,
    B00011111, B11111111, B11111000,
    B00011111, B11111111, B11111000,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111 },
  { B00000000, B00000000, B00000000, // Eye 3
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000011, B11111111, B11000000,
    B00000011, B11111111, B11000000 },
  { B00000000, B00000000, B00000000, // Mouth position D
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 },
  { B00000000, B00000000, B00000000, // Mouth position E
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 }},    
    mouthImg2[][24] = {                 // Mouth animation frames
  { B11111111, B11111111, B11111111, // Mouth position A
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111 },
  { B11111111, B11111111, B11111111, // Mouth position B
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111 },
  { B00000011, B11111111, B11000000, // Mouth position C
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111 },
  { B00000000, B00000000, B00000000, // Mouth position D
    B00000011, B11111111, B11000000,
    B00000011, B11111111, B11000000,
    B00000011, B11111111, B11000000,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B11111111, B11111111, B11111111,
    B00011111, B11111111, B11111000 },
  { B00000000, B00000000, B00000000, // Mouth position E
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B11100000, B00000000, B00000111,
    B11100000, B00000000, B00000111,
    B11100000, B00000000, B00000111,
    B00011111, B11111111, B11111000}},
    mouthImg3[][24] = {                 // Mouth animation frames
  { B11111111, B11111111, B11111111, // Mouth position A
    B11111111, B11111111, B11111111,
    B00011111, B11111111, B11111000,
    B00011111, B11111111, B11111000,
    B00011111, B11111111, B11111000,
    B00000011, B11111111, B11000000,
    B00000011, B11111111, B11000000,
    B00000011, B11111111, B11000000,},
  { B11111111, B11111111, B11111111, // Mouth position B
    B11111111, B11111111, B11111111,
    B00011111, B11111111, B11111000,
    B00011111, B11111111, B11111000,
    B00011111, B11111111, B11111000,
    B00000011, B11111111, B11000000,
    B00000011, B11111111, B11000000,
    B00000011, B11111111, B11000000 },
  { B11111111, B11111111, B11111111, // Mouth position C
    B11111111, B11111111, B11111111,
    B00000011, B11111111, B11000000,
    B00000011, B11111111, B11000000,
    B00000011, B11111111, B11000000,
    B00000000, B00111100, B00000000,
    B00000000, B00111100, B00000000,
    B00000000, B00000000, B00000000 },
  { B00011111, B11111111, B11111000, // Mouth position D
    B00011111, B11111111, B11111000,
    B00000000, B01111110, B00000000,
    B00000000, B01111110, B00000000,
    B00000000, B01111110, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 },
  { B00011111, B11111111, B11111000, // Mouth position E
    B00011111, B11111111, B11111000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 }};

uint8_t
  blinkIndex[] = { 1, 2, 3, 4, 3, 2, 1 }, // Blink bitmap sequence
  blinkCountdown = 100, // Countdown to next blink (in frames)
  gazeCountdown  =  75, // Countdown to next eye movement
  gazeFrames     =  50, // Duration of eye movement (smaller = faster)
  mouthPos       =   0, // Current image number for mouth
  mouthCountdown =  10; // Countdown to next mouth change
int8_t
  eyeX = 3, eyeY = 10,   // Current eye position
  newX = 3, newY = 10,   // Next eye position
  dX   = 0, dY   = 0;   // Distance from prior to new position

void setup() {

  // Seed random number generator from an unused analog input:
  randomSeed(analogRead(A0));
  
  Serial.begin(9600);
  Serial.println("16x8 LED Matrix Test");

  // Initialize each matrix object:
   for(uint8_t i=0; i<4; i++) {
    matrix[i].begin(matrixAddr[i]);
    
    if(i==3) matrix[i].setRotation(3);
  }
  
  matrix4.begin(matrixAddr[4]);
  matrix4.setRotation(4);
}

void loop() {

  // Draw eyeball in current state of blinkyness (no pupil).  Note that
  // only one eye needs to be drawn.  Because the two eye matrices share
  // the same address, the same data will be received by both.
  //matrix[MATRIX_EYES].clear();
  // When counting down to the next blink, show the eye in the fully-
  // open state.  On the last few counts (during the blink), look up
  // the corresponding bitmap index.
  /*
  matrix[MATRIX_EYES].drawBitmap(2, 3,
    blinkImg[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
      blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
      0                                       // No, show bitmap 0
    ], 16, 8, LED_ON);*/
    for(uint8_t i=0; i<3; i++) {
    matrix[MATRIX_EYES + i].clear();
    matrix[MATRIX_EYES + i].drawBitmap(0-i*8, 0, mouthImg[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
      blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
      0], 24, 8, LED_ON);
    matrix[MATRIX_EYES + i].drawBitmap(0-i*8, 8, mouthImg2[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
      blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
      0], 24, 16, LED_ON);
    }
    
    matrix4.clear();
    matrix4.drawBitmap(0, 0, mouthImg3[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
      blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
      0], 24, 8, LED_ON); 
 
    matrix[3].clear();
    matrix[3].drawBitmap(-8, 0, mouthImg3[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
      blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
      0], 24, 8, LED_ON);
    
 
 if(--blinkCountdown == 0) blinkCountdown = random(5, 180);
 //matrix1.drawBitmap(0, 0, x6_bmp, 8, 8, LED_ON);
  //matrix1.drawBitmap(8, 0, x9_bmp, 8,16, LED_ON);   
    /*
    for(uint8_t i=0; i<2; i++) {
    matrix[MATRIX_EYES + i].clear();
    matrix[MATRIX_EYES + i].drawBitmap(4-i*8, 4, blinkImg[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
      blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
      0                                       // No, show bitmap 0
    ], 8, 8, LED_ON);
  }*/
  // Decrement blink counter.  At end, set random time for next blink.
  if(--gazeCountdown <= gazeFrames) {
    // Eyes are in motion - draw pupil at interim position
    
    if((newX - (dX * gazeCountdown / gazeFrames)) >5){
    
     matrix[2].fillCircle(
      newX - (dX * gazeCountdown / gazeFrames)-8,
      newY - (dY * gazeCountdown / gazeFrames),
      2,LED_OFF);
    
    }
    
    if((newX - (dX * gazeCountdown / gazeFrames)) <3){
    
     matrix[0].fillCircle(
      newX - (dX * gazeCountdown / gazeFrames)+8,
      newY - (dY * gazeCountdown / gazeFrames),
      2,LED_OFF);
    
    matrix4.fillCircle(
      newX - (dX * gazeCountdown / gazeFrames),
      newY - (dY * gazeCountdown / gazeFrames)+8,
      2,LED_OFF);
    }
    
    if((newY - (dY * gazeCountdown / gazeFrames)) >16){
    
     matrix[3].fillCircle(
      newX - (dX * gazeCountdown / gazeFrames),
      newY - (dY * gazeCountdown / gazeFrames)-16,
      2,LED_OFF);
    
    }
    matrix[1].fillCircle(
      newX - (dX * gazeCountdown / gazeFrames),
      newY - (dY * gazeCountdown / gazeFrames),
      2,LED_OFF);
      
     Serial.println(newX - (dX * gazeCountdown / gazeFrames));
     Serial.println(newY - (dY * gazeCountdown / gazeFrames));
      
    if(gazeCountdown == 0) {    // Last frame?
      eyeX = newX; eyeY = newY; // Yes.  What's new is old, then...
      do { // Pick random positions until one is within the eye circle
        newX = random(20); newY = random(20);
        dX   = newX - 3;  dY   = newY - 10;
      } while((dX * dX + dY * dY) >= 40);      // Thank you Pythagoras
      dX            = newX - eyeX;             // Horizontal distance to move
      dY            = newY - eyeY;             // Vertical distance to move
      gazeFrames    = random(3, 15);           // Duration of eye movement
      gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
    }
  } else {
    // Not in motion yet -- draw pupil at current static position
    //matrix[1].fillRect(eyeX, eyeY, 3, 3, LED_OFF);
    //matrix[2].fillRect(eyeX-8, eyeY, 3, 3, LED_OFF);
    matrix[0].fillCircle(eyeX+8, eyeY, 2,LED_OFF);
    matrix[1].fillCircle(eyeX, eyeY, 2,LED_OFF);
    matrix[2].fillCircle(eyeX-8, eyeY, 2,LED_OFF);
    matrix[3].fillCircle(eyeX, eyeY-16, 2,LED_OFF);
    matrix4.fillCircle(eyeX, eyeY+8, 2,LED_OFF);
    
    
    //fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
  }   

  // Refresh all of the matrices in one quick pass
  for(uint8_t i=0; i<4; i++) matrix[i].writeDisplay();
  matrix4.writeDisplay();
  //matrix
  delay(20); // ~50 FPS
}

// Draw mouth image across three adjacent displays
void drawMouth(const uint8_t *img) {
  for(uint8_t i=0; i<3; i++) {
    matrix[MATRIX_MOUTH_LEFT + i].clear();
    matrix[MATRIX_MOUTH_LEFT + i].drawBitmap(i * -8, 0, img, 24, 8, LED_ON);
  }
}

