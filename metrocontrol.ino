#include <Metro.h>

/*
  metronomsteuerung
 	
ein drehinkrementalregler steuert einen periodischen klick/blink...

	
 The circuit:
 * drehinkrementalgeber attached to pin 6 and 7 from +5V
 * 10K resistor attached to pin 2 from ground
 * LED attached from pin 13 to ground (or use the built-in LED on
   most Arduino boards)
 

 */
 
 
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(5, 8, 9, 10, 11, 12);

// this constant won't change:
const int  buttonOnePin = 6;    // the pins for the drehinkr
const int  buttonTwoPin = 7;
const int ledPin = 13;       // the pin that the LED is attached to

// Variables will change:
unsigned short tempo = 120;
int knobState = 0;
int lastKnobState = 0;     // previous state of the button
unsigned short knobVal = 480; //knob value

int tempoMs = 10000;

Metro ledMetro = Metro(tempoMs);




void setup() {
  // initialize the button pins as a input:
  pinMode(buttonOnePin, INPUT);
  pinMode(buttonTwoPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(8, 2);
// Print a message to the LCD.
lcd.print("tempo");
// Move the cursor to the beginning of the second address block
// (note: line 1 is the second row, since counting begins with 0):
lcd.setCursor(5, 1);
lcd.print("120");

}

void tock() {
  digitalWrite(ledPin, HIGH);
  delay(5);
  digitalWrite(ledPin, LOW);
  return;
}

char knobAction() {
    // read the two input pins:
  knobState = (digitalRead(buttonOnePin) << 1) | (digitalRead(buttonTwoPin));
  if (knobState != lastKnobState) {
  
  
    switch (knobState) {
    
      case 0:
      if (lastKnobState == 2) knobVal-- ;
      if (lastKnobState == 1) knobVal++ ;
      break;
      
      case 1:
      if (lastKnobState == 0) knobVal-- ;
      if (lastKnobState == 3) knobVal++ ;
      break;
      
      case 2:
      if (lastKnobState == 3) knobVal-- ;
      if (lastKnobState == 0) knobVal++ ;
      break;
      
      case 3:
      if (lastKnobState == 1) knobVal-- ;
      if (lastKnobState == 2) knobVal++ ;
      break;
      
  }
  if (knobVal < 160) knobVal = 160;
  if (knobVal > 1000) knobVal = 1000;
  
  if (knobVal < 400) {
    lcd.setCursor(5, 1);
  // remove the digit here:
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("tempo");
  lcd.setCursor(6, 1);
  }
  if (knobVal >= 400) lcd.setCursor(5, 1);

  
  tempo = (knobVal >> 2);
  tempoMs = (60000 / tempo);
  
  // save the current state as the last state, 
  //for next time through the loop
  lastKnobState = knobState;
  
  return 1;

  }
  else {
  // save the current state as the last state, 
  //for next time through the loop
  lastKnobState = knobState;
    return 0;
  }
}


void loop() {

  
  if (knobAction() ) {
  Serial.println(tempo, DEC);
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):

  // print tempo:
 lcd.print(tempo, DEC);
  
  ledMetro.interval(tempoMs);
  
  }
    
  if (ledMetro.check() ) {
    tock();
    ledMetro.interval(tempoMs);
  } else {
      delay(1);  //wait 2millisec
  }
  
}









