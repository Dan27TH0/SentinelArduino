#include "Cerradura.h"
#include "Iman.h"

Cerradura cerradura;
Iman iman(34, cerradura.getBuzzer());

void setup() {
    Serial.begin(115200);
    cerradura.iniciar();
    Serial.println("Sistema iniciado");
}

void loop() {
    char tecla = cerradura.leerTecla();
    
    if (!cerradura.isPuertaDesbloqueada()) {
        iman.actualizar();
    }

    if (tecla == '#') {
        cerradura.ingresarClave();
    }

    cerradura.actualizar();
    
    // Procesar tecla para desactivar alarma
    if (cerradura.getBuzzer().alarmaActivada() && tecla) {
        cerradura.getBuzzer().procesarTecla(tecla);
    }
}