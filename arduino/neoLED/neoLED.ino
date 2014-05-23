#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6

#define TL_START 1
#define TL_STOP 27
#define TR_START 29
#define TR_STOP 55

Adafruit_NeoPixel strip = Adafruit_NeoPixel(56, PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial mySerial(2, 3); // RX, TX
String rawCmd;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
   mySerial.begin(9600);
   Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}

void loop() {
        if (mySerial.available() > 0) {
                //cmd[index] = mySerial.read();
                rawCmd = mySerial.readString();
                Serial.println(rawCmd);
                
                parseCmd(rawCmd);
        }       
}
void parseCmd(String cmd){
  if(getValue(cmd,1) == "wipe"){ //wipe <pos> <delay> <R> <G> <B>
    int r, g, b, wait;
    String pos;
    pos = getValue(cmd,2);
    wait = getValue(cmd,3).toInt();
    r = getValue(cmd,4).toInt();
    g = getValue(cmd,5).toInt();
    b = getValue(cmd,6).toInt();
    colorWipe2(strip.Color(r,g,b),wait,pos);
  }else if(getValue(cmd,1) == "rainbowCycle"){ //rainbowCycle <delay>
    rainbowCycle(getValue(cmd,2).toInt());
  }
} //END parsecmd()


String getValue(String data, int index){
  // FROM: http://stackoverflow.com/a/14824108
  char separator = ':';
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
} //END getValue()



////////////////////////////////
//
// DISPLAYS
//
////////////////////////////////
void pixel(uint32_t c, uint8_t wait, uint8_t ledStart, uint8_t ledStop) {
  for(uint16_t i=ledStart-1; i<ledStop; i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void colorWipe2(uint32_t c, uint8_t wait, String pos) {
  int ledStart, ledStop;
  if (pos == "TL"){
    ledStart = TL_START;
    ledStop = TL_STOP;
  }else if (pos == "TR"){
    ledStart = TR_START;
    ledStop = TR_STOP;
  }
  for(uint16_t i=ledStart-1; i<ledStop; i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

///////////////////
// From Adafruit examples
///////////////////
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}



