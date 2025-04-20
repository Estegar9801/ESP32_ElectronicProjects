/* 
  Proyecto: Salida Digital
  Descripción: Este proyecto nos introduce al mundo de la electrónica digital
  y la programación de microcontroladores. Encenderemos y apagaremos el LED de
  la tarjeta ESP32 durante unos segundos.
  Autor: Johah Hernández - Ingeniería Electrónica
  Fecha de creación: 09/08/2024
  Última modificación: 09/08/2024
*/

#define LED_PIN 2 // Definimos el Pin al que está conectado el LED

void setup(){
  pinMode(LED_PIN, OUTPUT);  // Se configura el pin del LED como salida
}

void loop(){
  digitalWrite(LED_PIN, HIGH);  // Se enciende el LED
  delay(1000);                  // Espera durante 1s = 1000ms
  digitalWrite(LED_PIN, LOW);   // Se apaga el LED
  delay(1000);                  // Espera durante 1s = 1000ms
}
