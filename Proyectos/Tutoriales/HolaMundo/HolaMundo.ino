void setup() {
  Serial.begin(115200);     // Inicia el puerto serial a 115200 baudios
}

void loop() {
  Serial.println("Hola mundo");   // Imprime un mensaje en el monitor serie: "Hola mundo"
  delay(2000);    // Genera un delay cada 2000ms o 2s
}