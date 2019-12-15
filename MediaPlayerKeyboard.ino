/*
 * MediaPlayer Keyboard
 * (copyleft)2019 Giovanni Bernardo
 * https://www.settorezero.com
 * 
 * Read teory on encoder here:
 * https://bit.ly/2qVRCcv
 * 
 * Install "HID-project by Nicohood" library
 * I've used an Olimexino 32U4 (Arduino Leonardo)
 * Since I had some problems on certains I/O using internal Pull-Ups
 * I've mounted external 10K pull-up resistors on GPIOs used
 */

#include "HID-Project.h"

// Encoder pins. 
// I use P and D letters for indicating Pulse and Direction
// Those pins must be connected to GPIOs capable of Interrupt on state change
// Leonardo has Interrupts on input change on GPIOs 0,1,2,3 and 7
#define encoderP  2
#define encoderD  3

// buttons
#define buttonA 4
#define buttonB 5
#define buttonC 6
#define buttonD 7

// led - not used
#define led LED_BUILTIN

bool prevState=false; // previous encoder state: false=>dir & pulse different levels, true=>dir & pulse had same value

void setup() 
  {
  pinMode(led, OUTPUT);
  pinMode(encoderP, INPUT);
  pinMode(encoderD, INPUT);
  pinMode(buttonA, INPUT);
  pinMode(buttonB, INPUT);
  pinMode(buttonC, INPUT);
  pinMode(buttonD, INPUT);
    
  // Sends a clean report to the host. This is important on any Arduino type.
  Consumer.begin();

  // initial state of encoder
  if ((digitalRead(encoderP))==(digitalRead(encoderD))) prevState=true;
  
  // Attach different state-change interrupts on encoder pins
  attachInterrupt(digitalPinToInterrupt(encoderP), encoderTick_P, CHANGE); // Pulse interrupt
  attachInterrupt(digitalPinToInterrupt(encoderD), encoderTick_D, CHANGE); // Direction interrupt
  }

void loop() 
  {
  // A button pressed (the one on GPIO 4) => STOP
  if (!digitalRead(buttonA)) 
    {
    delay(40);
    if (!digitalRead(buttonA))
      {
      Consumer.write(MEDIA_STOP);
      }
    }

  // B button pressed (the one on GPIO 5) => PLAY/PAUSE
  if (!digitalRead(buttonB)) 
    {
    delay(40);
    if (!digitalRead(buttonB))
      {
      Consumer.write(MEDIA_PLAY_PAUSE);
      }
    }

  // C button pressed (the one on GPIO 6) => NEXT SONG
  if (!digitalRead(buttonC)) 
    {
    delay(40);
    if (!digitalRead(buttonC))
      {
      Consumer.write(MEDIA_NEXT);
      }
    }

  // D button pressed (the one on GPIO 7) => PREVIOUS SONG
  if (!digitalRead(buttonD)) 
    {
    delay(40);
    if (!digitalRead(buttonD))
      {
      Consumer.write(MEDIA_PREVIOUS);
      }
    }
 }

// encoder Pulse interrupt: the pin marked as "pulse" (GPIO 2) has changed state
void encoderTick_P()
  {
  // interrupt on Pulse: encoder was rotated clockwise if pulse and dir are at same level
  // and previous state was at different levels
  uint8_t pulse=digitalRead(encoderP);
  uint8_t dir=digitalRead(encoderD);
  
  if (pulse==dir) // encoder rotated clockwise (Pulse interrupt)
     {
     if (!prevState) Consumer.write(MEDIA_VOLUME_UP); 
     prevState=true;
     }
  else // encoder rotated anti-clockwise
     {
     if (prevState) Consumer.write(MEDIA_VOLUME_DOWN); 
     prevState=false;
     }
  }

// encoder Direction interrupt: the pin marked as "direction" (GPIO 3) has changed state
void encoderTick_D()
  {
  // interrupt on Dir: encoder was rotated clockwise if pulse and dir are at different levels
  // and previous state was at same level
  uint8_t pulse=digitalRead(encoderP);
  uint8_t dir=digitalRead(encoderD);
  
  if (pulse==dir) // encoder rotated anti-clockwise (Direction interrupt)
     {
     if (!prevState) Consumer.write(MEDIA_VOLUME_DOWN); 
     prevState=true;
     }
  else // encoder rotated clockwise
     {
     if (prevState) Consumer.write(MEDIA_VOLUME_UP); 
     prevState=false;
     }
  }
