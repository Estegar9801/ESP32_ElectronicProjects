/*  
  Proyecto: Conexión Bluetooth 
  Descripción: En este proyecto aprenderemos a enviar mensajes desde un 
  dispositivo móvil al ESP32 de manera inalámbrica a través de Bluetooth.
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 21/04/2025
  Última modificación: 21/04/2025
*/

#include "BluetoothSerial.h"

#define PIN_LED 2   // Define el pin donde estará conectado el LED

// Objeto para manejar el Bluetooth
BluetoothSerial SerialBT;

void setup() {
    Serial.begin(115200);   // Monitor serie
    SerialBT.begin("ESP32_BT"); // Nombre del ESP32 en Bluetooth
    pinMode(PIN_LED, OUTPUT);  // Configura el pin del LED como salida

    delay(200); // Espera 200ms para estabilidad
    Serial.println("Bluetooth listo. Esperando comandos...");
}

void loop() {
    if (SerialBT.available()) {  // Si hay datos disponibles en Bluetooth
        String received = SerialBT.readString(); // Leer el mensaje recibido
        received.trim(); // Eliminar espacios y saltos de línea

        Serial.print("Comando recibido: ");
        Serial.println(received);

        if (received.equalsIgnoreCase("encender")) {
            digitalWrite(PIN_LED, HIGH); // Enciende el LED
            SerialBT.println("LED ENCENDIDO");
        } 
        else if (received.equalsIgnoreCase("apagar")) {
            digitalWrite(PIN_LED, LOW); // Apaga el LED
            SerialBT.println("LED APAGADO");
        } 
        else {
            SerialBT.println("Comando no reconocido");
        }
    }
}