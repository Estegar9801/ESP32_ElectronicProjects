/*  
  Proyecto: Encendiendo el LED RGB con ESP32
  Descripción: Este proyecto muestra cómo controlar un LED RGB usando un ESP32. Se generarán los colores blanco,
  rojo, verde y azul encendiendo los respectivos pines del LED RGB.

  Autor: David Esteban Garnica - Ingeniería Electrónica  
  Fecha de creación: 08/02/2025  
  Última modificación: 08/02/2025  
*/

// ------------------------------------------------------------------------------
// Definición de pines
// ------------------------------------------------------------------------------
#define PIN_ROJO 15   // Pin conectado al canal rojo del LED RGB
#define PIN_VERDE 2   // Pin conectado al canal verde del LED RGB
#define PIN_AZUL 0    // Pin conectado al canal azul del LED RGB

// ------------------------------------------------------------------------------
// Configuración inicial del ESP32
// ------------------------------------------------------------------------------
void setup() {  
  delay(10);  // Breve pausa para estabilidad

  // Configuración de los pines como salidas PWM (Pulse Width Modulation)
  // ledcAttachPin(pin, frecuencia, resolución) configura un canal PWM
  // La configuración del canal se realiza automáticamente
  ledcAttach(PIN_ROJO,  12000, 8);  // Configuración para el rojo, 12 kHz, 8 bits de resolución
  ledcAttach(PIN_VERDE, 12000, 8);  
  ledcAttach(PIN_AZUL,  12000, 8);
}

// ------------------------------------------------------------------------------
// Bucle principal: alterna los colores del LED RGB
// ------------------------------------------------------------------------------
void loop() {  
  cambiarColorRGB(255, 255, 255); // Blanco
  cambiarColorRGB(255, 0, 0);     // Rojo
  cambiarColorRGB(0, 255, 0);     // Verde
  cambiarColorRGB(0, 0, 255);     // Azul
  cambiarColorRGB(0, 0, 0);       // Apagar LED
}

// ------------------------------------------------------------------------------
// Función para cambiar el color del LED RGB
// ------------------------------------------------------------------------------
void cambiarColorRGB(uint8_t R, uint8_t G, uint8_t B) {
  // Ajusta la intensidad de cada color usando PWM
  ledcWrite(PIN_ROJO,  R);   // Controla el canal rojo
  ledcWrite(PIN_VERDE, G);   // Controla el canal verde
  ledcWrite(PIN_AZUL,  B);   // Controla el canal azul
  delay(1000);  // Espera 1 segundo antes de cambiar el color
}