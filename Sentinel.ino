#include "Cerradura.h"
#include "Iman.h"

Cerradura cerradura;
Iman iman(cerradura.getBuzzer());
void setup() {
  Serial.begin(115200);
  cerradura.iniciar(); // Inicializar la cerradura
  Serial.println("Sistema de cerradura iniciado.");
}

void loop() {
  // Esperar activamente a que se presione #
  char key = cerradura.leerTecla();
  if(!cerradura.isPuertaDesbloqueada())
    iman.detectar();

  if (key == '#') {
    cerradura.ingresarClave(); // Ingresar la clave
  }

  cerradura.actualizar();
}