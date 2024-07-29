/* 
  Proyecto: Sensor de proximidad
  Descripción: Se hace uso del sensor de ultrasonido HC-SR04 para medir la distancia a cualquier objeto, e indicar con una señal lumínica y sonora
  la cercanía y peligro de colisión
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 26/07/2024
  Última modificación: 27/07/2024
*/

#include <EasyUltrasonic.h> // Se carga la librería para hacer uso del sensor de Ultrasonido HC-SR04

// ------------------------------------------------------------------------------
// Definiciones de pines
// ------------------------------------------------------------------------------

#define pinTrigger 5  // Pin al que está conectado el Pin de Disparo del sensor (Trigger)
#define pinEcho 18    // Pin al que está conectado el Pin Receptor del sensor (Echo)

#define pinRojo 15    // Pin al que está conectado la terminal R del LED RGB 
#define pinVerde 2    // Pin al que está conectado la terminal G del LED RGB 
#define pinAzul 0     // Pin al que está conectado la terminal B del LED RGB 

#define pinBuzzer 4   // Pin al que está conectado el Buzzer

// ------------------------------------------------------------------------------
// Definiciones de las variables globales del proyecto
// ------------------------------------------------------------------------------

// Variables para el sensor de ultrasonido 
const int periodoMuestreoUltrasonido = 100; // Periodo de muestreo para el sensor de proximidad en 100ms = 0,1s. (Recomendado: tm >= 50ms)
unsigned long contadorTiempoUltrasonido;  // Contador de tiempo para el muestreo del sensor

// Variables para el Buzzer
int periodoBuzzer = 0;  // Periodo de activación del Buzzer (Variable entre 0ms y 1000ms)
unsigned long contadorTiempoBuzzer; // Contador de tiempo para la activación del Buzzer
bool estadoBuzzer = false;

// Creación del objeto "sensorHC" para usar el sensor con la librería "EasyUltrasonic"
EasyUltrasonic sensorHC;

// Constantes de distancia
const int LIMITE_SUPERIOR_DISTANCIA = 20; // Distancia aceptable
const int LIMITE_INFERIOR_DISTANCIA = 5;  // Distancia con peligro de colisión

// ------------------------------------------------------------------------------
// Funciones del proyecto
// ------------------------------------------------------------------------------

void setup() {
  // Inicio del puerto serial
  Serial.begin(115200);

  // Inicio de contadores de tiempo con la función millis()
  contadorTiempoUltrasonido = millis();
  contadorTiempoBuzzer = millis();

  // Configuración de pines
  sensorHC.attach(pinTrigger, pinEcho); //  Inicializa el sensor HC-SR04 con el objeto creado

  //ledcAttach(pinBuzzer, frecuenciaPWMBuzzer, resolucionPWMBuzzer);  // Inicia el PWM para el Buzzer
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinRojo, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAzul, OUTPUT);
}

void loop() {
  // Si el tiempo transcurrido es mayor al periodo de muestreo, mide la distancia
  if(millis() > contadorTiempoUltrasonido + periodoMuestreoUltrasonido){ 
    contadorTiempoUltrasonido = millis(); // Actualiza el contador de tiempo
    float distanciaSensor = sensorHC.getDistanceCM(); //Leer el valor de distancia del sensor de ultrasonido
    comprobarProximidad(distanciaSensor);  // Comprobar la proximidad al objeto detectado
    escribirDistancia(distanciaSensor);   // Imprimir la distancia en el puerto serial
  }
  
  // Activa el buzzer tras trascurrido el periodo asignado según la distancia al obstaculo
  if((millis() > contadorTiempoBuzzer + periodoBuzzer) && periodoBuzzer != 0 && !estadoBuzzer){
    sonarBuzzer();    
  }else if((millis() > contadorTiempoBuzzer + periodoBuzzer + 50) && estadoBuzzer){
    contadorTiempoBuzzer = millis();
    apagarBuzzer();
  }
}

/*
  Función: escribirDistancia
  Descripción: Imprime en el puerto serial la distancia leída por el sensor de proximidad en cm  
*/
void escribirDistancia(float distancia){
  Serial.print("Distancia: ");
  Serial.print(distancia, 2); // Imprime la distancia con dos decimales.
  Serial.println("cm.");
}

/*
  Función: comprobarProximidad
  Descripción: Comprueba la cercanía del sensor al objeto con los límites establecidos.  
*/
void comprobarProximidad(float distancia){
  // Si la distancia es menor al límite de colisión, cambia el color del LED RGB a Rojo y el buzzer suena cada 200ms.
  if(distancia <= 5){
    periodoBuzzer = 200;
    cambiarColorRGB(255,0,0);
  // Si la distancia está entre los limites de colisión y la distancia segura, calcula y cambia el color de alerta y el periodo de buzzer.
  }else if(distancia > 5 && distancia <= 20) {
    periodoBuzzer = calcularPeriodoBuzzer(distancia);
    cambiarColorRGB(255, calcularIntensidadVerde(distancia), 0);    
  // Si la distancia es mayor al límite de distancia segura, cambia color del LED RGB a verde y apaga el buzzer.
  }else{
    periodoBuzzer = 0;
    cambiarColorRGB(0,255,0);    
  }
}

/*
  Función: calcularPeriodoBuzzer
  Descripción: Calcula cada cuánto tiempo debería sonar el buzzer si está entre los límites de distancia  
*/
int calcularPeriodoBuzzer(int distancia){
  return map(distancia, LIMITE_INFERIOR_DISTANCIA, LIMITE_SUPERIOR_DISTANCIA, 200, 1000);
}

/*
  Función: calcularIntensidadVerde
  Descripción: Calcula cuál será la intensidad del color verde en el LED RGB si está entre los límites de distancia  
*/
int calcularIntensidadVerde(int distancia){
  return map(distancia, LIMITE_INFERIOR_DISTANCIA, LIMITE_SUPERIOR_DISTANCIA, 0, 255);
}

/*
  Funciones: sonarBuzzer y apagarBuzzer
  Descripción: Enciende y apaga el buzzer tras un tiempo corto
*/
void sonarBuzzer(){
  estadoBuzzer = true;
  digitalWrite(pinBuzzer,estadoBuzzer);
}

void apagarBuzzer(){
  estadoBuzzer = false;
  digitalWrite(pinBuzzer,estadoBuzzer);
}

/*
  Función: cambiarColorRGB
  Descripción: Establece la intensidad de cada uno de los pines de conexión del led RGB
*/
void cambiarColorRGB(int rojo, int verde, int azul){
  analogWrite(pinRojo, rojo);
  analogWrite(pinVerde, verde);
  analogWrite(pinAzul, azul);
}