//#include <SoftwareSerial.h>

/*
  GHOSTBUSTERS TRAP LED CONTROL.
  EH, 26Oct2025 onward
  v1 - combined examples of Fade and For Loop "Knightrider", but doesn't work togther due to sticking in loop.
  v2 - re-written as fader_step and scanner_step functions.
  v3 - main loop sequence used to direct activities, scanner pauses for other indicators (simplest just to pause!).
  v4 - updated pinout for Arduino Nano Every; adjusted scanner function; added debug functions and startup check

  Based on Arduino Example Fade and "Knightrider", but rewritten with subroutines.
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Fade
  https://docs.arduino.cc/built-in-examples/control-structures/ForLoopIteration/

  ROUGH PROCESS.
  * switch on
  * 4x small LED as scanning, knightrider sequence
  * large red LED as "we got one", pulses in and out slowly
  * then internal white flicker, 1ms fader
  * reset sequence
*/
//==========================

// SETUP INITIAL VARIABLES
int brightness = 0;  //initial LED brightness for all
int fadeAmount = 5;  //default LED fade step out of 255
enum enumdir {UP,DOWN}; //set custom words
enumdir scan_direction = UP; //set initial
int scan_start_pin = 14; //scanner start from this pin
int scan_max_pin = 17; // highest pin ID used for scanner LEDs
int scan_next_pin = scan_start_pin + 1;
int scan_interval = 100;  //interval for scanner, ms
int maincounter = 0;
int loopcounter = 0;

int ledPins[] = { 3, 5, 14, 15, 16, 17 }; //see SETUP for connected indicators
int ledCount = 6;


// SETUP ROUTINE - runs once when you press reset
void setup() {

  // flash onboard LED as hello
  debug_led_flash(5);
 
  // declare LED pins as outputs:
  pinMode(3, OUTPUT); // white, inside box, effect
  pinMode(5, OUTPUT); // red, outside box, main indicator
  pinMode(14, OUTPUT); // blue1, outside box, scan progress
  pinMode(15, OUTPUT); // blue2, outside box, scan progress
  pinMode(16, OUTPUT); // blue3, outside box, scan progress
  pinMode(17, OUTPUT); // blue4, outside box, scan progress

  //sequence check
  debug_led_step(ledPins,6,100);

  //initialise scanner LEDs
  scanner_full(false);
  scan_next_pin = scan_start_pin;

  delay (3000);
}

//==========================

// MAIN LOOP ROUTINE - runs over and over again forever
void loop() {

delay(100); //per cycle
maincounter = maincounter +1; //increment activity counter

  //GO INDICATORS: scanner
  scan_next_pin = scanner_step(scan_next_pin, scan_interval); //also uses global variables for max and start
//  scan_start_pin = scan_next_pin;

  if (maincounter == 100)
  {
  //GO INDICATORS: we got one!
  scanner_full(true);
  fader(5,5,10,5); //red (led 5) outside indicator x5 pulse
  delay(250);
  scanner_full(false);
  //flash trap
  fader(3,5,1,100); //white (led 3) flicker effect
  
  delay (2000);
  maincounter = maincounter + 1;
  };

  //more scanning

  if (maincounter == 250)
  {
    //GO INDICATORS: false alarm or recharge
    scanner_full(false);
    delay (500);
    digitalWrite(14,HIGH);
    digitalWrite(17,HIGH);
    fader(5,10,50,4); //red (led 5) outside indicator x4 really slow pulse

    //reset
    maincounter = 0;
    scanner_full(false); // turn off blue LEDs
  }

}

//==========================

//DEBUG FUNCTIONS
void debug_led_flash(int count) {
  pinMode(13, OUTPUT); //onboard
  for (int i = 0; i < count; i++) {
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13,LOW);
    delay(500);
  }
}

void debug_led_step(int ledArray[], int ledCount, int msInterval) {
  for (int thisPin = 0; thisPin < ledCount; thisPin++) {
    digitalWrite(ledArray[thisPin], HIGH);
    delay(msInterval);
    digitalWrite(ledArray[thisPin], LOW);
    delay(msInterval);
  }
}

//NEW FADER FUNCTION
void fader(int pin_id, int fadeAmount, int interval_ms, int fadeCount) {

int i=0;
do
  {
  // fade in from min to max by variable steps:
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += fadeAmount) {
    // sets the value (range from 0 to 255):
    analogWrite(pin_id, fadeValue);
    // pause for dimming step
    delay(interval_ms);
  }

  // fade out from max to min by variable steps:
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= fadeAmount) {
    // sets the value (range from 255 to 0):
    analogWrite(pin_id, fadeValue);
    // pause for dimming step
    delay(interval_ms);
  }
  i++;
  } while (i<fadeCount);
}

/*
//OLD FADER FUNCTION
void fader(int pin_id, int fadeAmount, int interval_ms) {
while(loopcounter < 2048)
  {
  // set the brightness of PWM pin:
  analogWrite(pin_id, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // delay between steps for dimming effect
  delay(interval_ms);

  loopcounter = loopcounter++;
  }
}
*/

//SCANNER FUNCTIONS

void scanner_full(bool all_leds) {
  if (all_leds == true)
  {
  digitalWrite(14,HIGH);
  digitalWrite(15,HIGH);
  digitalWrite(16,HIGH);
  digitalWrite(17,HIGH);
  }
  else
  {
  digitalWrite(14,LOW);
  digitalWrite(15,LOW);
  digitalWrite(16,LOW);
  digitalWrite(17,LOW);
  }
}  


int scanner_step(int scanPin, int scan_interval) {
// scanner range and direction is set in initial variables

int prevPin;
int nextPin;

//Serial.println(scanPin);  //DEBUG

// Set next and previous Pin indexes
if (scan_direction == UP) {
  if (scanPin == scan_start_pin) {
//    debug_led_flash(2);
    prevPin = scanPin -1; //or use scan_max_pin
    nextPin = scanPin +1;
    scan_direction = UP;
  }
  else if (scanPin == scan_max_pin) {
    prevPin = scan_start_pin;
    nextPin = scanPin -1;
    scan_direction = DOWN; //reset direction
  }
  else
  {
    prevPin = scanPin -1;
    nextPin = scanPin +1;
    scan_direction = UP;
  }
}
else if (scan_direction == DOWN) {
  if (scanPin == scan_max_pin) {
    prevPin = scanPin +1; // or use scan_start_pin
    nextPin = scanPin -1;
  }
  else if (scanPin == scan_start_pin) {
    prevPin = scan_max_pin;
    nextPin = scanPin +1;
    scan_direction = UP; //reset direction
  }
  else
  {
    prevPin = scanPin +1;
    nextPin = scanPin -1;
  }
}

// change to new state
digitalWrite(scanPin, HIGH);
digitalWrite(prevPin, LOW);

 //wait
delay(scan_interval);
/* 
// reset direction if needed
if (thisPin == maxPin) {
  scan_direction = DOWN;
  }
  else if (thisPin == scan_start_pin) {
    scan_direction = UP;
  }
*/

return (nextPin);
}
