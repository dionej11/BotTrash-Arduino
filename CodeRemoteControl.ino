/* 
 *NOMBRE DEL CODIGO: Control robot limpiador PIF
 *Diseñadoras: Isabela Ceballos Franco 1001539096
 *             Daniela Jurado Blandón 1001469387
 *Fecha de inicio: 21/05/2021
 *Fecha de finalizacion: 
 *Descripcion: Control remoto mediante una app diseñada en Android Studio para manejar un prototipo robotico que recolecte basura
 */
#include "BluetoothSerial.h"//libreria Bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define ECHO 35//pin del Echo ultrasonido
#define TRIGGER 32//pin del Trigger ultrasonido

BluetoothSerial SerialBT;//Canal de transmisión de datos
#include <analogWrite.h>//libreria para el analogWrite

int enA = 25; // PWM Motor A
int in1 = 13;  // Control Rotacion Motor A
int in2 = 12;  // Control Rotacion Motor A

int enB = 26; // PWM Motor B
int in3 = 14; // Control Rotacion Motor B
int in4 = 27;  // Control Rotacion Motor 

int motor_speed = 175; //velocidad alta motores

String message = ""; //Guarda la letra presionada
char inComigChar; //Guarda el caracter enviado via Bluetooth
int contador = 0; //Contador para controlar el tiempo de cada coordenada
String coordenada; //Guarda el recorrido final del robot
unsigned long tiempoPres = 0; //Maneja el tiempo

void setup()  { 
  Serial.begin(115200); //Inicializa el serial para la comunicación
  SerialBT.begin("ESP32test"); //Nombre del dispositivo Bluetooth 
  Serial.println("The device started, now you can pair it with bluetooth!");
  unsigned long tiempoTrans = millis(); //Asignar el tiempo transcurrido desde la inicialización del ESP32
  
  /**********************************Se asignan los pines de entrada y salida*************************************************/
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, LOW);//Inicializamos el Trigger con 0
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
/**********Velocidad de los motores**********/
  analogWrite(enA, motor_speed); 
  analogWrite(enB, motor_speed);
} 

void loop(){
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }  
  if (SerialBT.available()) { //Validación del funcionamiento del socket
    tiempoPres = millis()- tiempoPres;
    char inComingChar = SerialBT.read(); //Lectura del caracter entrante
    if(inComingChar != '.'){
        message += String(inComingChar); //Concatenación de la letra enviada
    }
    else{
      limpiarLugar(); //Función para limpiar automaticamente
    }
    if(contador > 0){
        coordenada += String(tiempoPres/1000); //Se concatena el tiempo de duración del movimiento ejecutado
        tiempoPres = millis();
    }
    /********Condicionales de los movimientos del robot********/
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
    /******Se envia la coordenada total a la app para ser subida a la BD******/
    if(message=="f"){  
      Serial.println("Se envió la coordenada final");
      Serial.println(coordenada);
      
      uint8_t myBuffer[coordenada.length()];
      // Copy at most 32 bytes, but no more than there actually is, into the buffer
      memcpy(myBuffer, coordenada.c_str(),coordenada.length()); 
      SerialBT.write(myBuffer,sizeof(myBuffer));
      coordenada = ""; //Reiniciar la variable
    } 
    coordenada += message;
    contador++;
    message=""; //Reiniciar la variable
  }
  delay(20);
}
void adelante(){
  digitalWrite(in1, LOW); //DERECHA
  digitalWrite(in2, HIGH);  

  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH);  //IZQUIERDA
}
void atras(){
  digitalWrite(in1, HIGH); //DERECHA
  digitalWrite(in2, LOW); 

  digitalWrite(in3, HIGH); //IZQUIERDA
  digitalWrite(in4, LOW);  
}
void parar(){
  digitalWrite(in1, LOW); //DERECHA
  digitalWrite(in2, LOW); 

  digitalWrite(in3, LOW); //IZQUIERDA
  digitalWrite(in4, LOW);
}
void derecha(){  
  digitalWrite(in1, LOW); //DERECHA
  digitalWrite(in2, LOW); 

  digitalWrite(in3, LOW); //IZQUIERDA
  digitalWrite(in4, HIGH);
}
void izquierda(){  
  digitalWrite(in1, LOW); //DERECHA
  digitalWrite(in2, HIGH); 

  digitalWrite(in3, LOW); //IZQUIERDA
  digitalWrite(in4, LOW);
}
void limpiarLugar(){
  int tiempoLimpieza = 0; //Duración de la limpieza automatica
  
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
