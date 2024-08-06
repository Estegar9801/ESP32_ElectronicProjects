/* 
  Proyecto: Juego de memoria - Pulsadores
  Descripción: En este proyecto las teclas se iluminan aleatoriamente y el jugador debe reproducir la secuencia de luz
  sin cometer un error. Usamos un LED RGB para mostrar los colores, un buzzer para tener un sonido para cada tecla y
  pulsadores como elementos accionables.
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 29/07/2024
  Última modificación: 02/08/2024
*/

// ------------------------------------------------------------------------------
// Definición de pines
// ------------------------------------------------------------------------------
#define botonInicio 25  // Pin al que está conectado el botón de Inicio
#define botonRojo 21  // Pin al que está conectado el botón Rojo
#define botonVerde 22 // Pin al que está conectado el botón Verde
#define botonAzul 5 // Pin al que está conectado el botón Azul
#define botonAmarillo 18  // Pin al que está conectado el botón Amarillo

#define pinRojo 15  // Pin al que está conectado la terminal R del LED RGB 
#define pinVerde 2  // Pin al que está conectado la terminal G del LED RGB 
#define pinAzul 0   // Pin al que está conectado la terminal B del LED RGB 

#define pinBuzzer 13 // Pin al que está conectado el Buzzer

// ------------------------------------------------------------------------------
// Declaración de variables
// ------------------------------------------------------------------------------
// Estados del juego: 
// 0 -> En espera del inicio
// 1 -> Mostrando secuencia
// 2 -> Esperando secuencia del jugador
// 3 -> Juego terminado

uint8_t estadoJuego;

const uint8_t numeroNiveles = 5;
uint8_t contadorSecuenciaActual = 0;
uint8_t nivelActual;

uint8_t secuenciaAleatoria[numeroNiveles];
uint8_t secuenciaJugador[numeroNiveles];
uint16_t coloresRGB[5][4] = {{0,0,0,0}, {255,0,0,220}, {0,255,0,250}, {0,0,255,280}, {255,255,0,310}};
String stringColores[5] = {"Apagado", "Rojo", "Verde", "Azul", "Amarillo"};

const int TIEMPO_REBOTE = 300; // Tiempo de rebote para todos los pulsadores. 300ms = 0,3s
long contadorRebote;
bool rebotePulsadores;

uint16_t frecuenciaPWMBuzzer = 2000;
uint16_t resolucionPWMBuzzer = 16;
uint16_t frecuenciaPWMLED = 2000;
uint8_t resolucionPWMLED = 8;

uint16_t duracionPulsoBuzzer;
uint16_t duracionPulsoLED;

void setup() {
  // Inicio del puerto serial
  Serial.begin(115200);

  estadoJuego = 0;
  contadorRebote = millis();  

  duracionPulsoBuzzer = 100;
  duracionPulsoLED = 300;

  pinMode(botonInicio, INPUT_PULLUP);
  pinMode(botonRojo, INPUT_PULLUP);
  pinMode(botonAzul, INPUT_PULLUP);
  pinMode(botonVerde, INPUT_PULLUP);
  pinMode(botonAmarillo, INPUT_PULLUP);

  ledcAttach(pinBuzzer, frecuenciaPWMBuzzer, resolucionPWMBuzzer);
  ledcAttach(pinRojo, frecuenciaPWMLED, resolucionPWMLED);
  ledcAttach(pinVerde, frecuenciaPWMLED, resolucionPWMLED);
  ledcAttach(pinAzul, frecuenciaPWMLED, resolucionPWMLED);
}

void loop() {
  int botonOprimido = 0;

  // Si se detecta que un boton se oprimió, se identifica cual fue.
  if(detectarBotonPresionado() && millis() > contadorRebote + TIEMPO_REBOTE){
    contadorRebote = millis();
    botonOprimido = identificarBoton();    
  }

  // Si el juego no ha empezado y se oprimió el botón de inicio, se reinician las variables e inicia el juego.
  if(estadoJuego == 0 && botonOprimido == 5){
    reiniciarVariables();
    botonOprimido = 0;
    estadoJuego = 1;    
  }
  
  // Si el nivel 
  if(nivelActual < numeroNiveles){ 
    if(estadoJuego == 1){
      mostrarSecuencia();
      delay(200);
    }else if(estadoJuego == 2){
      if(botonOprimido != 0 && botonOprimido != 5 && (contadorSecuenciaActual <= nivelActual)){
        sonarBuzzer(secuenciaAleatoria[botonOprimido]);
        cambiarColorRGB(botonOprimido); 
        cambiarColorRGB(0);               
        if(botonOprimido == secuenciaAleatoria[contadorSecuenciaActual]){          
          botonOprimido = 0;
          contadorSecuenciaActual += 1;
        }else{
          Serial.println("Has perdido. Pulsa para empezar nuevamente");          
          estadoJuego = 0;
        }
      }else if(contadorSecuenciaActual > nivelActual){      
        Serial.println("Secuencia correcta, sube el nivel.");
        contadorSecuenciaActual = 0;
        nivelActual += 1;
        estadoJuego = 1;        
      }
    }
  }else if(nivelActual >= numeroNiveles && estadoJuego != 0){
    Serial.println("Felicidades has ganado. Pulsa para empezar nuevamente.");  
    estadoJuego = 0;  
  }
}

void reiniciarVariables(){  
  estadoJuego = 0;
  nivelActual = 0;
  contadorSecuenciaActual = 0;
  generarSemilla();
  generarSecuencia();
}

void generarSemilla(){
  randomSeed(millis());
}

int generarNumeroAleatorio(){
  return random(1,5);
}

void generarSecuencia(){
  for (int i = 0; i < numeroNiveles; i++){
    secuenciaAleatoria[i] = generarNumeroAleatorio();    
  }
}

void mostrarSecuencia(){  
  delay(500);
  for (int i=0; i <= nivelActual; i++){    
    sonarBuzzer(secuenciaAleatoria[i]);
    cambiarColorRGB(secuenciaAleatoria[i]);    
    cambiarColorRGB(0);    
  }
  estadoJuego = 2;
}
/*
  Función: cambiarColorRGB
  Descripción: Establece la intensidad de cada uno de los pines de conexión del led RGB (Pueden tomar valores entre 0 - 255)
*/
void cambiarColorRGB(int numeroColor){
  ledcWrite(pinRojo, coloresRGB[numeroColor][0]);
  ledcWrite(pinVerde, coloresRGB[numeroColor][1]);
  ledcWrite(pinAzul, coloresRGB[numeroColor][2]);
  delay(duracionPulsoLED); // Espera durante un tiempo determinado
}
/*
  Función: detectarBotonPresionado
  Descripción: Detecta si alguno de los botones ha sido presionado retornando True si alguno se detectó en alto.
*/
bool detectarBotonPresionado(){  
  if (!digitalRead(botonInicio) || !digitalRead(botonRojo) || !digitalRead(botonVerde) || !digitalRead(botonAzul) || !digitalRead(botonAmarillo)){
    return true;    
  }else{
    return false;
  }
}
/*
  Función: identificarBoton
  Descripción: Identifica cuál de los botones de color fue presionado, se codifica según los valores definidos y retorna el valor.
  1: Boton Rojo
  2: Boton Verde
  3: Boton Azul
  4: Boton Amarillo
  5: Boton Inicio
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
  }else if(!digitalRead(botonInicio)){
    boton = 5;
  }
  return boton; 
}
/*
  Función: sonarBuzzer
  Descripción: Hace sonar el buzzer con un tono personalizado para cada boton durante un tiempo determinado
*/
void sonarBuzzer(uint8_t i){
  tone(pinBuzzer, coloresRGB[i][3], duracionPulsoBuzzer);
}