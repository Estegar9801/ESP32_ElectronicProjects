/* 
  Proyecto: Generando números aleatorios
  Descripción: En este script te mostramos una forma de generar
  números pseudo-aleatorios que te podrá servir para tus proyectos.
  Generando uno cada que se oprima un botón y mostrar el valor en una
  pantalla OLED.
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 26/04/2024
  Última modificación: 26/04/2025
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ANCHO_PANTALLA 128  // Número de píxeles horizontales de la pantalla OLED
#define ALTO_PANTALLA 32    // Número de píxeles verticales de la pantalla OLED

#define OLED_RESET -1       // Pin de reinicio de la pantalla (no se usa en este modelo)
#define DIR_PANTALLA 0x3C   // Dirección I2C de la pantalla OLED (0x3C para 128x32px, 0x3D para 128x64px)

// Se crea una instancia del objeto de la pantalla OLED
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, OLED_RESET);

// Define el pin dónde esta conectado el botón
#define PIN_BOTON 25

// Variables para controlar el rebote del botón
const int TIEMPO_REBOTE = 300;
long contadorRebote = 0;

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, DIR_PANTALLA)) {    
    for (;;); // Bucle infinito en caso de error en la inicialización
  }

  display.display(); // Muestra el contenido en la pantalla (inicialmente vacío)
  
  pinMode(PIN_BOTON, INPUT_PULLUP); // Definimos el pin como entrada pull-up
  delay(500);  // Retraso de 500s para estabilidad

  contadorRebote = millis();  // Inicia el contador del rebote
}

void loop() {
  // Si se oprime el botón y ya  no se ha completado el tiempo de rebote
  if ( !digitalRead(PIN_BOTON)  && (millis() > (contadorRebote + TIEMPO_REBOTE))){
    contadorRebote = millis();  // Reinicia el contador del rebote
    generarSemilla();  // Genera una semilla
    int numeroAleatorio = generarNumeroAleatorio(); // Recibe el número aleatorio

    // Se envía a la función imprimir mensaje para mostrarlo en la pantalla OLED
    imprimirMensaje(numeroAleatorio);
  }
}

// Genera una semilla
void generarSemilla(){
  randomSeed(millis());
}

// Genera un número pseudo-aleatorio entre 1 y 6
int generarNumeroAleatorio () {
  return random(1,7);
}

void imprimirMensaje(int valor) {
  display.clearDisplay(); // Borra la pantalla antes de dibujar el nuevo contenido

  display.setTextSize(2);              // Establece el tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Define el color del texto como blanco
  display.setCursor(0, 0);             // Establece la posición de inicio del texto
  display.print("Num: ");
  display.println(valor);    // Escribe el mensaje en la pantalla

  display.display(); // Actualiza la pantalla con el nuevo contenido
}
