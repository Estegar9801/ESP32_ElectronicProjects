#define pinRojo 15 // Pin al que está conectado la terminal R del led RGB 
#define pinVerde 2  // Pin al que está conectado la terminal G del led RGB 
#define pinAzul 0
#define pinAmarillo 4

#define botonRojo 36  // Pin al que está conectado el botón Rojo
#define botonVerde 39 // Pin al que está conectado el botón Verde
#define botonAzul 34 // Pin al que está conectado el botón Azul
#define botonAmarillo 35  // Pin al que está conectado el botón Amarillo

void setup() {
  pinMode(botonRojo, INPUT_PULLUP);
  pinMode(botonVerde, INPUT_PULLUP);
  pinMode(botonAzul, INPUT_PULLUP);
  pinMode(botonAmarillo, INPUT_PULLUP);

  pinMode(pinRojo, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAzul, OUTPUT);
  pinMode(pinAmarillo, OUTPUT);

}

void loop() {
  if (!digitalRead(botonRojo)) {
    digitalWrite(pinRojo, !digitalRead(pinRojo));
    delay(300);
  }
  if (!digitalRead(botonVerde)) {
    digitalWrite(pinVerde, !digitalRead(pinVerde));
    delay(300);
  }
  if (!digitalRead(botonAzul)) {
    digitalWrite(pinAzul, !digitalRead(pinAzul));
    delay(300);
  }
  if (!digitalRead(botonAmarillo)) {
    digitalWrite(pinAmarillo, !digitalRead(pinAmarillo));
    delay(300);
  }
}
