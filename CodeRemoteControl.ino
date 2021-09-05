/* 
 *NOMBRE DEL CODIGO: Control robot limpiador PIF
 *Diseñadoras: Isabela Ceballos Franco 1001539096
 *             Daniela Jurado Blandón 1001469387
 *Fecha de inicio: 21/05/2021
 *Fecha de finalizacion: 
 *Descripcion: Control remoto mediante una app diseñada en Android Studio para manejar un prototipo robotico que recolecte basura
 */
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define ECHO 35
#define TRIGGER 32

BluetoothSerial SerialBT;
#include <analogWrite.h>

int enA = 25; // PWM Motor A
int in1 = 13;  // Control Rotacion Motor A
int in2 = 12;  // Control Rotacion Motor A

int enB = 26; // PWM Motor B
int in3 = 14; // Control Rotacion Motor B
int in4 = 27;  // Control Rotacion Motor 

int motor_speed = 175; //velocidad alta motores

String message = "";
char inComigChar;
int contador = 0;
//int bnd = 0;
String coordenada;
//String coordenada_in;
unsigned long tiempoPres = 0;

void setup()  { 
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  unsigned long tiempoTrans = millis();

  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  
  digitalWrite(TRIGGER, LOW);//Inicializamos el pin con 0
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  analogWrite(enA, motor_speed); 
  analogWrite(enB, motor_speed);
} 

void loop(){
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }  
  if (SerialBT.available()) {
    tiempoPres = millis()- tiempoPres;
    char inComingChar = SerialBT.read();
    if(inComingChar != '.'){
        message += String(inComingChar);
    }
    else{
      limpiarLugar();
    }
    if(contador > 0){
        coordenada += String(tiempoPres/1000);
        tiempoPres = millis();
    }
    if(message=="a"){     
        Serial.println("Adelante");
        adelante();
    }
    if(message=="b"){  
      Serial.println("Izquierda"); 
      izquierda();
    }
    if(message=="c"){
      Serial.println("Derecha");
      derecha();
    }
    if(message=="d"){ 
      Serial.println("Atras");
      atras();
    }
    if(message=="e"){ 
      Serial.println("Stop");
      parar();
    }
    if(message=="f"){  
      Serial.println("Off");
      Serial.println(coordenada);
      
      uint8_t myBuffer[coordenada.length()];
      // Copy at most 32 bytes, but no more than there actually is, into the buffer
      memcpy(myBuffer, coordenada.c_str(),coordenada.length()); 
      SerialBT.write(myBuffer,sizeof(myBuffer));
      coordenada = "";
    } 
    coordenada += message;
    contador++;
    message="";
  }
  delay(20);
}
void adelante(){
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);  

  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH);  
}
void atras(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); 

  digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW);  
}
void parar(){
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW); 

  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW);
}
void derecha(){  
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW); 

  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH);
}
void izquierda(){  
  digitalWrite(in1, LOW); //DERECHA
  digitalWrite(in2, HIGH); 

  digitalWrite(in3, LOW); //IZQUIERDA
  digitalWrite(in4, LOW);
}
void limpiarLugar(){
  int tiempoLimpieza = 0;
  while(tiempoLimpieza < 20){
      long t; //timepo que demora en llegar el eco
      long d; //distancia en centimetros
    
      digitalWrite(TRIGGER, HIGH);
      delayMicroseconds(10);          //Enviamos un pulso de 10us
      digitalWrite(TRIGGER, LOW);
      
      t = pulseIn(ECHO, HIGH); //obtenemos el ancho del pulso
      d = t/59;             //escalamos el tiempo a una distancia en cm
      
      Serial.print("Distancia: ");
      Serial.print(d);      //Enviamos serialmente el valor de la distancia
      Serial.print("cm");
      Serial.println();
      delay(100);          //Hacemos una pausa de 100ms
      adelante();
      if(d >= 15 && d < 50){
        Serial.println("obstaculo");
        parar();
        delay(2000);
        atras();
        delay(1000);
        derecha();
        delay(600);
        tiempoLimpieza++;
      }
  }
}
