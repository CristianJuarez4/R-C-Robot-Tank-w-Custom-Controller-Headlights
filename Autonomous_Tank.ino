int trig= 2;
int echo= 4;

long duration;
int distance;

int OUT1= 5; //right motor fwd
int OUT2= 6; //right motor bwd
int OUT3= 9; //left motor fwd
int OUT4= 10;//left motor bwd

int led = A2;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

pinMode(trig,OUTPUT);
pinMode(echo,INPUT);

pinMode(motorA,OUTPUT);
pinMode(brakeA,OUTPUT);

pinMode(motorB,OUTPUT);
pinMode(brakeB,OUTPUT);

pinMode(led,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(trig, LOW);
digitalWrite(trig, HIGH);
digitalWrite(trig, LOW);
duration = pulseIn(echo, HIGH);
distance= duration*0.034/2;
Serial.print("Distance: ");
Serial.println(distance);

analogWrite(led,255);

if (distance > 15)
{
  digitalWrite(OUT1,180);
  digitalWrite(OUT2,0);
  digitalWrite(OUT3,180);
  digitalWrite(OUT4,0);
}
  else if (distance < 15)
  {
    digitalWrite(OUT1,0);
    digitalWrite(OUT2,180);
    digitalWrite(OUT3,0);
    digitalWrite(OUT4,180);
}
}
