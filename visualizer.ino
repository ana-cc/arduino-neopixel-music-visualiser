#include <Adafruit_NeoPixel.h>

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
String inString ="";
char inChar ='!';
int n;
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(115200);
  }
  
  void loop() {

  while (Serial.available() > 0) {
      inChar = Serial.read();
      inString += inChar;
      uint32_t a,off;
      int r1,r2,r3;
      a=strip.Color(2,23,45);
      if (inChar == '\n') {
         int i=0;
         for (i=0;i<inString.length()-1;i++){
         if (i%2 ==1) {
             a=strip.Color(45,0,45);}
         else {
               r1=random(25);
               r2=random(25);
               r3=random(25);        
               a=strip.Color(r1,r2,r3);}
         n = inString[i] - '0';
         mod_col(i, n, a);
  }
      
         strip.show();
         
         inString = "";
     } }
  }
  
   void mod_col(int col, int val, uint32_t c)
        {
          int i;
          Serial.println(col); 
          Serial.println(val);
          Serial.println(c);
          for (i=0;i<=val;i++)
              {strip.setPixelColor(col+8 *i,c);
          }
          for (i=val;i<=5;i++){
            strip.setPixelColor(col+8 * i, strip.Color(0,0,0));
          }

        }


   void mod_off(uint32_t off)
       {int i;
          for (i=0;i<=39;i++)
              {strip.setPixelColor(i,off);}
       }
        
