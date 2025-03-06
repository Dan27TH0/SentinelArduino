#ifndef CERRADURA_H
#define CERRADURA_H

#include "Teclado.h"
#include "PantallaLCD.h"
#include "ConexionIoT.h"

class Cerradura {
  private:
    Teclado teclado;
    PantallaLCD pantalla;
    ConexionIoT iot;
    String clave;
    const String claveCorrecta = "1587";
    int intentos = 3;
    bool desbloqueado = false;

  public:
    Cerradura() : teclado(), pantalla(), iot() {}

    void iniciar() {
      pantalla.iniciarLCD();
    }

    void mostrarMensajeInicial() {
      pantalla.mostrarMensaje("Presione # para", 0, 0);
      pantalla.mostrarMensaje("ingresar clave", 0, 1);
    }

    void ingresarClave() {
      pantalla.limpiar();
      pantalla.mostrarMensaje("Ingrese clave:", 0, 0);
      clave = "";
      char key;
      while ((key = teclado.leerTecla()) != '#') {
        if (key) {
          if (key == '*') {
            if (!clave.isEmpty()) {
              clave.remove(clave.length() - 1, 1);
            }
          } else {
            clave += key;
          }
          pantalla.mostrarMensaje(clave, 0, 1);
        }
      }
      verificarClave();
    }

    void verificarClave() {
      if (clave == claveCorrecta) {
        pantalla.limpiar();
        pantalla.mostrarMensaje("Acceso concedido", 0, 0);
        desbloqueado = true;
        iot.enviarEstadoPuerta("Cerradura desbloqueada");
      } else {
        intentos--;
        pantalla.limpiar();
        pantalla.mostrarMensaje("Clave incorrecta", 0, 0);
        pantalla.mostrarMensaje("Intentos: " + String(intentos), 0, 1);
        if (intentos == 0) {
          pantalla.limpiar();
          pantalla.mostrarMensaje("Cerradura bloqueada", 0, 0);
          iot.enviarEstadoPuerta("Cerradura bloqueada");
          iniciar();
        }
      }
    }
};

#endif