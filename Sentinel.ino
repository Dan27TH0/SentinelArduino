#include "Cerradura.h"

Cerradura cerradura;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("AAA");
  cerradura.iniciar();
  cerradura.mostrarMensajeInicial();
}

void loop() {
  // put your main code here, to run repeatedly:
  cerradura.ingresarClave();
  delay(100);
}
