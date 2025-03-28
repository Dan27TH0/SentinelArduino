#ifndef CHAPA_H
#define CHAPA_H

class Chapa {
private:
    const int pinChapa;
    bool estado;

public:
    Chapa(int pin = 4) : pinChapa(pin), estado(false) {
        pinMode(pinChapa, OUTPUT);
    }

    void abrirPuerta() {
        digitalWrite(pinChapa, HIGH);
        estado = true;
    }

    void cerrarPuerta() {
        digitalWrite(pinChapa, LOW);
        estado = false;
    }

    void alternar() {
        estado = !estado;
        digitalWrite(pinChapa, estado);
    }

    bool estaAbierta() const { return estado; }
};

#endif