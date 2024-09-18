#include <Keypad.h>
#include <Servo.h>

const byte rows = 4, cols = 4;

char keyArray[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'}, 
  {'*', '0', '#', 'D'}
};

byte rowPorts[rows] = {11, 10, 9, 8}, colPorts[cols] = {7, 6, 5, 4};

Keypad myKeypad = Keypad(makeKeymap(keyArray), rowPorts, colPorts, rows, cols);

const int buzzerPort = 3; bool octaveUp = false; int lightPort = A0; bool sharp = false;

const int fanInput = A2, fanOutput = 12;

const int killSwitch = 13; int killSwitchState = 0;

const int volumeInput = A3;

void setup()
{
  Serial.begin(9600);
  pinMode(buzzerPort, OUTPUT);
  pinMode(lightPort, INPUT);
  pinMode(fanInput, INPUT);
  pinMode(fanOutput, OUTPUT);
  pinMode(killSwitch, INPUT);
  pinMode(volumeInput, INPUT);
}

void loop()
{

  killSwitchState = digitalRead(killSwitch);

  char val = myKeypad.getKey(), note; int octave; 
  int lightReading = analogRead(lightPort); 

  int fanAnalogReading = analogRead(fanInput);
  fanAnalogReading = map(fanAnalogReading, 0, 1023, 0, 255);
  
  if(killSwitchState == LOW) analogWrite(fanOutput, fanAnalogReading);
  
  if(val && (killSwitchState == LOW))
  {
    if(val == '1') note = 'C';
    else if(val == '2') note = 'D';
    else if(val == '3') note = 'E';
    else if(val == '4') note = 'F';
    else if(val == '5') note = 'G';
    else if(val == '6') note = 'A';
    else if(val == '7') note = 'B';
    else if(val == '8') { note = 'C'; octaveUp = true; }
    else if(val == 'A') octave = 4; 
    else if(val == 'B') octave = 5;
    
    if(val >= '1' && val <= '8') 
    {
      analogWrite(fanOutput, fanAnalogReading);
      while(myKeypad.getState() != RELEASED)
      {
        val = myKeypad.getKey();
        if(lightReading < 500) sharp = true;
        if(octaveUp) { 
          if(sharp) 
          {
            if(octave == 4) tone(buzzerPort, 554);
            else tone(buzzerPort, 1109);
          }
          else 
          {
            if(octave == 4) tone(buzzerPort, 523);
            else tone(buzzerPort, 1047);
          }
        }
        else play(note, octave, sharp);
      }
    }
    noTone(buzzerPort);
    octaveUp = false;
    sharp = false;
  }
}

void play(char note, int octave, bool sharp) 
{
  int freq; 
  if(octave == 4)
  {
    if(note == 'C') freq = 262;
    else if(note == 'D') freq = 294;
    else if(note == 'E') freq = 330;
    else if(note == 'F') freq = 349; 
    else if(note == 'G') freq = 392;
    else if(note == 'A') freq = 440;  
    else if(note == 'B') freq = 494; 

    if(sharp)
    {
      if(note == 'C') freq = 277;
      if(note == 'D') freq = 311;
      if(note == 'F') freq = 370;
      if(note == 'G') freq = 415;
      if(note == 'A') freq = 466;
    }

  }

  else if(octave == 5) 
  {
    if(note == 'C') freq = 523;
    else if(note == 'D') freq = 587;
    else if(note == 'E') freq = 659;
    else if(note == 'F') freq = 698; 
    else if(note == 'G') freq = 784;
    else if(note == 'A') freq = 880;  
    else if(note == 'B') freq = 988;

    if(sharp)
    {
      if(note == 'C') freq = 554;
      if(note == 'D') freq = 622;
      if(note == 'F') freq = 740;
      if(note == 'G') freq = 831;
      if(note == 'A') freq = 932;
    }
  }
  tone(buzzerPort, freq);
}
