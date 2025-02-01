/*  
  Proyecto: Contando números  
  Descripción: Este proyecto muestra cómo contar con el ESP32 usando una variable  
  que se incrementa cada segundo. El valor se imprimirá en el puerto serial,  
  recorriendo los números del 0 al 255 de manera cíclica.  

  Autor: David Esteban Garnica - Ingeniería Electrónica  
  Fecha de creación: 01/02/2025  
  Última modificación: 01/02/2025  
*/  

uint8_t contador = 0; // Variable de 8 bits sin signo para contar de 0 a 255  

void setup() {  
  Serial.begin(115200);  // Inicializa la comunicación serial a 115200 baudios  
}  

void loop() {  
  Serial.println(contador);  // Imprime el valor actual del contador  
  contador = (contador + 1) % 256; // Incrementa y reinicia a 0 después de 255  
  delay(1000);  // Espera 1 segundo antes de la siguiente iteración  
}