/*The Babybird Numitron IV-9 Clock with TPIC6B595N shift registers. 
This clock does not use an RTC (real time clock) and instead has the
Arduino keep track of time through counting with the millis function. 
Arrays are currently set up to display time in a 12 hour format.
After 12 hours clock was still perfectly in sync with my computers clock. 
The arrays and counter variables are specifically tailored to one 
another in order to get the correct number to display. This clock
includes two buttons: one to change minutes and hours, and
one which will display the temperature in farenheit and humidity. 
Seconds reset to 0 when minutes are changed. */

//Temperature variables DHT11
#include <dht11.h> 

bool    fHasLooped  = false;

long ran;
long ran2;
long ran3;

int animation=0;

dht11 DHT;                  //Note: DHT on behalf of the DHT11 sensor 
const int dht11_data = 3;   //Please put the DH11`s dht11_data pin connect with arduino digital Port 6
int temp=0;
int temp1=0;
int temp2=0;
int temp3=0;
int temp4=0;
int hum=0;
int hum1=0;
int hum2=0;
int hum3=0;
int hum4=0;
float tempF = 0.0;

//Define TPIC6B595N pins to the Arduino Uno
#define DATA 6
#define LATCH 8
#define CLOCK 10

//Variables for milisecond counter
unsigned long timeNow = 0;
unsigned long timeLast = 0;

//Define the button pins to arduino and beginning variables
const int buttonPin = 4; // the number of the pushbutton pin 
//const int buttonPin2 = 7; // the number of the pushbutton pin 
const int buttonPin3 = 12; // the number of the pushbutton pin
int buttonState = 0; 
int buttonState2 = 0; 
int buttonState3 = 0; 

int lastButtonState = 0; // previous state of the button
int startPressed = 0;    // the time button was pressed
int endPressed = 0;      // the time button was released
int timeHold = 0;        // the time button is hold
int timeReleased = 0;    // the time button is released

//Time start Settings for the hour (do not edit)

int startingHour = 0;
int startingHour2 = 0;

// set your starting hour here, not below at int hour. This ensures accurate daily correction of time
//Numbers split into 1-9 increments to be inserted into the various numitron arrays.
int realseconds = 0;

int seconds = 0;
int seconds2 = 0;
int seconds3 = 0;

int minutes = 0;
int minutes2 = 2;

int hours = 3;
int hours2 = 3;
int hours3 = 0;
int hours4 = 0;

int days = 0;

//Accuracy settings

int dailyErrorFast = 0; // set the average number of milliseconds your microcontroller's time is fast on a daily basis

int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis

int correctedToday = 1; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.

void setup() { // put your setup code here, to run once:
  //initialize TPIC6B595N pins for Arduino
   pinMode(DATA, OUTPUT);
   pinMode(LATCH, OUTPUT);
   pinMode(CLOCK, OUTPUT);
   // initialize the pushbutton pin as an input:
   pinMode(buttonPin, INPUT_PULLUP);
 //  pinMode(buttonPin2, INPUT_PULLUP);
   pinMode(buttonPin3, INPUT_PULLUP);
   //begins serial connection
   Serial.begin(9600); }

//Various arrays for displaying numbers on the Numitron. 
//These coincide with specific counter variables to display the correct hour, minute, temperature, etc.

byte fontArray1[] = {

B01111101, //0
B01010000, //1
B00111110, //2
B01110110, //3
B01010011, //4
B01100111, //5
B01101111, //6
B01110000, //7
B01111111, //8
B01110011, //9
B00000000, //blank
};
byte fontArray2[] = {

B01111110, //0
B00110000, //1
B01011101, //2
B01110101, //3
B00110011, //4
B01100111, //5
B01101111, //6
B01110000, //7
B01111111, //8
B01110011, //9
B00000000, //blank
};
byte fontArray3[] = {

B01010000, //1 2
B00111101, //2 3
B01110101, //3 4
B01010011, //4 5
B01100111, //5 6
B01101111, //6 7 
B01110000, //7 8 
B01111111,
B01110011, //8 9
B01111110, //0 1
B01010000, //1 2
B00111101, //2 3

};
byte fontArray4[] = {

B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B10010000, //1
B10010000, //1
B10010000, //1


};

byte fontArray5[] = {

B10010000,
B10101110,
B10110110,
B10010011,
B00110111,
B00011111,
B10110000,
B10111111,
B10110011,



};

byte tempArray1[] = {

B10111101, //0
B10010000, //1
B10101110, //2
B10110110, //3
B10010011, //4
B00110111, //5
B00111111, //6
B10110000, //7
B10111111, //8
B10110011, //9
B00000000, //blank
};

byte tempArray2[] = {

B01111110, //0
B01010000, //1
B00111101, //2
B01110101, //3
B01010011, //4
B01100111, //5
B01101111, //6
B01110000, //7
B01111111, //8
B01110011, //9
B00000000, //blank
};

byte tempArray3[] = {

B00000000, //blank  8
B11000000, //1  9
};

byte tempArray4[] = {

B00101011, //F
B01011011, //H
};

void loop() { // This is the main loop of the clock

timeNow = millis()/1000; // the number of milliseconds that have passed since boot

seconds = timeNow - timeLast;

buttonState = digitalRead(buttonPin);

buttonState3 = digitalRead(buttonPin3);

    if (buttonState != lastButtonState) {

      // the button was just pressed
      if (buttonState == HIGH) {
          startPressed = millis();
          timeReleased = startPressed - endPressed;

          if (timeReleased >= 0 && timeReleased < 1000) {
              Serial.println("Button idle for half a second"); 
          }
                  if (timeReleased >= 1000) {
              Serial.println("Button idle for one second or more"); 
          }
      // the button was just released
      } else {
          endPressed = millis();
          timeHold = endPressed - startPressed;

          if (timeHold >= 0 && timeHold < 1000) {
              Serial.println("Button hold for half a second"); 
                  minutes = minutes +1;
                  seconds = 0;
                  timeLast = timeNow;
          }

          if (timeHold >= 1000) {
              Serial.println("Button hold for one second or more"); 
              hours = hours +1;
              hours2 = hours2 +1;
          }
      }

  lastButtonState = buttonState;

}


  if (buttonState3 == HIGH) {
  DHT.read(dht11_data);
  temp=DHT.temperature * 1.8 + 32;
  temp1=temp / 10;
  temp2=(temp1 * 10 - temp);
  temp3=-(temp2);
  temp4=temp / 100;
  hum=DHT.humidity;
  hum1=hum / 10;
  hum2=(hum1 * 10 - hum);
  hum3=-(hum2);
  hum4=hum / 100;
  Serial.print("Hum=\t");            
  Serial.print(hum);
  Serial.print("\tTemp=\t");  
  Serial.println(temp);          
  Serial.println(temp1);
  Serial.println(temp3);
  Serial.println(hum1);
  Serial.println(hum3);

//Displays Temperature
  digitalWrite(LATCH, LOW);  
      shiftOut(DATA, CLOCK, LSBFIRST, tempArray4[0]);
      shiftOut(DATA, CLOCK, LSBFIRST, tempArray3[B00000000]);
      shiftOut(DATA, CLOCK, LSBFIRST, tempArray2[temp3]);
      shiftOut(DATA, CLOCK, LSBFIRST, tempArray1[temp1]);
  digitalWrite(LATCH, HIGH);
  delay(3000);
//Displays Humidity
 digitalWrite(LATCH, LOW);
     shiftOut(DATA, CLOCK, LSBFIRST, tempArray4[1]);
     shiftOut(DATA, CLOCK, LSBFIRST, tempArray3[B00000000]);
     shiftOut(DATA, CLOCK, LSBFIRST, tempArray2[hum3]);
     shiftOut(DATA, CLOCK, LSBFIRST, tempArray1[hum1]); 
 digitalWrite(LATCH, HIGH);
 delay(3000);
  }

//the number of seconds that have passed since the last time 10 seconds was reached.
//seconds resets at 10 and adds a 1 to "seconds2"


if (seconds >=60){

  minutes = minutes +1;
    
   timeLast = timeNow;
}

if (minutes == 10){

  minutes = 0;

  minutes2 = minutes2 + 1; }

if (minutes2 == 6){

  minutes2 = 0;

  hours = hours + 1; 

  hours2 = hours2 + 1;

  hours3 = hours3 +1;
  
  animation = animation +1;
}

if (animation == 1){
  animation = 2;
//Variables for temperature to be display every hour
DHT.read(dht11_data);
  temp=DHT.temperature * 1.8 + 32;
  temp1=temp / 10;
  temp2=(temp1 * 10 - temp);
  temp3=-(temp2);
  temp4=temp / 100;

  //Hour change animation
    digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
    digitalWrite(LATCH, HIGH);
    delay(250);

    digitalWrite(LATCH, LOW); 
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);
     
     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B11000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);
     
     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B11100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00110110);
     digitalWrite(LATCH, HIGH);
     delay(200);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00110111);
     digitalWrite(LATCH, HIGH);
     delay(200);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);


     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);
     
     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B11000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B11100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);
     
     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00110110);
     digitalWrite(LATCH, HIGH);
     delay(200);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00110111);
     digitalWrite(LATCH, HIGH);
     delay(200);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
     digitalWrite(LATCH, HIGH);
     delay(100);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(100);

//Displays Temperature
  digitalWrite(LATCH, LOW);  
      shiftOut(DATA, CLOCK, LSBFIRST, tempArray4[0]);
      shiftOut(DATA, CLOCK, LSBFIRST, tempArray3[B00000000]);
      shiftOut(DATA, CLOCK, LSBFIRST, tempArray2[temp3]);
      shiftOut(DATA, CLOCK, LSBFIRST, tempArray1[temp1]);
  digitalWrite(LATCH, HIGH);
  delay(3000);
}

  if (animation == 3) {
        animation = 0;
        seconds ==9;
   if ( fHasLooped == false )
    {
    for ( int x = 0; x < 35; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[ran2]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray5[ran2]);
     digitalWrite(LATCH, HIGH);
     delay(100);
   }

   }
     for ( int x = 0; x < 25; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minutes2]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray5[ran2]);
     digitalWrite(LATCH, HIGH);
     delay(100);
    }

    for ( int x = 0; x < 9; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minutes]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minutes2]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray5[ran2]);
     digitalWrite(LATCH, HIGH);
     delay(100);
    }

    for ( int x = 0; x < 19; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minutes]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minutes2]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hours2]);
     digitalWrite(LATCH, HIGH);
     delay(100);
    }

    for ( int x = 0; x < 8; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minutes]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minutes2]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hours2]);
     digitalWrite(LATCH, HIGH);
     delay(250);
    }

      for ( int x = 0; x < 3; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minutes]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minutes2]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hours2]);
     digitalWrite(LATCH, HIGH);
     delay(1000);
    }
        for ( int x = 0; x < 3; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minutes]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minutes2]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[hours]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hours2]);
     digitalWrite(LATCH, HIGH);

     delay(1000);

       digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay (1000);
     
    }
    fHasLooped = true;
    }

//When hours reach 12 the hour counters will reset.
if (hours == 12){
  hours = 0;
  hours2 = 0;
}

// After 24 hours adds 1 day to the clock (not used in this current setup)
if (hours3 == 24){
hours3 = 0;
days = days + 1;
}

//if 24 hours have passed, add one day
if (hours3 ==(24 - startingHour) && correctedToday == 0);{
delay(dailyErrorFast*1000);
seconds = seconds + dailyErrorBehind;
correctedToday = 1; }

//every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the program with some milliseconds.

//Change these varialbes according to the error of your board.

// The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days

// and then determining how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).

if (hours == 24 - startingHour + 2);{
correctedToday = 0; }

//let the sketch know that a new day has started for what concerns correction, if this line was not here the arduiono // would continue to correct for an entire hour that is 24 - startingHour.


//This section displays the current time onto the Numitrons using the counters plugged into the byte arrays.  
    digitalWrite(LATCH, LOW);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minutes]);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minutes2]);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[hours]);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hours2]);
    digitalWrite(LATCH, HIGH);

//Serial for debugging.    
Serial.print("The time is: ");
Serial.print(days);
Serial.print(":");
Serial.print(hours2);
Serial.print(":");
Serial.print(hours);
Serial.print(":");
Serial.print(minutes2);
Serial.print(":");
Serial.print(minutes);
Serial.print(":");
Serial.print(seconds);
Serial.println(" ");
}

