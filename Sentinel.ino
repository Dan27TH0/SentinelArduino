#include "Cerradura.h"

Cerradura cerradura;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cerradura.iniciar();
  cerradura.mostrarMensajeInicial();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("AAA");
  cerradura.ingresarClave();
  delay(100);
}
