/* 
  Proyecto: Juego de dados en pantalla OLED
  Descripción: Este programa dibuja un dado en una pantalla OLED,
  mostrando números generados de forma pseudoaleatoria cuando se presiona un botón.
  Está pensado como ejercicio educativo para aprender a usar pantallas OLED, 
  trabajar con entradas digitales y manejar números aleatorios en Arduino.
  
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 26/04/2024
  Última modificación: 26/04/2025
*/

#include <Wire.h>                // Librería para comunicación I2C
#include <Adafruit_GFX.h>         // Librería de gráficos para pantallas
#include <Adafruit_SSD1306.h>     // Librería específica para pantallas OLED SSD1306

// Dimensiones de la pantalla OLED
#define ANCHO_PANTALLA 128        // Número de píxeles horizontales de la pantalla
#define ALTO_PANTALLA 32          // Número de píxeles verticales de la pantalla

// Configuración de la pantalla OLED
#define OLED_RESET -1             // Pin de reinicio (no se usa en este módulo)
#define DIR_PANTALLA 0x3C         // Dirección I2C de la pantalla OLED

// Se crea una instancia (objeto) de la pantalla OLED
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, OLED_RESET);

// Definición del pin al que está conectado el botón
#define PIN_BOTON 25

// Variables para manejar el "rebote" del botón
const int TIEMPO_REBOTE = 300;    // Tiempo mínimo (en milisegundos) para considerar una nueva pulsación
long contadorRebote = 0;          // Variable para llevar el control del tiempo de rebote

// Función que se ejecuta una sola vez al encender el sistema
void setup() {
  // Inicializamos la pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, DIR_PANTALLA)) {    
    for (;;); // Si falla la inicialización, quedamos atrapados en un bucle infinito
  }

  display.display(); // Muestra inicialmente el contenido actual (vacío)
  
  // Configuramos el pin del botón como entrada, activando la resistencia pull-up interna
  pinMode(PIN_BOTON, INPUT_PULLUP); 
  
  delay(500);  // Esperamos medio segundo para asegurar estabilidad en la comunicación I2C

  // Iniciamos el contador de tiempo para controlar el rebote
  contadorRebote = millis();
}

// Función principal que se ejecuta repetidamente
void loop() {
  // Detectamos si el botón ha sido presionado y ha pasado suficiente tiempo para evitar rebotes
  if (!digitalRead(PIN_BOTON) && (millis() > (contadorRebote + TIEMPO_REBOTE))) {
    contadorRebote = millis();      // Reiniciamos el contador de rebote
    generarSemilla();               // Generamos una nueva semilla para números aleatorios
    display.clearDisplay();         // Limpiamos la pantalla antes de dibujar algo nuevo

    int numeroAleatorio = generarNumeroAleatorio(); // Generamos un número aleatorio entre 1 y 6    

    dibujarDado(numeroAleatorio, 2, 2); // Dibujamos el dado en pantalla en la posición (2,2)    
  }
}

// Función para generar una semilla pseudoaleatoria
// Utilizamos el tiempo actual (millis) como base para obtener resultados menos predecibles
void generarSemilla() {
  randomSeed(millis());
}

// Función que devuelve un número aleatorio entre 1 y 6
// Esto simula la tirada de un dado real
int generarNumeroAleatorio() {
  return random(1, 7); // random(a, b) genera números desde 'a' hasta 'b-1'
}

// Función para dibujar un dado en la pantalla OLED
// Recibe como parámetros: el número a mostrar y las coordenadas (x, y) de inicio
void dibujarDado(int valor, int x, int y) {  
  display.drawRect(x, y, 30, 30, SSD1306_WHITE); // Dibujamos el contorno del dado (cuadrado)

  // Dependiendo del número del dado, dibujamos los puntos correspondientes
  if (valor == 1) {
    dibujarPuntoDado(15, 15, x, y); // Punto central
  } 
  else if (valor == 2) {
    dibujarPuntoDado(8, 8, x, y);   // Arriba a la izquierda
    dibujarPuntoDado(22, 22, x, y); // Abajo a la derecha
  } 
  else if (valor == 3) {
    dibujarPuntoDado(15, 15, x, y); // Centro
    dibujarPuntoDado(8, 8, x, y);   // Arriba a la izquierda
    dibujarPuntoDado(22, 22, x, y); // Abajo a la derecha
  } 
  else if (valor == 4) {
    dibujarPuntoDado(8, 8, x, y);   // Arriba a la izquierda
    dibujarPuntoDado(22, 8, x, y);  // Arriba a la derecha
    dibujarPuntoDado(8, 22, x, y);  // Abajo a la izquierda
    dibujarPuntoDado(22, 22, x, y); // Abajo a la derecha
  } 
  else if (valor == 5) {
    dibujarPuntoDado(8, 8, x, y);
    dibujarPuntoDado(22, 8, x, y);
    dibujarPuntoDado(15, 15, x, y); // Centro
    dibujarPuntoDado(8, 22, x, y);
    dibujarPuntoDado(22, 22, x, y);
  } 
  else if (valor == 6) {
    dibujarPuntoDado(8, 8, x, y);
    dibujarPuntoDado(22, 8, x, y);
    dibujarPuntoDado(8, 15, x, y);  // Centro izquierda
    dibujarPuntoDado(22, 15, x, y); // Centro derecha
    dibujarPuntoDado(8, 22, x, y);
    dibujarPuntoDado(22, 22, x, y);
  }

  display.display(); // Refrescamos la pantalla para mostrar el dibujo
}

// Función auxiliar para dibujar un círculo relleno (punto del dado)
// Los parámetros x e y son relativos dentro del dado, y x0, y0 son el desplazamiento general
void dibujarPuntoDado(int x, int y, int x0, int y0) {
  display.fillCircle(x + x0, y + y0, 2, SSD1306_WHITE); // Dibuja un círculo de radio 2 píxeles
}