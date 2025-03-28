#ifndef CERRADURA_H
#define CERRADURA_H

#include "Teclado.h"
#include "PantallaLCD.h"
#include "Chapa.h"
#include "ConexionIoT.h"
#include "Buzzer.h"

class Cerradura {
private:
    // Componentes
    Teclado teclado;
    PantallaLCD pantalla;
    Chapa chapa;
    ConexionIoT conexion;
    Buzzer buzzer;
    
    // Configuración
    const String CLAVE_CORRECTA = "1587";
    const unsigned long TIEMPO_ESPERA = 3000;
    
    // Estado
    String claveIngresada;
    int intentosRestantes = 3;
    bool desbloqueado = false;
    unsigned long tiempoReferencia = 0;
    bool enEspera = false;

    void manejarEntradaTeclado(char tecla) {
        if (tecla == '*' && !claveIngresada.isEmpty()) {
            claveIngresada.remove(claveIngresada.length() - 1);
            pantalla.mostrarMensaje("                ", 0, 1);
        } else if (tecla != '*') {
            claveIngresada += tecla;
            pantalla.mostrarMensaje(claveIngresada, 0, 1);
        }
    }

    void reiniciarIntentos() { intentosRestantes = 3; }

    void publicarEstado(const String& estado) {
        conexion.publicar("sentinel/estadoPuerta", estado.c_str());
    }

    void concederAcceso() {
        pantalla.limpiar();
        pantalla.mostrarMensaje("Acceso concedido", 0, 0);
        desbloqueado = true;
        chapa.abrirPuerta();
        publicarEstado("DESBLOQUEADO");
        reiniciarIntentos();
    }

    void denegarAcceso() {
        intentosRestantes--;
        pantalla.limpiar();
        pantalla.mostrarMensaje("Clave incorrecta", 0, 0);
        pantalla.mostrarMensaje("Intentos: " + String(intentosRestantes), 0, 1);
        buzzer.sonarAlarma(intentosRestantes > 0 ? 600 : 1500);
    }

    void manejarComandosRemotos() {
        String mensaje = conexion.mensajeTopic();
        
        if (mensaje == "DESBLOQUEADO" && !desbloqueado) {
            pantalla.limpiar();
            pantalla.mostrarMensaje("Remoto:", 0, 0);
            pantalla.mostrarMensaje("Acceso concedido", 0, 1);
            desbloqueado = true;
            chapa.abrirPuerta();
            tiempoReferencia = millis();
            enEspera = true;
        } 
        else if (mensaje == "BLOQUEADO" && desbloqueado) {
            pantalla.limpiar();
            pantalla.mostrarMensaje("Remoto:", 0, 0);
            pantalla.mostrarMensaje("Puerta bloqueada", 0, 1);
            desbloqueado = false;
            chapa.cerrarPuerta();
            tiempoReferencia = millis();
            enEspera = true;
        }
    }

public:
    void iniciar() {
        pantalla.iniciar();
        conexion.conectarWiFi();
        mostrarMensajeInicial();
    }

    void mostrarMensajeInicial() {
        pantalla.limpiar();
        pantalla.mostrarMensaje("Presione # para", 0, 0);
        pantalla.mostrarMensaje("ingresar clave", 0, 1);
    }

    char leerTecla() { return teclado.leerTecla(); }

    void ingresarClave() {
        pantalla.limpiar();
        pantalla.mostrarMensaje("Ingrese clave:", 0, 0);
        claveIngresada = "";
        
        char tecla;
        while ((tecla = leerTecla()) != '#') {
            if (tecla) manejarEntradaTeclado(tecla);
        }
        verificarClave();
    }

    void verificarClave() {
        if (claveIngresada == CLAVE_CORRECTA) {
            concederAcceso();
        } else {
            denegarAcceso();
        }
        enEspera = true;
        tiempoReferencia = millis();
    }

    void actualizar() {
        if (enEspera && (millis() - tiempoReferencia >= TIEMPO_ESPERA)) {
            enEspera = false;
            if (!desbloqueado) {
                chapa.cerrarPuerta();
                publicarEstado("BLOQUEADO");
                intentosRestantes > 0 ? mostrarMensajeInicial() : mostrarBloqueo();
            }
        }
        manejarComandosRemotos();
        conexion.loop();
        buzzer.actualizar();
    }

    void mostrarBloqueo() {
        pantalla.limpiar();
        pantalla.mostrarMensaje("Cerradura", 0, 0);
        pantalla.mostrarMensaje("bloqueada", 0, 1);
    }

    bool isPuertaDesbloqueada() const { return desbloqueado; }
    Buzzer& getBuzzer() { return buzzer; }
};

#endif