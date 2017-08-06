/*
 * If you use the serial monitor for debugging remember to ster the baud rate to 115200.
 * The modules are set to only one-way communication. That means that the controller can only transmit and the tank can only receive.
 * If you wish to change this for two-way communication there is plenty of documentation to do so on the internet and in examples.
 * 
 * This program receives the X and Y values and processes it into movement.
 */
#include <SPI.h>  
#include "RF24.h" 

RF24 myRadio (7, 8); 
struct package
{
  int X=512;
  int Y=512;
};

byte addresses[][6] = {"0"}; 
int OUT1 = 5; // right motor fwd
int OUT2 = 6; // right motor bwd
int OUT3 = 9; // left motor fwd
int OUT4 = 10;// left motor bwd

typedef struct package Package;
Package data;

int pResistor = A0;
int ledPin = 3;

int light;
int brightness;

int trig = 2;
int echo = 4;

long duration;
int distance;

void setup() 
{
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(ledPin,OUTPUT);
  pinMode(pResistor,INPUT);
  Serial.begin(115200);
  delay(1000);

  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  analogWrite(OUT1, 0);
  analogWrite(OUT2, 0);
  analogWrite(OUT3, 0);
  analogWrite(OUT4, 0);
  
  analogWrite(ledPin,0);
}

void loop()  
{
  light = analogRead(pResistor);
  brightness = map(light, 0, 600, 255, 0);
  if (light < 600){
    analogWrite(ledPin, brightness);
  }
  else if (light > 600){
    analogWrite(ledPin,0);
  }
  if ( myRadio.available()) 
  {
    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
    }
    Serial.print("X:");
    Serial.print(data.X);
    Serial.print("      Y");
    Serial.println(data.Y);
    int X = data.X;
    int Y = data.Y;
    int foward = map(X,524,1024,0,255);
    int backward = map(X,500,0,0,255);
    int right = map(Y,524,1024,0,255);
    int left = map(Y,500,0,0,255);
    digitalWrite(trig, LOW);
    digitalWrite(trig, HIGH);
    digitalWrite(trig, LOW);
    duration = pulseIn(echo, HIGH);
    distance = duration*0.034/2;
    if(X > 524 && Y < 524 && Y > 500 && distance > 15){
      analogWrite(OUT3, foward);
      analogWrite(OUT4, 0);
      analogWrite(OUT1, foward);
      analogWrite(OUT2, 0);
    }else if(X < 500 && Y < 524 && Y > 500 && distance > 15){
      analogWrite(OUT4, backward);
      analogWrite(OUT3, 0);
      analogWrite(OUT2, backward);
      analogWrite(OUT1, 0);
    }else if(X < 524 && X > 500 && Y < 524 && Y > 500 distance > 15){
      analogWrite(OUT4, 0);
      analogWrite(OUT3, 0);
      analogWrite(OUT2, 0);
      analogWrite(OUT1, 0);
    }else if(X < 524 && X > 500 && Y > 524 && distance > 15){
      analogWrite(OUT4, 0);
      analogWrite(OUT3, left);
      analogWrite(OUT2, left);
      analogWrite(OUT1, 0);
    }else if(X < 524 && X > 500 && Y < 500 && distance > 15){
      analogWrite(OUT4, right);
      analogWrite(OUT3, 0);
      analogWrite(OUT2, 0);
      analogWrite(OUT1, right);
    }else if(X > 524 && Y > 524 && distance > 15){
      analogWrite(OUT3, foward);
      analogWrite(OUT4, 0);
      analogWrite(OUT1, foward-right);
      analogWrite(OUT2, 0);
    }else if(X > 524 && Y < 500 && distance > 15){
      analogWrite(OUT3, foward-left);
      analogWrite(OUT4, 0);
      analogWrite(OUT1, foward);
      analogWrite(OUT2, 0);
    }else if(X < 500 && Y > 524 && distance > 15){
      analogWrite(OUT4, backward);
      analogWrite(OUT3, 0);
      analogWrite(OUT2, backward-right);
      analogWrite(OUT1, 0);
    }else if(X < 500 && Y < 500 && distance > 15){
      analogWrite(OUT4, backward-left);
      analogWrite(OUT3, 0);
      analogWrite(OUT2, backward);
      analogWrite(OUT1, 0);
    }else if(distance < 15){ // Safety Mechanism
      analogWrite(OUT1, 0);
      analogWrite(OUT2, 0);
      analogWrite(OUT3, 0);
      analogWrite(OUT4, 0);
    }else if(X < 524 && X > 500 && Y > 524 && distance > 15){
      analogWrite(OUT1, 0);
      analogWrite(OUT2, 180);
      analogWrite(OUT3, 180);
      analogWrite(OUT4, 0);
    }else if(X < 524 && X > 500 && Y < 500 && distance > 15){
      analogWrite(OUT1, 180);
      analogWrite(OUT2, 0);
      analogWrite(OUT3, 0);
      analogWrite(OUT4, 180);
    }
  }
}
