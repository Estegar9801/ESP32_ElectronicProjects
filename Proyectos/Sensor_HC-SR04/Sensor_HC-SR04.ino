/* 
  Proyecto: Sensor de proximidad HC-SR04
  Descripción: En este proyecto se hace uso del sensor de ultrasonido HC-SR04 para medir la distancia
  a un obstáculo y mostrarla en el monitor serial.  
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 09/02/2025
  Última modificación: 09/02/2025
*/

// --------------------------------------------------------------------------------
// Importación de librerías
// --------------------------------------------------------------------------------
#include <EasyUltrasonic.h> // Librería para usar el sensor de Ultrasonido HC-SR04

// --------------------------------------------------------------------------------
// Definición de pines
// --------------------------------------------------------------------------------
#define PIN_TRIGGER 5  // Pin del ESP32 conectado a la terminal Trigger del sensor HC-SR04
#define PIN_ECHO 18    // Pin del ESP32 conectado a la terminal Echo del sensor HC-SR04

// --------------------------------------------------------------------------------
// Variables globales
// --------------------------------------------------------------------------------
// Creación del objeto "sensorHC" para manejar el sensor
EasyUltrasonic sensorHC;  

// Define el intervalo de medición en milisegundos (Recomendado: >= 50ms)
const int INTERVALO_MEDICION = 500;

// --------------------------------------------------------------------------------
// Configuración inicial del ESP32
// --------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200); // Inicia la comunicación serie a 115200 baudios
  sensorHC.attach(PIN_TRIGGER, PIN_ECHO); // Configura los pines del sensor HC-SR04
}

// ------------------------------------------------------------------------------
// Bucle principal: mide e imprime la distancia periódicamente
// ------------------------------------------------------------------------------
void loop() {
  float distanciaMedida = sensorHC.getDistanceCM(); // Obtiene la distancia en centímetros
  imprimirDistancia(distanciaMedida);
  delay(INTERVALO_MEDICION); // Espera el tiempo definido antes de la siguiente medición
}

// ------------------------------------------------------------------------------
// Función para imprimir la distancia en el monitor serial
// ------------------------------------------------------------------------------
void imprimirDistancia(float distancia){
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
}