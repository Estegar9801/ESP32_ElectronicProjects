/* 
  Proyecto: Juego de memoria - Pulsadores
  Descripción: En este proyecto las teclas se iluminan aleatoriamente y 
  el jugador debe reproducir la secuencia de luz sin cometer un error. Usamos 
  un LED RGB para mostrar los colores, un buzzer para tener un sonido para 
  cada tecla y pulsadores como elementos accionables.
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 04/08/2024
  Última modificación: 26/04/2025
*/

// ------------------------------------------------------------------------------
// Definición de pines
// ------------------------------------------------------------------------------
#define botonRojo 21  // Pin al que está conectado el botón Rojo
#define botonVerde 22 // Pin al que está conectado el botón Verde
#define botonAzul 25 // Pin al que está conectado el botón Azul
#define botonAmarillo 18  // Pin al que está conectado el botón Amarillo

#define pinRojo 15  // Pin al que está conectado la terminal R del led RGB 
#define pinVerde 2  // Pin al que está conectado la terminal G del led RGB 
#define pinAzul 0   // Pin al que está conectado la terminal B del led RGB 

#define pinBuzzer 13 // Pin al que está conectado el buzzer

// ------------------------------------------------------------------------------
// Declaración de variables
// ------------------------------------------------------------------------------
// Estados del juego: 
// 0 -> En espera del inicio
// 1 -> Mostrando secuencia
// 2 -> Esperando secuencia del jugador
uint8_t estadoJuego;

const uint8_t NUM_NIVELES = 10;  // Define el número máximo de niveles del juego
uint8_t contadorSecuenciaActual = 0;  // Lleva el conteo de las acciones en el turno del jugador
uint8_t nivelActual;  // El nivel actual del juego

uint8_t secuenciaAleatoria[NUM_NIVELES];  // Almacena la secuencia aleatoria de colores
uint8_t secuenciaJugador[NUM_NIVELES];  // Almacena temporalmente la secuencia en el turno del jugador

// Guarda los códigos de colores segun la posición, la intensidad para cada color del led RGB y la frecuencia del tono predefinida para que suena con el buzzer:
// j: Intensidad o frecuencia
// [0][j]: Apagar | [1][j]: Rojo | [2][j]: Verde | [3][j]: Azul | [4][j]: Amarillo
// c: codigo del color
// [c][0]: Intensidad de rojo | [c][1]: Intensidad de verde | [c][2]: Intensidad de azul | [c][3]: Frecuencia del tono
uint16_t codigosColores[5][4] = {{0,0,0,0}, {255,0,0,250}, {0,255,0,300}, {0,0,255,350}, {255,255,0,400}};

const int TIEMPO_REBOTE = 300; // Tiempo de rebote para todos los pulsadores. 300ms = 0,3s
long contadorRebote;  // Contador de tiempo para el rebote para los pulsadores
bool rebotePulsadores;  // Estado del pulsador

uint16_t frecuenciaPWMBuzzer = 2000;  // Frecuencia del PWM del Buzzer
uint16_t resolucionPWMBuzzer = 16;  // Resolución del PWM del Buzzer
uint16_t frecuenciaPWMLED = 2000; // Frecuencia del PWM de los led RGB
uint8_t resolucionPWMLED = 8; // Resolución del PWM de los led RGB

uint16_t duracionPulsoBuzzer; // Tiempo que tarda encendido el buzzer
uint16_t duracionPulsoLED;  // Tiempo que tarda encendido el led RGB

uint8_t mejorPuntaje; // Guarda el mejor puntaje

void setup() {
  // Inicio del puerto serial
  Serial.begin(115200);

  // Definición de valores para las variables
  estadoJuego = 0; // Cambia el estado del juego a: En espero de inicio
  contadorRebote = millis();  // Inicia el conteo de tiempo de rebote
  duracionPulsoBuzzer = 100;  // Tiempo que dura el buzzer encendido 100ms = 0,1s
  duracionPulsoLED = 300; // Tiempo que dura el led RGB encendido 300ms = 0,3s
  mejorPuntaje = 0; // Almacena el mejor puntaje

  // Configuración de pines de entrada (5 botones)  
  pinMode(botonRojo, INPUT_PULLUP);
  pinMode(botonAzul, INPUT_PULLUP);
  pinMode(botonVerde, INPUT_PULLUP);
  pinMode(botonAmarillo, INPUT_PULLUP);

  // Configuración de pines de salida (3 Led RGB - 1 Buzzer)
  ledcAttach(pinBuzzer, frecuenciaPWMBuzzer, resolucionPWMBuzzer);
  ledcAttach(pinRojo, frecuenciaPWMLED, resolucionPWMLED);
  ledcAttach(pinVerde, frecuenciaPWMLED, resolucionPWMLED);
  ledcAttach(pinAzul, frecuenciaPWMLED, resolucionPWMLED);

  delay(500);
  Serial.println("Oprime cualquier botón para iniciar.");
}

void loop() {
  int botonOprimido = 0;

  // Si se detecta que un boton se oprimió, se identifica cual fue.
  if(detectarBotonPresionado() && millis() > contadorRebote + TIEMPO_REBOTE){
    contadorRebote = millis();
    botonOprimido = identificarBoton();    
  }

  // Si el juego no ha iniciado y se oprimió un botón, se reinician las variables e inicia el juego.
  if(estadoJuego == 0 && botonOprimido != 0){
    reiniciarVariables();
    botonOprimido = 0;
    estadoJuego = 1;    
  }
  
  // Si aún no se han completado todos los niveles, inicia el juego
  if(nivelActual < NUM_NIVELES){ 
    // Muestra la secuencia si el estado del juego lo indica
    if(estadoJuego == 1){            
      mostrarSecuencia();
    // Si es turno del jugador, espera a que se oprima el boton
    }else if(estadoJuego == 2){
      // Si se oprimió uno de los botones de color y aún no se ha completado el turno
      if(botonOprimido != 0 && botonOprimido != 5 && (contadorSecuenciaActual <= nivelActual)){
        sonarBuzzer(botonOprimido); // Enciende el buzzer
        cambiarColorRGB(botonOprimido); // Enciende el led RGB con el color del botón oprimido
        cambiarColorRGB(0); // Apaga el led RGB
        // Si el botón que se oprimió es correcto, se aumenta el contador del turno actual
        if(botonOprimido == secuenciaAleatoria[contadorSecuenciaActual]){          
          botonOprimido = 0;
          contadorSecuenciaActual += 1;
        // Si el jugador oprimió el botón incorrecto, se da por terminado el juego
        }else{
          Serial.println("Has perdido.\n"); // Se le indica al jugador que ha perdido.
          compararMejorPuntaje(); // Se comparan los puntajes
          Serial.println("Pulsa cualquier botón para jugar nuevamente.\n");
          estadoJuego = 0;  // Cambia el estado del juego a: En espero de inicio
        }
      // Si ya se completó el turno del jugador, se aumenta el nivel.
      }else if(contadorSecuenciaActual > nivelActual){              
        contadorSecuenciaActual = 0;  // Reinicia el contador del turno actual
        nivelActual += 1; // Aumenta el nivel
        Serial.print("Secuencia Correcta. Nivel ");
        Serial.println(nivelActual);
        estadoJuego = 1; // Cambia el estado del juego a: Mostrar secuencia
      }
    }
  // Si el juego esta en progreso y ya se superó el número de Niveles, se indica que ya se completó el juego
  }else if(nivelActual >= NUM_NIVELES && estadoJuego != 0){        
    Serial.println("Felicidades has completado todos los niveles.");    
    compararMejorPuntaje();
    Serial.println("Pulsa el botón de Inicio para jugar nuevamente.");
    estadoJuego = 0;  // Cambia el estado del juego a: En espero de inicio
  }
}
/*
  Función: reiniciarVariables
  Descripción: Establece todos los valores del juego en los valores iniciales y
  genera una nueva secuencia de colores
*/
void reiniciarVariables(){  
  estadoJuego = 0;
  nivelActual = 0;
  contadorSecuenciaActual = 0;
  generarSemilla();
  generarSecuencia();
}
/*
  Función: generarSemilla
  Descripción: Inicia la semilla de números pseudo-aleatorios enviando como argumento la función millis()
*/
void generarSemilla(){
  randomSeed(millis());
}
/*
  Función: generarNumeroAleatorio
  Descripción: Retorna un número pseudo-aleatorio entre 1 y 4.
*/
int generarNumeroAleatorio(){
  return random(1,5);
}
/*
  Función: generarSecuencia
  Descripción: Llena el arreglo con una secuencia aleatoria de números entre 1 y 4, correspondientes al código de colores.
*/
void generarSecuencia(){
  for (int i = 0; i < NUM_NIVELES; i++){
    secuenciaAleatoria[i] = generarNumeroAleatorio();    
  }
}
/*
  Función: mostrarSecuencia
  Descripción: Muestra la secuencia de colores para que el jugador la observe antes de iniciar su turno
*/
void mostrarSecuencia(){  
  delay(500); // Espera 500 ms antes de mostrar la secuencia
  Serial.println("Mostrando secuencia.");
  for (int i=0; i <= nivelActual; i++){
    sonarBuzzer(secuenciaAleatoria[i]); // Enciende el buzzer
    cambiarColorRGB(secuenciaAleatoria[i]); // Enciende el Led RGB del color almacenado en el arreglo
    cambiarColorRGB(0); // Cambia el color del LED RGB enviando el cero para apagarlo    
  }
  estadoJuego = 2;  // Cambia el estado del juego a: turno del jugador
  Serial.println("Ahora es tu turno.");  
}
/*
  Función: detectarBotonPresionado
  Descripción: Detecta si alguno de los botones ha sido presionado retornando True si alguno se detectó en alto.
*/
bool detectarBotonPresionado(){  
  if (!digitalRead(botonRojo) || !digitalRead(botonVerde) || !digitalRead(botonAzul) || !digitalRead(botonAmarillo)){
    return true;
  }else{
    return false;
  }
}
/*
  Función: identificarBoton
  Descripción: Identifica cuál de los botones fue presionado, se codifica según los valores definidos y retorna el valor.
  1: Boton Rojo
  2: Boton Verde
  3: Boton Azul
  4: Boton Amarillo  
*/
int identificarBoton(){
  int boton = 0;
  if(!digitalRead(botonRojo)){
    boton = 1;  
  }else if(!digitalRead(botonVerde)){
    boton = 2;  
  }else if(!digitalRead(botonAzul)){        
    boton = 3;
  }else if(!digitalRead(botonAmarillo)){        
    boton = 4;
  }
  return boton; 
}

/*
  Función: compararMejorPuntaje
  Descripción: Comprueba si el puntaje actual es mejor que el último mejor puntaje registrado, si es así lo guarda
*/
void compararMejorPuntaje(){
  // Si el nivel actual es mayor al mejor puntaje, lo define como nuevo mejor puntaje
  if(nivelActual > mejorPuntaje){
    mejorPuntaje = nivelActual;
    Serial.print("Has superado el récord. Tu puntaje: ");
    Serial.println(mejorPuntaje);    
  }
}
/*
  Función: sonarBuzzer
  Descripción: Hace sonar el buzzer con un tono personalizado para cada botón durante un tiempo determinado
*/
void sonarBuzzer(uint8_t i){
  tone(pinBuzzer, codigosColores[i][3], duracionPulsoBuzzer);
}
/*
  Función: cambiarColorRGB
  Descripción: Establece la intensidad de cada uno de los pines de conexión del led RGB (Pueden tomar valores entre 0 - 255)
*/
void cambiarColorRGB(int numeroColor){
  ledcWrite(pinRojo, codigosColores[numeroColor][0]);
  ledcWrite(pinVerde, codigosColores[numeroColor][1]);
  ledcWrite(pinAzul, codigosColores[numeroColor][2]);
  delay(duracionPulsoLED); // Espera durante un tiempo determinado
}