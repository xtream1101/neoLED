#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <RGBConvert.h>

#define PIN0 6


Adafruit_NeoPixel strip[] = {Adafruit_NeoPixel(120, PIN0, NEO_GRB + NEO_KHZ800)};
RGBConvert convert;

SoftwareSerial wifi(2, 3); // RX, TX
String rawCmd;
bool cycle = false;
 
void setup(){

    strip[0].begin();
    strip[0].show(); // Initialize all pixels to 'off'

    wifi.begin(9600);
    Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
} //END setup()

void loop(){

    if(cycle) parseCmd(rawCmd);
    
    if (wifi.available() > 0){
        rawCmd = wifi.readString();
        Serial.println(rawCmd);
        parseCmd(rawCmd); 
    }   
       
} //END loop()

void parseCmd(String cmd){
    //Set default values for all
    int ledStrip = 0;
    int delay = 30;
    int startLed = 0;
    int length = 1;
    int sublength = 1;
    int dir = 0;
    int color1[10][3];

    if(getValue(cmd, 2) == "1"){
        cycle = true;
    }else{
        cycle = false;
    }
    if(getValue(cmd,1) == "slide"){

        ledStrip = getValue(cmd,3).toInt();
        startLed = getValue(cmd,4).toInt();
        length = getValue(cmd,5).toInt();
        delay = getValue(cmd,6).toInt();
        color1[0][0] = getValue(cmd,7).toInt();
        color1[0][1] = getValue(cmd,8).toInt();
        color1[0][2] = getValue(cmd,9).toInt();
        color1[1][0] = getValue(cmd,10).toInt();
        color1[1][1] = getValue(cmd,11).toInt();
        color1[1][2] = getValue(cmd,12).toInt();        
        sublength = getValue(cmd,13).toInt();
        dir = getValue(cmd,14).toInt();

        uint32_t tempColor1 = strip[ledStrip].Color(color1[0][0], color1[0][1], color1[0][2]);
        uint32_t tempColor2 = strip[ledStrip].Color(color1[1][0], color1[1][1], color1[1][2]);

        slide(ledStrip, startLed, length, delay, tempColor1, tempColor2, sublength, dir);

    }else if(getValue(cmd,1) == "solid"){

        ledStrip = getValue(cmd,3).toInt();
        startLed = getValue(cmd,4).toInt();
        length = getValue(cmd,5).toInt();
        delay = getValue(cmd,6).toInt();
        color1[0][0] = getValue(cmd,7).toInt();
        color1[0][1] = getValue(cmd,8).toInt();
        color1[0][2] = getValue(cmd,9).toInt();
        dir = getValue(cmd,10).toInt();
        uint32_t tempColor = strip[ledStrip].Color(color1[0][0], color1[0][1], color1[0][2]);

        solid(ledStrip, startLed, length, delay, tempColor, dir);

    }else if(getValue(cmd,1) == "rCycle"){

        ledStrip = getValue(cmd,3).toInt();
        startLed = getValue(cmd,4).toInt();
        length = getValue(cmd,5).toInt();
        delay = getValue(cmd,6).toInt();
        color1[0][0] = getValue(cmd,7).toInt();
        color1[0][1] = getValue(cmd,8).toInt();
        color1[0][2] = getValue(cmd,9).toInt();
        color1[1][0] = getValue(cmd,10).toInt();
        color1[1][1] = getValue(cmd,11).toInt();
        color1[1][2] = getValue(cmd,12).toInt();        
        dir = getValue(cmd,13).toInt();

        rCycle(ledStrip, startLed, length, delay, color1, dir);
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


void slide(int port, int start, int length, int wait, uint32_t subColor, uint32_t bgColor, int sublength, int dir){
    int i,j;
    uint32_t setC;

    int end = start+length-1;
    for(j=0; j<length+sublength-1; j++){
        for(i=start-1; i<end; i++){
            if( (i <= j && j < sublength) || (j >= sublength && i <= j && i >= (j - sublength)+1) )
                setC = subColor;
            else
                setC = bgColor;
            strip[port].setPixelColor(i, setC);
        }
        strip[port].show();
        delay(wait);  
    }
} //END slide()


//TODO
void slideBounce(int port, int start, int length, int wait, uint32_t subColor, uint32_t bgColor, int sublength, int dir){
    int i,j;
    uint32_t setC;

    int end = start+length-1;
    for(j=0; j<length+sublength; j++){
        for(i=start-1; i<end; i++){
            if( (i <= j && j < sublength) || (j >= sublength && i <= j && i >= (j - sublength)+1) )
                setC = subColor;
            else
                setC = bgColor;
            strip[port].setPixelColor(i, setC);
        }
        strip[port].show();
        delay(wait);  
    }
} //END slide()


void solid(int port, int start, int length, int wait, uint32_t c, int dir){
    int end = start+length-1;
    for(int i=start-1; i<end; i++){
        strip[0].setPixelColor(i, c);
        strip[0].show();
        delay(wait);
    }
} //END solid()


void rCycle(int port, int start, int length, int wait, int c[10][3], int dir){
    int i, j, colorFade;
    float middle, change;
    int currHSV[3], hsvStart[3], hsvEnd[3], rgb[3];

    int r1 = c[0][0];
    int g1 = c[0][1];
    int b1 = c[0][2];
    int r2 = c[1][0];
    int g2 = c[1][1];
    int b2 = c[1][2];

    convert.rgbTohsv(r1, g1, b1, hsvStart);
    convert.rgbTohsv(r2, g2, b2, hsvEnd);

    currHSV[0] = hsvStart[0];
    currHSV[1] = hsvStart[1];
    currHSV[2] = hsvStart[2];

    middle = length/2;
    change = abs(hsvStart[0] - hsvEnd[0])/middle;

    if(hsvStart[0] < hsvEnd[0])
        colorFade = 1;
    else
        colorFade = -1;

    int end = start+length-1;
    int pos;
    for(j=start-1;j<end;j++){
        for(i=start-1;i<end;i++){
            pos = abs(j - i);
            if(pos >= middle)
                pos = length - pos;
            currHSV[0] = hsvStart[0]+(change*pos*colorFade);
            convert.hsvTorgb(currHSV[0], currHSV[1], currHSV[2], rgb);
            strip[port].setPixelColor(i, strip[port].Color(int(rgb[0]),int(rgb[1]),int(rgb[2])));
        }
        strip[port].show();
        delay(wait);
    }
} //END rCycle()


//TODO
void rCycleBounce(int wait, uint32_t c1, uint32_t c2){
    int i, j;
    j=0;
    int k =0;
    for(j=0; j<85*2; j++){
        k=j;
        if(j>85){
            k = (85*2)-j;
        }
        for(i=0; i< 9; i++){            
            strip[0].setPixelColor(i, Wheel(((i * 85 / 9) + k) & 255));
        }

        strip[0].show();
        delay(wait);
    }
}

////////////////////////////
// From Adafruit examples //
////////////////////////////

void rainbowCycle(uint8_t wait){
    uint16_t i, j;

    for(j=0; j<256; j++){
        for(i=0; i< strip[0].numPixels(); i++){
            strip[0].setPixelColor(i, Wheel(((i * 256 / strip[0].numPixels()) + j) & 255));
        }
        strip[0].show();
        delay(wait);
    }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos){
    if(WheelPos < 85) {
        return strip[0].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }else if(WheelPos < 170){
        WheelPos -= 85;
        return strip[0].Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }else{
        WheelPos -= 170;
        return strip[0].Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}



