#include "Cerradura.h"

Cerradura cerradura;

void setup() {
  Serial.begin(115200);
  cerradura.iniciar(); // Inicializar la cerradura
  Serial.println("AAA");
}

void loop() {
  // Esperar activamente a que se presione #
  char key = cerradura.leerTecla();
  if (key == '#') {
    cerradura.ingresarClave(); // Ingresar la clave
  }

  // Actualizar la lógica de espera
  cerradura.actualizar();
}