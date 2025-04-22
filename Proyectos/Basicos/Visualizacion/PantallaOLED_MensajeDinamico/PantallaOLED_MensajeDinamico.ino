/*  
  Proyecto: Texto en Pantalla OLED  
  Descripción: En este proyecto se muestra como mostrar mensajes de manera
  dinámica en la pantalla OLED. El mensaje se actualizará con cada cadena de
  texto que escribamos en el puerto serial.
  Autor: David Esteban Garnica - Ingeniería Electrónica  
  Fecha de creación: 21/04/2025  
  Última modificación: 21/04/2025  
*/

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

  Serial.begin(115200); // Inicia el puerto serial a 115200 baudios
  Serial.println("Envía el primer mensaje a la pantalla."); // Imprime un mensaje

  // Inicializa la pantalla OLED y verifica si la comunicación es exitosa
  if (!display.begin(SSD1306_SWITCHCAPVCC, DIR_PANTALLA)) {    
    for (;;); // Bucle infinito en caso de error en la inicialización
  }

  display.display(); // Muestra el contenido en la pantalla (inicialmente vacío)
  delay(2000);      // Espera 2 segundos antes de continuar  
}

void loop() {
  if (Serial.available() > 0){  // Si se recibe un mensaje por el puerto serial
    String mensaje = Serial.readString(); // Guarda el mensaje 
    imprimirMensaje(mensaje); // Imprime el mensaje en la pantalla OLED
  }
}

// Función para imprimir el mensaje recibido en la pantalla OLED
void imprimirMensaje(String mensaje) {


  display.clearDisplay(); // Borra la pantalla antes de dibujar el nuevo contenido

  display.setTextSize(2);              // Establece el tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Define el color del texto como blanco
  display.setCursor(0, 0);             // Establece la posición de inicio del texto
  display.println(mensaje);    // Escribe el mensaje en la pantalla

  display.display(); // Actualiza la pantalla con el nuevo contenido
}
