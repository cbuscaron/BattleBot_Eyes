

#include <stdint.h>
#include <TouchScreen.h>
#include <TFT.h>

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 


int ch0; // Here's where we'll keep our channel values
int ch1;
int poX=160;
int poY=120;

int xPrev = poX;
int yPrev = poY;


void setup()
{

Tft.init();  //init TFT library

/*  Demo of 
    void drawCircle(int poX, int poY, int r,unsigned int color) and
    void fillCircle(int poX, int poY, int r,unsigned int color);
*/
Tft.fillCircle(120, 160, 120,WHITE);
Tft.fillCircle(poY, poX, 40,BLACK);


pinMode(0, INPUT); // Set our input pins as such
pinMode(1, INPUT);


//Serial.begin(9600); // Pour a bowl of Serial
}

void loop()
{
  
  ch0 = pulseIn(0, HIGH, 25000); // Read the pulse width of 
  ch1 = pulseIn(1, HIGH, 25000); // each channel
  
  poX = map(ch0, 1000,2000,60,260);
  poY = map(ch1, 1000,2000,60,200);
  //Serial.print("Right Stick X:"); // Ch3 was x-axis 
  //Serial.println(map(ch0, 1150,1750,0,240)); // center at 0
  //Serial.println(poX);
  
  //Serial.print("Right Stick Y:"); // Ch2 was y-axis
  //Serial.println(map(ch1, 950,1900,0,320)); // center at 0
  //Serial.println(ch1);
   //Serial.println(); //make some room

  //delay(500);// I put this here just to make the terminal 
           // window happier
           
  //Tft.fillCircle(120, 160, 120,WHITE);
  
  //Tft.fillCircle(poY, poX, 40,BLACK);
  
  if((poX >= xPrev + 3 || poX <= xPrev - 3 ) || (poY <= yPrev -3 || poY >= yPrev + 3)){
      Tft.fillCircle(yPrev, xPrev, 46,WHITE);
      Tft.fillCircle(poY, poX, 40,BLACK);
  
  }
  
  xPrev = poX;
  yPrev = poY; 
  
  delay(300);
  
  
}
