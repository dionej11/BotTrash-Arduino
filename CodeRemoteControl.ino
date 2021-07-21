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

BluetoothSerial SerialBT;
#include <analogWrite.h>

int enA = 25; // PWM Motor A
int in1 = 13;  // Control Rotacion Motor A
int in2 = 12;  // Control Rotacion Motor A

int enB = 26; // PWM Motor B
int in3 = 14; // Control Rotacion Motor B
int in4 = 27;  // Control Rotacion Motor 

int motor_speed = 180; //velocidad alta motores
int motor_speed2 = 160;//velocidad baja motores

String message = "";
char inComigChar;
int contador = 0;
int bnd = 0;
String coordenada;
String coordenada_in;
unsigned long tiempoPres = 0;

void setup()  { 
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  unsigned long tiempoTrans = millis();

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
      if(inComingChar == 'h'){
        Serial.println("quinta activity");
        bnd++;
        coordenada_in = "";
        coordenada = "";
      }else{
        message += String(inComingChar);
      }
    }
    else{
      Serial.println("finaliza la coordenada: ");
      Serial.print(coordenada_in);
      bnd = 0;
      limpiar(coordenada_in);
    }
    if(contador > 0){
        coordenada += String(tiempoPres/1000);
        tiempoPres = millis();
    }
    if(message=="a" && bnd == 0){     
        Serial.println("Adelante");
        adelante();
    }
    if(message=="b" && bnd == 0){  
      Serial.println("Izquierda"); 
      izquierda();
    }
    if(message=="c" && bnd == 0){
      Serial.println("Derecha");
      derecha();
    }
    if(message=="d" && bnd == 0){ 
      Serial.println("Atras");
      atras();
    }
    if(message=="e" && bnd == 0){ 
      Serial.println("Stop");
      parar();
    }
    if(message=="f" && bnd == 0){  
      Serial.println("Off");
      Serial.println(coordenada);
      
      uint8_t myBuffer[coordenada.length()];
      // Copy at most 32 bytes, but no more than there actually is, into the buffer
      memcpy(myBuffer, coordenada.c_str(),coordenada.length()); 
      SerialBT.write(myBuffer,sizeof(myBuffer));
      coordenada = "";
    } 
    coordenada += message;
    coordenada_in += message;
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
  analogWrite(enA, motor_speed2); 
  analogWrite(enB, motor_speed2);
  
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW); 

  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH);
}

void izquierda(){
  analogWrite(enA, motor_speed2); 
  analogWrite(enB, motor_speed2);
  
  digitalWrite(in1, LOW); //DERECHA
  digitalWrite(in2, HIGH); 

  digitalWrite(in3, LOW); //IZQUIERDA
  digitalWrite(in4, LOW);
}
void limpiar(String lugar){//a1e1
  int i = 0;
  while(i < lugar.length()){
    char letra = lugar.charAt(i);
    switch (letra) {
      case 'a':
        {
          Serial.println("entra al case a");
          String cadena_tiempo = "";
          i++;
          while(isDigit(lugar.charAt(i))) {
            cadena_tiempo += lugar.charAt(i);
            i++;
          }
          int tiempo = cadena_tiempo.toInt();
          adelante();
          Serial.println(tiempo);
          delay(tiempo*1000);
        }
        break; 

      case 'b':
        {
          Serial.println("entra al case b");
          String cadena_tiempo = "";
          i++;
          while(isDigit(lugar.charAt(i))) {
            cadena_tiempo += lugar.charAt(i);
            i++;
          }
          int tiempo = cadena_tiempo.toInt();
          izquierda();
          Serial.println(tiempo);
          delay(tiempo*1000);
        }
        break; 

      case 'c':
        {
          Serial.println("entra al case c");
          String cadena_tiempo = "";
          i++;
          while(isDigit(lugar.charAt(i))) {
            cadena_tiempo += lugar.charAt(i);
            i++;
          }
          int tiempo = cadena_tiempo.toInt();
          derecha();
          Serial.println(tiempo);
          delay(tiempo*1000);
        }
        break; 

      case 'd':
        {
          Serial.println("entra al case d");
          String cadena_tiempo = "";
          i++;
          while(isDigit(lugar.charAt(i))) {
            cadena_tiempo += lugar.charAt(i);
            i++;
          }
          int tiempo = cadena_tiempo.toInt();
          atras();
          Serial.println(tiempo);
          delay(tiempo*1000);
        }
        break; 

      case 'e':
        {
          Serial.println("entra al case e");
          String cadena_tiempo = "";
          i++;
          while(isDigit(lugar.charAt(i))) {
            cadena_tiempo += lugar.charAt(i);
            i++;
          }
          int tiempo = cadena_tiempo.toInt();
          parar();
          Serial.println(tiempo);
          delay(tiempo*1000);
        }
        break; 
    }
  }
  Serial.println("sale del while y para");
  parar();
  delay(3000);
}
