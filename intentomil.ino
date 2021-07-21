#include <analogWrite.h>

int enA = 25; // PWM Motor A
int in1 = 13;  // Control Rotacion Motor A
int in2 = 12;  // Control Rotacion Motor A

int enB = 26; // PWM Motor B
int in3 = 14; // Control Rotacion Motor B
int in4 = 27;  // Control Rotacion Motor 

int motor_speed = 180; 

void setup()
{
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  analogWrite(enA, motor_speed); 
  analogWrite(enB, motor_speed); 
}

void loop()
{
//ADELANTE
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);  

  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH);  
  delay(5000);
//STOP
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW); 

  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW);  
  delay(5000);
//ATRAS
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); 

  digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW);  
  delay(5000);
}
