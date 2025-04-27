/* 
  Proyecto: Generando números aleatorios
  Descripción: En este script te mostramos una forma de generar
  números pseudo-aleatorios que te podrá servir para tus proyectos.
  Generaremos uno nuevo cada que oprimamos un botón.
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 26/04/2024
  Última modificación: 26/04/2025
*/
// Define el pin dónde esta conectado el botón
#define PIN_BOTON 25

// Variables para controlar el rebote del botón
const int TIEMPO_REBOTE = 300;
long contadorRebote = 0;

void setup() {
  Serial.begin(115200); // Inicia el puerto serial a 115200 baudios
  pinMode(PIN_BOTON, INPUT_PULLUP); // Definimos el pin como entrada pull-up

  delay(500);  // Retraso de 500s para estabilidad
  Serial.println("¡Bienvenido!");
  Serial.println("Oprime el botón para generar un número aleatorio!");

  contadorRebote = millis();  // Inicia el contador del rebote
}

void loop() {
  // Si se oprime el botón y ya  no se ha completado el tiempo de rebote
  if ( !digitalRead(PIN_BOTON)  && (millis() > (contadorRebote + TIEMPO_REBOTE))){
    contadorRebote = millis();  // Reinicia el contador del rebote
    generarSemilla();  // Genera una semilla
    int numeroAleatorio = generarNumeroAleatorio(); // Recibe el número aleatorio

    // Imprime el número aleatorio en el puerto serial
    Serial.print("El número aleatorio es: "); 
    Serial.println(numeroAleatorio);
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
