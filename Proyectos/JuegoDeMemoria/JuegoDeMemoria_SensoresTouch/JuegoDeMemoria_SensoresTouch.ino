/* 
  Proyecto: Juego de memoria - Pulsadores
  Descripción: En este proyecto las teclas se iluminan aleatoriamente y el jugador debe reproducir la secuencia de luz
  sin cometer un error. Usamos un LED RGB para mostrar los colores, un buzzer para tener un sonido, se utilizan los 
  sensores capacitivos del ESP32 como elementos accionables para botones de color, adicional a un botón de inicio (Pulsador). 
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 06/08/2024
  Última modificación: 06/08/2024
*/

// ------------------------------------------------------------------------------
// Definición de pines
// ------------------------------------------------------------------------------
#define BOTON_INICIO 25
#define PIN_ROJO 15
#define PIN_VERDE 2
#define PIN_AZUL 0
#define PIN_BUZZER 13

// ------------------------------------------------------------------------------
// Declaración de variables
// ------------------------------------------------------------------------------
enum EstadoJuego { ESPERA_INICIO, MOSTRANDO_SECUENCIA, ESPERANDO_JUGADOR }; // Se declaran los estados del juego

EstadoJuego estadoJuego = ESPERA_INICIO; // Posibles estados del juego
const uint8_t NUM_NIVELES = 15; // Número máximo de niveles del juego
uint8_t contadorSecuenciaActual = 0;  // Conteo del turno del jugador
uint8_t nivelActual = 0;  // Nivel actual del juego

uint8_t secuenciaAleatoria[NUM_NIVELES];  // Guarda la secuencia aleatoria
uint8_t secuenciaJugador[NUM_NIVELES];  // Guarda la secuencia ingresada por el jugador

uint16_t codigosColores[5][4] = { // Almacena los códigos de color RGB y la frecuencia para cada tono del buzzer
    {0, 0, 0, 0},
    {255, 0, 0, 250},
    {0, 255, 0, 300},
    {0, 0, 255, 350},
    {255, 255, 0, 400}
};

const uint8_t LIMITE_TOUCH = 40;  // Mide el valor de activación de los sensores capacitivos
uint8_t touchDetectado = 0; // Guarda el sensor capacitido oprimido

const int TIEMPO_REBOTE = 300;  // Tiempo de antirobote
unsigned long contadorRebote = 0; // Contador del tiempo de antirebote

uint16_t duracionPulsoBuzzer = 100; // Duración del tono del buzzer
uint16_t duracionPulsoLED = 300;  // Duración del encendido del led

uint8_t mejorPuntaje = 0; // Mejor puntaje

// ------------------------------------------------------------------------------
// Funciones
// ------------------------------------------------------------------------------
void compararTurnoJugador() {
  sonarBuzzer(touchDetectado);
  cambiarColorRGB(touchDetectado);
  cambiarColorRGB(0);

  if (touchDetectado == secuenciaAleatoria[contadorSecuenciaActual]) {
    contadorSecuenciaActual++;
    if (contadorSecuenciaActual > nivelActual) {
      contadorSecuenciaActual = 0;
      nivelActual++;
      estadoJuego = MOSTRANDO_SECUENCIA;
    }
  } else {
    Serial.println("Has perdido.");
    compararMejorPuntaje();
    Serial.println("Pulsa el botón de Inicio para jugar nuevamente.");
    estadoJuego = ESPERA_INICIO;
  }
}

void comprobarInterrupcion(uint8_t touch) {
  if (estadoJuego == ESPERANDO_JUGADOR && millis() - contadorRebote > TIEMPO_REBOTE) {
    contadorRebote = millis();
    touchDetectado = touch;
  }
}
/*
  Funciones: interrupcionesColor
  Descripción: Ejecuta la rutina de interrupción para los sensores capacitivos, comprobando si se oprimió mientras es el turno del jugador
  y si se cumplió el tiempo de rebote, se envia a la función de comparación el código del color correspondiente.
*/
void interrupcionRojo() { comprobarInterrupcion(1); }
void interrupcionVerde() { comprobarInterrupcion(2); }
void interrupcionAzul() { comprobarInterrupcion(3); }
void interrupcionAmarillo() { comprobarInterrupcion(4); }

void setup() {
  Serial.begin(115200);
  
  // Configuración de pin de entrada del botón de inicio
  pinMode(BOTON_INICIO, INPUT_PULLUP);

  // Configuración de las interrupciones de los sensores capacitivos
  touchAttachInterrupt(T5, interrupcionVerde, LIMITE_TOUCH);
  touchAttachInterrupt(T6, interrupcionRojo, LIMITE_TOUCH);
  touchAttachInterrupt(T8, interrupcionAmarillo, LIMITE_TOUCH);
  touchAttachInterrupt(T9, interrupcionAzul, LIMITE_TOUCH);

  // Configuración de los pines de salida
  ledcAttach(PIN_BUZZER, 2000, 16);
  ledcAttach(PIN_ROJO, 10000, 8);
  ledcAttach(PIN_VERDE, 10000, 8);
  ledcAttach(PIN_AZUL, 10000, 8);

  // Reinicio de las variables del juego
  reiniciarVariables();
}

void loop() {
  // Si se detecta que se oprimió el botón de inicio, empieza el juego
  if (!digitalRead(BOTON_INICIO) && millis() - contadorRebote > TIEMPO_REBOTE && estadoJuego == ESPERA_INICIO) {
    contadorRebote = millis();
    reiniciarVariables();
    estadoJuego = MOSTRANDO_SECUENCIA;
  }
  // Si aún no se ha alcanzado el nivel máximo del juego, se ejecuta el juego
  if (nivelActual < NUM_NIVELES) {
    // Si el estado del juego es MOSTRANDO_SECUENCIA, se muestra la secuencia
    if (estadoJuego == MOSTRANDO_SECUENCIA) {
      mostrarSecuencia();
    // Si es turno del jugador y se oprimió un botón táctil, se compara con la secuencia aleatoria
    } else if (estadoJuego == ESPERANDO_JUGADOR && touchDetectado != 0) {
      compararTurnoJugador();
      touchDetectado = 0;
    }
  // Si se alcanzó el número máximo de niveles, se indica que se completó el juego
  } else if (nivelActual >= NUM_NIVELES && estadoJuego != ESPERA_INICIO) {
    Serial.println("Felicidades has completado todos los niveles.");
    compararMejorPuntaje();
    Serial.println("Pulsa el botón de Inicio para jugar nuevamente.");
    estadoJuego = ESPERA_INICIO;
  }
}
/*
  Función: reiniciarVariables
  Descripción: Establece todos los valores del juego en los valores iniciales y genera una nueva secuencia de colores
*/
void reiniciarVariables() {
  estadoJuego = ESPERA_INICIO;
  nivelActual = 0;
  contadorSecuenciaActual = 0;
  generarSemilla();
  generarSecuencia();
}
/*
  Función: generarSemilla
  Descripción: Inicia la semilla de números pseudo-aleatorios enviando como argumento la función millis()
*/
void generarSemilla() {
  randomSeed(millis());
}
/*
  Función: generarNumeroAleatorio
  Descripción: Retorna un número pseudo-aleatorio entre 1 y 4.
*/
int generarNumeroAleatorio() {
  return random(1, 5);
}
/*
  Función: generarSecuencia
  Descripción: Llena el arreglo con una secuencia aleatoria de números entre 1 y 4, correspondientes al código de colores.
*/
void generarSecuencia() {
  for (int i = 0; i < NUM_NIVELES; i++) {
    secuenciaAleatoria[i] = generarNumeroAleatorio();
  }
}
/*
  Función: mostrarSecuencia
  Descripción: Muestra la secuencia de colores para que el jugador la observe antes de iniciar su turno
*/
void mostrarSecuencia() {
  delay(500);
  Serial.println("Mostrando secuencia.");
  for (int i = 0; i <= nivelActual; i++) {
    sonarBuzzer(secuenciaAleatoria[i]);
    cambiarColorRGB(secuenciaAleatoria[i]);
    cambiarColorRGB(0);
  }
  estadoJuego = ESPERANDO_JUGADOR;
  Serial.println("Ahora es tu turno.");
}
/*
  Función: compararMejorPuntaje
  Descripción: Comprueba si el puntaje actual es mejor que el último mejor puntaje registrado, si es así lo guarda
*/
void compararMejorPuntaje() {
  if (nivelActual > mejorPuntaje) {
    mejorPuntaje = nivelActual;
    Serial.print("Has superado el récord. Tu puntaje: ");
    Serial.println(mejorPuntaje);
  }
}
/*
  Función: sonarBuzzer
  Descripción: Hace sonar el buzzer con un tono personalizado para cada botón durante un tiempo determinado
*/
void sonarBuzzer(uint8_t i) {
  tone(PIN_BUZZER, codigosColores[i][3], duracionPulsoBuzzer);
}
/*
  Función: cambiarColorRGB
  Descripción: Establece la intensidad de cada uno de los pines de conexión del led RGB (Pueden tomar valores entre 0 - 255)
*/
void cambiarColorRGB(int numeroColor) {
  ledcWrite(PIN_ROJO, codigosColores[numeroColor][0]);
  ledcWrite(PIN_VERDE, codigosColores[numeroColor][1]);
  ledcWrite(PIN_AZUL, codigosColores[numeroColor][2]);
  delay(duracionPulsoLED);
}