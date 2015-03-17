/*
 
 */

int ch1; // Here's where we'll keep our channel values
int ch2;
int ch3;

void setup() {

pinMode(5, INPUT); // Set our input pins as such
pinMode(6, INPUT);
pinMode(7, INPUT);

Serial.begin(9600); // Pour a bowl of Serial

}

void loop() {
  
  ch1 = pulseIn(5, HIGH, 25000); // Read the pulse width of 
  ch2 = pulseIn(6, HIGH, 25000); // each channel
  ch3 = pulseIn(7, HIGH, 25000);
  
  if(ch1>1000){Serial.println("Left Switch: Engaged");} 
  if(ch1<1000){Serial.println("Left Switch: Disengaged");}
  /* I found that Ch1 was my left switch and that it 
  floats around 900 in the off position and jumps to 
  around 1100 in the on position */

Serial.print("Right Stick X:"); // Ch3 was x-axis 
Serial.println(map(ch3, 1150,1750,-500,500)); // center at 0

Serial.print("Right Stick Y:"); // Ch2 was y-axis
Serial.println(map(ch2, 950,1900,-500,500)); // center at 0

Serial.println(); //make some room

delay(500);// I put this here just to make the terminal 
           // window happier
}

