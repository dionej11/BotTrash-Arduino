/* 
 *NOMBRE DEL CODIGO: Control robot limpiador PIF
 *Diseñadoras: Isabela Ceballos Franco 1001539096
 *             Daniela Jurado Blandón 1001469387
 *Fecha de inicio: 21/05/2021
 *Fecha de finalizacion: 
 *Descripcion: Control remoto mediante una app diseñada en Appinventor para manejar un prototipo robotico que recoja basura
 */

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

String message = "";
char inComigChar;
int contador = 0;
String coordenada;
unsigned long tiempoPres = 0;
//int estado = SerialBT.read();         // inicia detenido

void setup()  { 
  Serial.begin(115200);
  //Serial2.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  unsigned long tiempoTrans = millis();
  
} 

void loop(){
   
 if (Serial.available()) {
    SerialBT.write(Serial.read());
  }  
  if (SerialBT.available()) {
    tiempoPres = millis()- tiempoPres;
    char inComingChar = SerialBT.read();
    if(inComingChar != '\n'){
      message += String(inComingChar);
    }
    if(contador > 0){
        coordenada += String(tiempoPres/1000);
        tiempoPres = millis();
    }
    if(message=="a"){     
        Serial.println("Adelante");
    }
    if(message=="b"){  
      Serial.println("Izquierda"); 
    }
    if(message=="c"){
      Serial.println("Derecha");
    }
    if(message=="d"){ 
      Serial.println("Atras");
    }
    if(message=="e"){ 
      Serial.println("Stop");
    }
    if(message=="f"){  
      Serial.println("Off");
      Serial.println(coordenada);
      
      uint8_t myBuffer[coordenada.length()];
      // Copy at most 32 bytes, but no more than there actually is, into the buffer
      memcpy(myBuffer, coordenada.c_str(),coordenada.length()); 
      //uint8_t* data[50];
      SerialBT.write(myBuffer,sizeof(myBuffer));
      coordenada = "";
    }
    coordenada += message;
    contador++;
    message="";
  }
      delay(20);
}
