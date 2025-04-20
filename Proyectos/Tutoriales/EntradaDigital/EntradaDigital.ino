/* 
  Proyecto: Entrada Digital
  Descripción: Este proyecto permitirá identificar cuando un pulsador es 
  oprimido y hacer que el LED del ESP32 se encienda o apague con un único
   pulsador, conoceremos la configuración de los pines para esta tarea.
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 09/08/2024
  Última modificación: 09/08/2024
*/

#define LED_PIN 2  // Definimos el Pin al que está conectado el LED
#define PULSADOR_PIN 36 // Definimos el Pin al que estará conectado el pulsador

void setup(){
  pinMode(LED_PIN, OUTPUT);  // Se configura el pin del LED como salida
  // Se configura el pin del pulsador como entrada PULL-UP
  pinMode(PULSADOR_PIN, INPUT_PULLUP); 
}

void loop(){
  // Comprobamos si el pulsador fue oprimido
  if(digitalRead(PULSADOR_PIN) == false ){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Cambiamos el estado del LED
    delay(300); // Espera durante 300ms a modo de antirebote
  }
}