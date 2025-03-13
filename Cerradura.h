#ifndef CERRADURA_H
#define CERRADURA_H

#include "Teclado.h"
#include "PantallaLCD.h"
#include "Chapa.h"
#include "ConexionIoT.h"

class Cerradura {
  private:
    Teclado teclado;
    PantallaLCD pantalla;
    Chapa chapa;
    ConexionIoT conexion;
    String clave;
    const String claveCorrecta = "1587";
    int intentos = 3;
    bool desbloqueado = false;
    unsigned long tiempoAnterior = 0; // Para el manejo de tiempos sin delay
    bool esperandoConfirmacion = false; // Para controlar la espera después de verificar la clave

  public:
    Cerradura() : teclado(), pantalla(), chapa(), conexion() {}

    void iniciar() {
      pantalla.iniciarLCD();
      tiempoAnterior = millis(); // Inicializar el tiempo
      mostrarMensajeInicial(); // Mostrar el mensaje inicial al iniciar
    }

    void mostrarMensajeInicial() {
      pantalla.limpiar();
      pantalla.mostrarMensaje("Presione # para", 0, 0);
      pantalla.mostrarMensaje("ingresar clave", 0, 1);
    }

    char leerTecla() {
      return teclado.leerTecla();
    }

    void ingresarClave() {
      pantalla.limpiar();
      pantalla.mostrarMensaje("Ingrese clave:", 0, 0);
      clave = "";
      char key;
      while (true) {
        key = teclado.leerTecla();
        if (key) {
          if (key == '#') {
            break; // Salir del bucle al presionar #
          } else if (key == '*') {
            if (!clave.isEmpty()) {
              clave.remove(clave.length() - 1, 1);
              pantalla.mostrarMensaje("                ", 0, 1); // Limpia la línea
              pantalla.mostrarMensaje(clave, 0, 1);
            }
          } else {
            clave += key;
            pantalla.mostrarMensaje(clave, 0, 1);
          }
        }
      }
      verificarClave();
    }

    void verificarClave() {
      if (clave == claveCorrecta) {
        pantalla.limpiar();
        pantalla.mostrarMensaje("Acceso concedido", 0, 0);
        desbloqueado = true;
        chapa.abrirPuerta();
        conexion.obtenerEstadoPuerta("abierta"); 
        esperandoConfirmacion = true; // Activar la espera
        tiempoAnterior = millis(); // Reiniciar el tiempo
      } else {
        intentos--;
        pantalla.limpiar();
        pantalla.mostrarMensaje("Clave incorrecta", 0, 0);
        pantalla.mostrarMensaje("Intentos: " + String(intentos), 0, 1);
        esperandoConfirmacion = true; // Activar la espera
        tiempoAnterior = millis(); // Reiniciar el tiempo

        if (intentos == 0) {
          pantalla.limpiar();
          pantalla.mostrarMensaje("Cerradura bloqueada", 0, 0);
          esperandoConfirmacion = true; // Activar la espera
          tiempoAnterior = millis(); // Reiniciar el tiempo
        }
      }
    }

    void actualizar() {
      if (esperandoConfirmacion) {
        // Verificar si han pasado 3 segundos
        if (millis() - tiempoAnterior >= 3000) {
          esperandoConfirmacion = false; // Desactivar la espera
          chapa.cerrarPuerta();
          conexion.obtenerEstadoPuerta("cerrada");
          if (intentos == 0 || desbloqueado) {
            iniciar(); // Reiniciar si se agotaron los intentos
          } else {
            ingresarClave(); // Volver al mensaje inicial
          }
        }
      }
      String estadoAPI = conexion.recibirEstadoChapa();
      if (estadoAPI == "abierta" && !desbloqueado) {
        pantalla.limpiar();
        pantalla.mostrarMensaje("Abierto por API", 0, 0);
        chapa.abrirPuerta();
        desbloqueado = true;
        esperandoConfirmacion = true;
        tiempoAnterior = millis();
      }
    }
};

#endif