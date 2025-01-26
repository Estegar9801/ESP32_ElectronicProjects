/* 
  Proyecto: Hola Mundo
  Descripción: Se hace uso del puerto serie para mostrar un mensaje desde el ESP32. 
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 26/01/2024
  Última modificación: 26/01/2024
*/

void setup() {
  Serial.begin(115200);     // Inicia el puerto serial a 115200 baudios
}

void loop() {
  Serial.println("Hola mundo");   // Imprime un mensaje en el monitor serie: "Hola mundo"
  delay(2000);    // Genera un delay cada 2000ms o 2s
}