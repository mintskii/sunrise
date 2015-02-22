/*Pseudo Code
 1. Define the pins (LED pin 6?)
 2. Setup a timer
 2. Define a variable for start colour and end colour
 3. Set up the LED pin as an output
 3b.Set up the button A pin as an input
 4. Set up a loop
 1. fade the lights on to start colour
 2. slowly fade between start colour and end colour
 3. hold the light on until button A is pressed
 
Wait loop
  wait for alarm time, then call Sunrise
  or wait for button press, then call Nightlight

Sunrise
  come on automatically early in the morning,
  Monday to Friday, fade up then stay on until 6:40.
  Then flash on/off until button a is pressed.
  Then stay on for 20 miniutes, or until button a pressed again.
  Then switch off and wait for next alarm time
  
Nightlight
  anytime outside of Sunrise alarm time wait for button a press
  then fade up light to white.
  stay on until next button press or 01:30am
  then fade down
 
 */
const int buttonPin = 7;
int buttonState = 0;
int buttonPushCounter = 0;
int lastButtonState = 0;

#include <Adafruit_NeoPixel.h>
#include <Time.h>
#include <TimeAlarms.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(40, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  setTime(22,40,0,1,1,11);
  Alarm.alarmRepeat(5,55,0,sunrise);
  //Alarm.alarmRepeat(22,01,0,sunrise);
  //Alarm.alarmRepeat(22,02,0,sunrise);
}

void loop() {
  Serial.println("void loop");
  // put your main code here, to run repeatedly: 
  //
  //  strip.setPixelColor(n, r, g, b); //n=pixel number, rgb=0-255
  //or
  //  strip.setPixelColor(n, color); //n=pixel number, color=32-bit type
  //  i.e. uint32_t magenta = strip.Color(255,0,255);
  //then
  //  strip.show();
  //
  //uint32_t color = strip.getPixelColor(n); //query returns 32-bit merged color value of previously set pixel
  //strip.setBrightness(64); //adjusts overall brightness of all LEDs (but may be jittery)

  digitalClockDisplay();
  Alarm.delay(1000); // wait one second between clock display
  nightLight();
}

void nightLight(){
  Serial.println("void nightLight");
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState){
    if (buttonState == HIGH){

        buttonPushCounter++;
        Serial.println("on");
        Serial.print("number of button pushes:  ");
        Serial.println(buttonPushCounter);
      }
      else{
        Serial.println("off");
      }
  }
  lastButtonState = buttonState;
  if (buttonPushCounter % 2 == 0) {
          for(byte n = 0; n < strip.numPixels(); n++){
        strip.setPixelColor(n, 170, 100, 20);
        strip.show();
      }
  }
      else{
              for(byte n = 0; n < strip.numPixels(); n++){
        strip.setPixelColor(n, 0, 0, 0);
        strip.show();
      }
      }
  }

void digitalClockDisplay()
{
  //Serial.println("void digitalClockDisplay");
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits)
{
  //Serial.println("void printDigits");
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

//variables declaration
//need to work out how to reset these after each sunrise loop
byte colour[] = {
  0, 0, 0};
byte preDawnR = 20;
unsigned long preDawnT = 120000; // 2 minutes
byte brightR = 200;
byte brightG = 100;
//byte brightB = 255;
byte greenStart = (brightR - brightG) - 1;
unsigned long dawnT = 600000; // 10 minutes
byte finalBrightR = 255;
byte finalBrightG = 150;
byte finalBrightB = 100;
unsigned long daylightT = 1500000; // 25 minutes

void sunrise() {
  
  Serial.println("void sunrise");
  while (colour[0] < preDawnR) {
    preDawn();
  }
  while (colour[0] < brightR) {
    dawn();
  }
  while (colour[2] < finalBrightB) {
    daylight();
  }
}

void preDawn(){
  Serial.print("preDawn ");
  for (byte n = 0; n < strip.numPixels(); n++){
    strip.setPixelColor(n, colour[0], colour[1], colour[2]);
    strip.show();
  }
  colour[0] += 1;
  delay(preDawnT/preDawnR); // 2 minutes/20 steps = 6 seconds
  feedback();
}

void dawn(){
  Serial.print("DAWN ");
  for (byte n = 0; n < strip.numPixels(); n++){
    strip.setPixelColor(n, colour[0], colour[1], colour[2]);
    strip.show();
  }
  colour[0] += 1;
  if (colour[0] < greenStart) {
    colour[1] = 0;
  }
  else if (colour[0] > greenStart && colour[0] < (brightR - 1)) {
    colour[1] += 1;
  }
  else if (colour[0] > (brightR - 1)) {
    colour[1] = brightG;
  }
  delay(dawnT/brightR); // 10 minutes/200 = 3 seconds ???
  feedback();
}

void daylight(){
  Serial.print("DAYLIGHT ");
  for (byte n = 0; n < strip.numPixels(); n++){
    strip.setPixelColor(n, colour[0], colour[1], colour[2]);
    strip.show();
  }
  if (colour[0] < finalBrightR){
    colour[0] += 1;
  }
  else if (colour[1] < finalBrightG){
    colour[1] += 1;
  }
  else if(colour[2] < finalBrightB){
    colour[2] += 1;
  }
  /* started here
  else if (colour[2] == (finalBrightB-1)){
    buttonPushCounter++;
    delay(5000);
  }
  ended here*/
  delay(daylightT/finalBrightR); // 25 minutes/255 steps = 5.8 seconds
  feedback();
}

void feedback(){
  //Serial.println("void feedback");
  Serial.print("LED ");
  Serial.print(0);
  Serial.print(" is at ");
  Serial.print(colour[0]);
  Serial.print(", ");
  Serial.print(colour[1]);
  Serial.print(", ");
  Serial.println(colour[2]);
  //delay(200);
}

