/*  
  Proyecto: Conexión Bluetooth 
  Descripción: En este proyecto aprenderemos a enviar mensajes desde un 
  dispositivo móvil al ESP32 de manera inalámbrica a través de Bluetooth.
  Autor: David Esteban Garnica - Ingeniería Electrónica  
  Fecha de creación: 17/03/2025  
  Última modificación: 17/03/2025  
*/

#include "BluetoothSerial.h"

// Objeto para manejar el Bluetooth
BluetoothSerial SerialBT;

// Definir el pin del LED
const int ledPin = 2;

void setup() {
    Serial.begin(115200);   // Monitor serie
    SerialBT.begin("ESP32_BT"); // Nombre del ESP32 en Bluetooth
    pinMode(ledPin, OUTPUT);  // Configura el pin del LED como salida
    Serial.println("Bluetooth listo. Esperando comandos...");
}

void loop() {
    if (SerialBT.available()) {  // Si hay datos disponibles en Bluetooth
        String received = SerialBT.readString(); // Leer el mensaje recibido
        received.trim(); // Eliminar espacios y saltos de línea

        Serial.print("Comando recibido: ");
        Serial.println(received);

        if (received.equalsIgnoreCase("encender")) {
            digitalWrite(ledPin, HIGH); // Enciende el LED
            SerialBT.println("LED ENCENDIDO");
        } 
        else if (received.equalsIgnoreCase("apagar")) {
            digitalWrite(ledPin, LOW); // Apaga el LED
            SerialBT.println("LED APAGADO");
        } 
        else {
            SerialBT.println("Comando no reconocido");
        }
    }
}