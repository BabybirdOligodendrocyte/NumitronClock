
#include <TimeLib.h>
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 
#include <dht11.h>
#include <DS3231.h>

DS3231  rtc(SDA, SCL);

Time t;

dht11 DHT;                  //Note: DHT on behalf of the DHT11 sensor
const int dht11_data = 3;   //Please put the DH11`s dht11_data pin connect with arduino digital Port 6
int temp = 0;
int temp1 = 0;
int temp2 = 0;
int temp3 = 0;
int temp4 = 0;
int hum = 0;
int hum1 = 0;
int hum2 = 0;
int hum3 = 0;
int hum4 = 0;
float tempF = 0.0;

int wait = 0;
int oldhour = 0;
long ran;
long ran2;
long ran3;

//Define TPIC6B595N pins to the Arduino Uno
#define DATA 6
#define LATCH 8
#define CLOCK 10

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

void setup()  {
  //initialize TPIC6B595N pins for Arduino
  pinMode(DATA, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
     pinMode(buttonPin, INPUT_PULLUP);
 //  pinMode(buttonPin2, INPUT_PULLUP);
   pinMode(buttonPin3, INPUT_PULLUP);
   //begins serial connection
  Serial.begin(115200);
  while (!Serial) ; // Needed for Leonardo only
  pinMode(13, OUTPUT);
  rtc.begin();
  setSyncProvider(requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message (Unix Timestamp e.g. '1510548236')");
}

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

};
byte fontArray2[] = {

  B01111110, //0
  B01111110, //0
  B01111110, //0
  B01111110, //0
  B01111110, //0
  B01111110, //0
  B01111110, //0
  B01111110, //0
  B01111110, //0
  B01111110, //0
  B00110000, //1
  B00110000, //1
  B00110000, //1
  B00110000, //1
  B00110000, //1
  B00110000, //1
  B00110000, //1
  B00110000, //1
  B00110000, //1
  B00110000, //1
  B01011101, //2
  B01011101, //2
  B01011101, //2
  B01011101, //2
  B01011101, //2
  B01011101, //2
  B01011101, //2
  B01011101, //2
  B01011101, //2
  B01011101, //2
  B01110101, //3
  B01110101, //3
  B01110101, //3
  B01110101, //3
  B01110101, //3
  B01110101, //3
  B01110101, //3
  B01110101, //3
  B01110101, //3
  B01110101, //3
  B00110011, //4
  B00110011, //4
  B00110011, //4
  B00110011, //4
  B00110011, //4
  B00110011, //4
  B00110011, //4
  B00110011, //4
  B00110011, //4
  B00110011, //4
  B01100111, //5
  B01100111, //5
  B01100111, //5
  B01100111, //5
  B01100111, //5
  B01100111, //5
  B01100111, //5
  B01100111, //5
  B01100111, //5
  B01100111, //5
  B00000000, //blank
};
byte fontArray3[] = {

  B00111101, //2
  B01010000, //1
  B00111101, //2
  B01110101, //3
  B01010011, //4
  B01100111, //5
  B01101111, //6
  B01110000, //7
  B01110011, //8
  B01110011, // 9
  B01111110, //0
  B01010000, //1 2
  B00111101, //2 3
  B01010000, //1
  B00111101, //2
  B01110101, //3
  B01010011, //4
  B01100111, //5
  B01101111, //6
  B01110000, //7
  B01110011, //8
  B01110011, // 9
  B01111110, //0
  B01010000, //1 2

};
byte fontArray4[] = {

  B10010000, //1
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

byte fontArray6[] = {

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

void loop() {
  buttonState = digitalRead(buttonPin);
  buttonState3 = digitalRead(buttonPin3);
  
  if (wait == 0) {
    waitmessage();
  }

  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus() != timeNotSet) {
    digitalClockDisplay();
  }
  if (timeStatus() == timeSet) {
    digitalWrite(13, HIGH); // LED on if synced
  } else {
    digitalWrite(13, LOW);  // LED off if needs refresh
  }
  if ((minute() == 59) && (second() == 59)) {
    houranimation();
    temperature();
  }
  if ((hour() == 4) && (minute() == 20)) {
    weedanimation();
    rtcmodule2();
  }
  if ((hour() == 16) && (minute() == 20)) {
    weedanimation();
  }
  if ((minute() == 15) && (second() == 00) || (minute() == 30) && (second() == 00) || (minute() == 45) && (second() == 00)) {
    temperature();
  }
  if (wait == 0) {
     rtcmodule();
  }
 if (buttonState == HIGH) {
  t = rtc.getTime();
  rtc.setTime((t.hour),(t.min + 1),(t.sec));
  delay(100);
  rtcmodule2();
  }
 if (buttonState3 == HIGH) {
  t = rtc.getTime();
  rtc.setTime((t.hour),(t.min - 1),(t.sec));
  delay(100);
  rtcmodule2();
  }
  if (buttonState3 == HIGH && buttonState == HIGH) {
  t = rtc.getTime();
  rtc.setTime((t.hour + 1),(t.min),(t.sec));
  delay(100);
  rtcmodule2();
  }

  delay(1000);
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(month());
  Serial.print("/");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(year());
  Serial.print(" ");
  Serial.print(temp);
  Serial.print("F");
  Serial.print(" ");
  Serial.print(hum);
  Serial.print("H");
  Serial.println();

  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
  shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
  shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[hour()]);
  shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hour()]);
  digitalWrite(LATCH, HIGH);
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void waitmessage() {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B01000000);
  digitalWrite(LATCH, HIGH);
  delay(500);
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B10000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  digitalWrite(LATCH, HIGH);
  delay(500);
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B10000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  digitalWrite(LATCH, HIGH);
  delay(500);
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B10000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  digitalWrite(LATCH, HIGH);
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if (Serial.available()) {
    pctime = Serial.parseInt();
    if ( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
      setTime(pctime); // Sync Arduino clock to the time received on the serial port
      wait = 1;
    }
  }
}

void houranimation() {
  for ( int x = 0; x < 35; x++ ) {// Loop to do "something" n times
    // Do "something"

    digitalWrite(LATCH, LOW);
    ran = random(0, 9);
    ran2 = random(0, 9);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[ran]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray5[ran2]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray6[ran2]);
    digitalWrite(LATCH, HIGH);
    delay(100);
  }

  for ( int x = 0; x < 25; x++ ) {// Loop to do "something" n times
    // Do "something"

    digitalWrite(LATCH, LOW);
    ran = random(0, 9);
    ran2 = random(0, 9);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[ran]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray6[ran2]);
    digitalWrite(LATCH, HIGH);
    delay(100);
  }

  for ( int x = 0; x < 9; x++ ) {// Loop to do "something" n times
    // Do "something"

    digitalWrite(LATCH, LOW);
    ran = random(0, 9);
    ran2 = random(0, 9);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray6[ran2]);
    digitalWrite(LATCH, HIGH);
    delay(100);
  }

  for ( int x = 0; x < 19; x++ ) {// Loop to do "something" n times
    // Do "something"

    digitalWrite(LATCH, LOW);
    ran = random(0, 9);
    ran2 = random(0, 9);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hour()]);
    digitalWrite(LATCH, HIGH);
    delay(100);
  }

  for ( int x = 0; x < 8; x++ ) {// Loop to do "something" n times
    // Do "something"

    digitalWrite(LATCH, LOW);
    ran = random(0, 9);
    ran2 = random(0, 9);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hour()]);
    digitalWrite(LATCH, HIGH);
    delay(250);
  }

  for ( int x = 0; x < 3; x++ ) {// Loop to do "something" n times
    // Do "something"

    digitalWrite(LATCH, LOW);
    ran = random(0, 9);
    ran2 = random(0, 9);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hour()]);
    digitalWrite(LATCH, HIGH);
    delay(1000);
  }
  for ( int x = 0; x < 3; x++ ) {// Loop to do "something" n times
    // Do "something"

    digitalWrite(LATCH, LOW);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[hour()]);
    shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hour()]);
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
  delay(500);
}

void weedanimation() {
  //hash
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B01011011);
  shiftOut(DATA, CLOCK, LSBFIRST, B01100111);
  shiftOut(DATA, CLOCK, LSBFIRST, B01111011);
  shiftOut(DATA, CLOCK, LSBFIRST, B10011011);
  digitalWrite(LATCH, HIGH);
  delay (1000);
  //dope
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B00101111);
  shiftOut(DATA, CLOCK, LSBFIRST, B01011011);
  shiftOut(DATA, CLOCK, LSBFIRST, B01001101);
  shiftOut(DATA, CLOCK, LSBFIRST, B10011110);
  digitalWrite(LATCH, HIGH);
  delay (1000);
  //ganj
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B01010100);
  shiftOut(DATA, CLOCK, LSBFIRST, B00101001);
  shiftOut(DATA, CLOCK, LSBFIRST, B01111011);
  shiftOut(DATA, CLOCK, LSBFIRST, B00111111);
  digitalWrite(LATCH, HIGH);
  delay(1000);
  //herb
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B01001111);
  shiftOut(DATA, CLOCK, LSBFIRST, B00001001);
  shiftOut(DATA, CLOCK, LSBFIRST, B00101111);
  shiftOut(DATA, CLOCK, LSBFIRST, B10011011);
  digitalWrite(LATCH, HIGH);
  delay(1000);
  //bud
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B01011110);
  shiftOut(DATA, CLOCK, LSBFIRST, B00101100);
  shiftOut(DATA, CLOCK, LSBFIRST, B01001111);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  digitalWrite(LATCH, HIGH);
  delay(1000);
  //indo
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B01001110);
  shiftOut(DATA, CLOCK, LSBFIRST, B00111101);
  shiftOut(DATA, CLOCK, LSBFIRST, B01001001);
  shiftOut(DATA, CLOCK, LSBFIRST, B10010000);
  digitalWrite(LATCH, HIGH);
  delay(1000);
  //dab
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B01001111);
  shiftOut(DATA, CLOCK, LSBFIRST, B01111011);
  shiftOut(DATA, CLOCK, LSBFIRST, B01011101);
  shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
  digitalWrite(LATCH, HIGH);
  delay(1000);
  //fire
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B00101111);
  shiftOut(DATA, CLOCK, LSBFIRST, B00001001);
  shiftOut(DATA, CLOCK, LSBFIRST, B01010000);
  shiftOut(DATA, CLOCK, LSBFIRST, B00101011);
  digitalWrite(LATCH, HIGH);
  delay(1000);
  //loud
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, B01011110);
  shiftOut(DATA, CLOCK, LSBFIRST, B00101100);
  shiftOut(DATA, CLOCK, LSBFIRST, B01001101);
  shiftOut(DATA, CLOCK, LSBFIRST, B00001101);
  digitalWrite(LATCH, HIGH);
  delay(1000);
  //420
  delay (1000);
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
  shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
  shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[hour()]);
  shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hour()]);
  digitalWrite(LATCH, HIGH);
  delay (1000);

}

void temperature() {
  //Displays Temperature
  DHT.read(dht11_data);
  temp = DHT.temperature * 1.8 + 32;
  temp1 = temp / 10;
  temp2 = (temp1 * 10 - temp);
  temp3 = -(temp2);
  temp4 = temp / 100;
  hum = DHT.humidity;
  hum1 = hum / 10;
  hum2 = (hum1 * 10 - hum);
  hum3 = -(hum2);
  hum4 = hum / 100;
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

void rtcmodule(){
  unsigned long pctime2;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if (wait == 0) {
    pctime2 = (rtc.getUnixTime(rtc.getTime()));
    if ( pctime2 >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
      setTime(pctime2); // Sync Arduino clock to the time received on the serial port
      wait = 1;
  // Send Unixtime
  // ** Note that there may be a one second difference between the current time **
  // ** and current unixtime show if the second changes between the two calls   **
  Serial.println(rtc.getUnixTime(rtc.getTime()));
    }
}
}

void rtcmodule2(){
  unsigned long pctime3;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

    pctime3 = (rtc.getUnixTime(rtc.getTime()));
    if ( pctime3 >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
      setTime(pctime3); // Sync Arduino clock to the time received on the serial port
      wait = 1;    
      
  // Send Unixtime
  // ** Note that there may be a one second difference between the current time **
  // ** and current unixtime show if the second changes between the two calls   **
  Serial.println(rtc.getUnixTime(rtc.getTime()));
    }
    }

void changetime(){

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
                rtc.setTime((t.hour),(t.min + 1),(t.sec));
          }

          if (timeHold >= 1000) {
              Serial.println("Button hold for one second or more"); 
              rtc.setTime((t.hour),(t.min - 1),(t.sec));
          }
      }

  lastButtonState = buttonState;

}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);
  return 0; // the time will be sent later in response to serial mesg
}

