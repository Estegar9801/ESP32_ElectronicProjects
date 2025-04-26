/* 
  Proyecto: Robot Explorador Bluetooth  
  Descripción: En este proyecto se desarrolla un Robot Explorador controlado 
  por comandos enviados vía Bluetooth. El robot puede avanzar, retroceder y 
  girar a la izquierda o derecha. Además, incorpora un sensor de ultrasonido 
  para detectar obstáculos cercanos y detener el movimiento automáticamente 
  si se detecta un objeto a menos de 20 cm. 
  
  Autor: David Esteban Garnica - Ingeniería Electrónica  
  Fecha de creación: 09/02/2025  
  Última modificación: 09/02/2025  
*/

#include <EasyUltrasonic.h>
#include "BluetoothSerial.h"

// Definición de pines para el control de los motores
#define MOTOR_IZQ_CTR2 33
#define MOTOR_IZQ_CTR1 32
#define MOTOR_IZQ_PWM 27

#define MOTOR_DER_CTR2 23
#define MOTOR_DER_CTR1 18
#define MOTOR_DER_PWM 19

// Pines del sensor ultrasónico
#define PIN_TRIGGER 16
#define PIN_ECHO 17


// Objetos para el control del Sensor y del Bluetooth
EasyUltrasonic sensorHC;
BluetoothSerial SerialBT;

// Configuración del PWM
const int frecuenciaPWM = 3000;
const int resolucionPWM = 8;

// Variables de tiempo para controlar la frecuencia de medición del sensor
unsigned long tiempoActualMedicion;
const int INTERVALO_MEDICION = 100; // Medir cada 100 ms

// Umbral de distancia mínima para detener el robot (en centímetros)
const uint8_t LIMITE_INFERIOR = 20;

// Estado actual del robot
// 0: Detenido | 1: Avanzando | 2: Retrocediendo | 3: Girando a la izquierda | 4: Girando a la derecha
uint8_t estadoRobot = 0;

void setup() {
  Serial.begin(115200);         // Comunicación con el monitor serie
  SerialBT.begin("ROBOT_BT");   // Inicializa el Bluetooth con el nombre "ROBOT_BT"

  // Configuración de pines de control de los motores
  pinMode(MOTOR_IZQ_CTR1, OUTPUT);
  pinMode(MOTOR_IZQ_CTR2, OUTPUT);
  pinMode(MOTOR_DER_CTR1, OUTPUT);
  pinMode(MOTOR_DER_CTR2, OUTPUT);

  // Asociación de los canales PWM a los pines
  ledcAttach(MOTOR_IZQ_PWM, frecuenciaPWM, resolucionPWM);
  ledcAttach(MOTOR_DER_PWM, frecuenciaPWM, resolucionPWM);

  // Inicialización del sensor ultrasónico
  sensorHC.attach(PIN_TRIGGER, PIN_ECHO);

  // Registro del tiempo inicial de medición
  tiempoActualMedicion = millis();
}

void loop() {
  // Revisión de comandos recibidos por Bluetooth
  if (SerialBT.available()) {
    String comandoRecibido = SerialBT.readString();
    comandoRecibido.trim();

    // Procesamiento del comando recibido
    if (comandoRecibido.equalsIgnoreCase("1")) {
      avanzar();
      estadoRobot = 1;
      SerialBT.println("Robot avanzando...");
    }
    else if (comandoRecibido.equalsIgnoreCase("2")) {
      retroceder();
      estadoRobot = 2;
      SerialBT.println("Robot retrocediendo...");
    }
    else if (comandoRecibido.equalsIgnoreCase("3")) {
      girarIzquierda();
      estadoRobot = 3;
      SerialBT.println("Robot girando a la izquierda.");
    }
    else if (comandoRecibido.equalsIgnoreCase("4")) {
      girarDerecha();
      estadoRobot = 4;
      SerialBT.println("Robot girando a la derecha.");
    }
    else if (comandoRecibido.equalsIgnoreCase("0")) {
      detener();
      estadoRobot = 0;
    }
    else {
      SerialBT.println("Comando no reconocido.");
      SerialBT.print("Comandos disponibles:\n  0. Detener\n  1. Avanzar\n  2. Reversa\n  3. Giro Izquierda\n  4. Giro Derecha\n");
    }
  }

  // Revisión periódica de la distancia medida por el sensor
  if (millis() > (tiempoActualMedicion + INTERVALO_MEDICION)) {
    tiempoActualMedicion = millis();
    float distanciaMedida = sensorHC.getDistanceCM();
    imprimirDistancia(distanciaMedida);
    comprobarDistancia(distanciaMedida);
  }
}

// Función para mostrar la distancia medida en el monitor serie
void imprimirDistancia(float distancia) {
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
}

// Función para configurar la velocidad de los motores mediante PWM
void configurarPWM(int motorIzquierdo, int motorDerecho) {
  ledcWrite(MOTOR_IZQ_PWM, motorIzquierdo);
  ledcWrite(MOTOR_DER_PWM, motorDerecho);
}

// Función para avanzar
void avanzar() {
  if (estadoRobot == 2) {
    // Detener antes de cambiar de dirección
    detener();
    delay(1000);
  }

  configurarPWM(150, 220);

  digitalWrite(MOTOR_IZQ_CTR1, HIGH);
  digitalWrite(MOTOR_IZQ_CTR2, LOW);

  digitalWrite(MOTOR_DER_CTR1, HIGH);
  digitalWrite(MOTOR_DER_CTR2, LOW);
}

// Función para retroceder
void retroceder() {
  if (estadoRobot == 1) {
    // Detener antes de cambiar de dirección
    detener();
    delay(1000);
  }

  configurarPWM(200, 220);

  digitalWrite(MOTOR_IZQ_CTR1, LOW);
  digitalWrite(MOTOR_IZQ_CTR2, HIGH);

  digitalWrite(MOTOR_DER_CTR1, LOW);
  digitalWrite(MOTOR_DER_CTR2, HIGH);
}

// Función para detener el movimiento del robot
void detener() {
  digitalWrite(MOTOR_IZQ_CTR1, LOW);
  digitalWrite(MOTOR_IZQ_CTR2, LOW);

  digitalWrite(MOTOR_DER_CTR1, LOW);
  digitalWrite(MOTOR_DER_CTR2, LOW);
}

// Función para girar a la izquierda
void girarIzquierda() {
  configurarPWM(0, 200); // Solo se activa el motor derecho

  digitalWrite(MOTOR_IZQ_CTR1, LOW);
  digitalWrite(MOTOR_IZQ_CTR2, LOW);

  digitalWrite(MOTOR_DER_CTR1, HIGH);
  digitalWrite(MOTOR_DER_CTR2, LOW);
}

// Función para girar a la derecha
void girarDerecha() {
  configurarPWM(200, 0); // Solo se activa el motor izquierdo

  digitalWrite(MOTOR_IZQ_CTR1, HIGH);
  digitalWrite(MOTOR_IZQ_CTR2, LOW);

  digitalWrite(MOTOR_DER_CTR1, LOW);
  digitalWrite(MOTOR_DER_CTR2, LOW);
}

// Función que detiene el robot si detecta un obstáculo muy cerca
void comprobarDistancia(float distancia) {
  if (distancia <= LIMITE_INFERIOR && estadoRobot == 1) {
    SerialBT.println("¡Obstáculo detectado! Robot detenido.");
    estadoRobot = 0;
    detener();
  }
}