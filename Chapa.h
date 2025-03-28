class Chapa{
  private:
    int pinChapa = 4;
    bool estadoChapa;
  public:
    Chapa(){
      estadoChapa = false;
      pinMode(pinChapa, OUTPUT);
    }

    void abrirPuerta(){
      digitalWrite(pinChapa, HIGH);
      estadoChapa = true;
    }

    void cerrarPuerta(){
      digitalWrite(pinChapa, LOW);
      estadoChapa = false;
    }

    void activarPuerta(){
      estadoChapa = !estadoChapa;
      digitalWrite(pinChapa, estadoChapa ? HIGH : LOW);
    }
};