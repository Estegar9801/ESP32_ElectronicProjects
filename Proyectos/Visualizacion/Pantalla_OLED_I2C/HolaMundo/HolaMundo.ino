/*  
  Proyecto: Texto en Pantalla OLED  
  Descripción: En este proyecto se muestra una introducción a las pantalla OLED utilizando la biblioteca Adafruit SSD1306. La usaremos para mostrar el mensaje Hola Mundo.
  Autor: Johan Darío Hernández - Ingeniería Electrónica  
  Fecha de creación: 15/03/2025  
  Última modificación: 15/03/2025  
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ANCHO_PANTALLA 128  // Número de píxeles horizontales de la pantalla OLED
#define ALTO_PANTALLA 32    // Número de píxeles verticales de la pantalla OLED

#define OLED_RESET -1       // Pin de reinicio de la pantalla (no se usa en este modelo)
#define DIR_PANTALLA  0x3C  // Dirección I2C de la pantalla OLED (0x3C para 128x32px, 0x3D para 128x64px)

// Se crea una instancia del objeto de la pantalla OLED
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, OLED_RESET);

void setup() {
  // Inicializa la pantalla OLED y verifica si la comunicación es exitosa
  if (!display.begin(SSD1306_SWITCHCAPVCC, DIR_PANTALLA)) {    
    for (;;); // Bucle infinito en caso de error en la inicialización
  }

  display.display(); // Muestra el contenido en la pantalla (inicialmente vacío)
  delay(2000);      // Espera 2 segundos antes de continuar

  imprimirMensaje(); // Llama a la función para mostrar un mensaje en la pantalla
}

void loop() {
  // Código principal (actualmente vacío)
}

// Función para imprimir un mensaje en la pantalla OLED
void imprimirMensaje() {
  display.clearDisplay(); // Borra la pantalla antes de dibujar el nuevo contenido

  display.setTextSize(2);              // Establece el tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Define el color del texto como blanco
  display.setCursor(0, 0);             // Establece la posición de inicio del texto
  display.println(F("Hola mundo"));    // Escribe el mensaje en la pantalla

  display.display(); // Actualiza la pantalla con el nuevo contenido
}
