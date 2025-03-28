#ifndef BUZZER_H
#define BUZZER_H

class Buzzer {
  private:
    int pinBuzzer = 15;
    bool estadoBuzzer;
    unsigned long tiempoAnterior = 0;
  public:
    // Constructor
    Buzzer(){
      estadoBuzzer = false;
      pinMode(pinBuzzer, OUTPUT);
    }

    // Método para activar el buzzer
    void activar() {
      digitalWrite(pinBuzzer, HIGH);
      estadoBuzzer = true;
    }

    // Método para desactivar el buzzer
    void desactivar() {
      digitalWrite(pinBuzzer, LOW);
      estadoBuzzer = false;
    }

    // Método para alternar el estado del buzzer
    void alternar() {
      estadoBuzzer = !estadoBuzzer;
      digitalWrite(pinBuzzer, estadoBuzzer ? HIGH : LOW);
    }

    // Método para hacer sonar el buzzer como una alarma (dos pitidos cortos)
    void sonarAlarma(int tiempoDuracion) {
      unsigned long inicioPitido = millis(); // Tiempo de inicio del pitido

      activar(); 
      Serial.println("A");
      while (millis() - inicioPitido < tiempoDuracion) {}
      desactivar(); 
      Serial.println("O");

      inicioPitido = millis(); 
      while (millis() - inicioPitido < 200) {}
      
      activar(); 
      Serial.println("A");
      while (millis() - inicioPitido < tiempoDuracion) {}
      desactivar(); 
      Serial.println("O");
      
      inicioPitido = millis(); 
      while (millis() - inicioPitido < 200) {}

      activar(); 
      Serial.println("A");
      while (millis() - inicioPitido < tiempoDuracion) {}
      desactivar(); 
      Serial.println("O");
    }
    
};

#endif