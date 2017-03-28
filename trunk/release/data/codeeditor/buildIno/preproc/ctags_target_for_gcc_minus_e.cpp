# 1 "/home/user/Escritorio/simulide_0.1.4/release/SimulIDE_0.1.4-Lin64-NO_PIC/data/codeeditor/buildIno/preino/Blink.ino"
# 1 "/home/user/Escritorio/simulide_0.1.4/release/SimulIDE_0.1.4-Lin64-NO_PIC/data/codeeditor/buildIno/preino/Blink.ino"
  // INOLINE 1
 // INOLINE 2
  // INOLINE 3
// Pin 13 has an LED connected on most Arduino boards. // INOLINE 4
// give it a name: // INOLINE 5
int led = 13; // INOLINE 6
float myvar; // INOLINE 7
 // INOLINE 8
// fin led=13< // INOLINE 9
 // INOLINE 10
// the setup routine runs once when you press reset: // INOLINE 11
void setup() { // INOLINE 12
  myvar = 0.33; // INOLINE 13
  //kk = 0.67; // INOLINE 14
  // initialize the digital pin as an output. // INOLINE 15
  pinMode(led, 0x1); // INOLINE 16
} // INOLINE 17
 // INOLINE 18
// the loop routine runs over and over again forever: // INOLINE 19
void loop() // INOLINE 20
{ // INOLINE 21
    float kk = myvar; // INOLINE 22
 // INOLINE 23
    myvar += 0.1; // INOLINE 24
    digitalWrite(led, 0x1); // turn the LED on (HIGH is the voltage level) // INOLINE 25
    delay(1000); // wait for a second // INOLINE 26
    digitalWrite(led, 0x0); // turn the LED off by making the voltage LOW // INOLINE 27
    delay(1000); // wait for a second // INOLINE 28
    kk += myvar; // INOLINE 29
    if( kk > 3 ) // INOLINE 30
    { // INOLINE 31
        delay( kk ); // INOLINE 32
        kk = myvar-kk; // INOLINE 33
    } // INOLINE 34
    myvar = kk*2; // INOLINE 35
} // INOLINE 36
 // INOLINE 37
 // INOLINE 38
