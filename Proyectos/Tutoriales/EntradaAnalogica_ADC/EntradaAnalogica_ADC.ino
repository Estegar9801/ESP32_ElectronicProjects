/*  
  Proyecto: Entrada Analógica  
  Descripción: Este proyecto demuestra cómo leer el voltaje en una de las entradas analógicas del ESP32  
  utilizando su convertidor analógico-digital (ADC). Para simular variaciones en el voltaje, se emplea  
  un potenciómetro, y los valores obtenidos se muestran en el monitor serie.  
  Autor: Johan Darío Hernández - Ingeniería Electrónica  
  Fecha de creación: 01/03/2025  
  Última modificación: 01/03/2025  
*/

#define PIN_POTENCIOMETRO 36  // Pin de entrada analógica donde está conectado el potenciómetro

void setup() {
  Serial.begin(115200);     // Inicia la comunicación serie a 115200 baudios
  analogReadResolution(12); // Configura la resolución del ADC en 12 bits (valores de 0 a 4095)
}

void loop() {
  // Lee el valor analógico del potenciómetro en una escala de 0 a 4095
  int lecturaAnalogica = analogRead(PIN_POTENCIOMETRO);

  // Convierte la lectura analógica a milivoltios (rango de 0 mV a 5000 mV)
  int lecturaVoltios = analogReadMilliVolts(PIN_POTENCIOMETRO);

  // Muestra los valores en el monitor serie
  Serial.printf("Lectura del ADC: %d\n", lecturaAnalogica);
  Serial.printf("Lectura del ADC en milivoltios: %dmV\n\n", lecturaVoltios);

  delay(500); // Pausa la ejecución durante 500 ms para evitar lecturas excesivas
}