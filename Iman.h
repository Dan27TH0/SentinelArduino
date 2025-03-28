#ifndef IMAN_H
#define IMAN_H

#include "Buzzer.h"

class Iman {
  private:
    int pinIman = 34;
    bool estadoIman;
    unsigned long tiempoAnterior;
    Buzzer &buz; // Referencia al buzzer

  public:
    Iman(Buzzer &buzzer) : buz(buzzer) {
      pinMode(pinIman, INPUT);
      estadoIman = (digitalRead(pinIman) == LOW);
      tiempoAnterior = millis();
    }

    // Método para detectar el imán
    void detectar() {
      bool imanDetectado = (digitalRead(pinIman) == LOW);

      if (imanDetectado) {
        Serial.println("Imán detectado");
      } else {
        Serial.println("Imán no detectado");
      }

      estadoIman = imanDetectado;

      if (!estadoIman) {
        buz.sonarAlarma(2000); // Activar la alarma si el imán no está detectado
      }
    }
};
#endif