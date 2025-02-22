/* 
  Proyecto: Sensor de proximidad HC-SR04
  Descripción: En este proyecto se hace uso del sensor de ultrasonido HC-SR04 para 
  medir la distancia a un obstáculo y mostrarla en el monitor serial, además se agrega
  un LED RGB como indicador visual en determinado rango de distancia.  
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 17/02/2025
  Última modificación: 17/02/2025
*/
// --------------------------------------------------------------------------------
// Importación de librerías
// --------------------------------------------------------------------------------
#include <EasyUltrasonic.h> // Librería para usar el sensor de Ultrasonido HC-SR04
// --------------------------------------------------------------------------------
// Definición de pines
// --------------------------------------------------------------------------------
#define PIN_TRIGGER 5   // Pin del ESP32 conectado a la terminal Trigger del sensor HC-SR04
#define PIN_ECHO 18     // Pin del ESP32 conectado a la terminal Echo del sensor HC-SR04
#define PIN_ROJO 15     // Pin conectado al canal rojo del LED RGB
#define PIN_VERDE 2     // Pin conectado al canal verde del LED RGB
#define PIN_AZUL 0      // Pin conectado al canal azul del LED RGB
// --------------------------------------------------------------------------------
// Variables globales
// --------------------------------------------------------------------------------
// Creación del objeto "sensorHC" para manejar el sensor
EasyUltrasonic sensorHC;  
// Define el intervalo de medición en milisegundos (Recomendado: >= 50ms)
const uint16_t INTERVALO_MEDICION = 500;
// Captura el tiempo transcurrido entre una medición y la siguiente
unsigned long tiempoActualMedicion;
// Define los limites de las alertas para las distancias
const uint8_t LIMITE_INFERIOR = 5;
const uint8_t LIMITE_SUPERIOR = 15;

// --------------------------------------------------------------------------------
// Configuración inicial del ESP32
// --------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200); // Inicia la comunicación serie a 115200 baudios
  sensorHC.attach(PIN_TRIGGER, PIN_ECHO); // Configura los pines del sensor HC-SR04

  // Configuración de los pines como salidas PWM (Pulse Width Modulation)
  // ledcAttachPin(pin, frecuencia, resolución) configura un canal PWM
  // La configuración del canal se realiza automáticamente
  ledcAttach(PIN_ROJO,  12000, 8);  // Configuración para el rojo, 12 kHz, 8 bits de resolución
  ledcAttach(PIN_VERDE, 12000, 8);  
  ledcAttach(PIN_AZUL,  12000, 8);

  // Captura el tiempo transcurrido desde que se conectó el ESP32 en milisegundos
  tiempoActualMedicion = millis();
}
// ------------------------------------------------------------------------------
// Bucle principal: mide e imprime la distancia periódicamente
// ------------------------------------------------------------------------------
void loop() {
  if ( millis() > ( tiempoActualMedicion + INTERVALO_MEDICION ) ) {
    tiempoActualMedicion = millis();
    float distanciaMedida = sensorHC.getDistanceCM(); // Obtiene la distancia en centímetros
    imprimirDistancia(distanciaMedida);
    comprobarDistancia(distanciaMedida);
  }    
}
// ------------------------------------------------------------------------------
// Función para imprimir la distancia en el monitor serial
// ------------------------------------------------------------------------------
void imprimirDistancia(float distancia){
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
}
// ------------------------------------------------------------------------------
// Función que comprueba que tan cerca está el sensor de un Obstáculo
// ------------------------------------------------------------------------------
void comprobarDistancia(float distancia){
  if(distancia <= LIMITE_INFERIOR){
    cambiarColorRGB(255,0,0);
  }else if( (LIMITE_INFERIOR < distancia) && (distancia <= LIMITE_SUPERIOR) ){
    cambiarColorRGB(255, calcularIntesidadVerde(distancia), 0);
  }else{
    cambiarColorRGB(0, 255, 0);
  }
}
// ------------------------------------------------------------------------------
// Función que calcula la intensidad de la componente del verde 
// ------------------------------------------------------------------------------
int calcularIntesidadVerde(float distancia){
  return map(distancia, LIMITE_INFERIOR, LIMITE_SUPERIOR, 0, 255);
}
// ------------------------------------------------------------------------------
// Función para cambiar el color del LED RGB
// ------------------------------------------------------------------------------
void cambiarColorRGB(uint8_t R, uint8_t G, uint8_t B) {
  // Ajusta la intensidad de cada color usando PWM
  ledcWrite(PIN_ROJO,  R);   // Controla el canal rojo
  ledcWrite(PIN_VERDE, G);   // Controla el canal verde
  ledcWrite(PIN_AZUL,  B);   // Controla el canal azul  
}