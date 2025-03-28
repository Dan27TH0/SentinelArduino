#include <SPI.h>
#include <MFRC522.h>

class LectorRFID {
private:
    MFRC522 mfrc522; // Instancia del lector RFID
    bool tarjetaPresente = false;
    String ultimaUID = "";
    unsigned long ultimaLectura = 0;
    const unsigned long intervaloAntiRebote = 1000; // Tiempo en ms entre lecturas

public:
    /**
     * @brief Constructor de la clase LectorRFID
     * @param ssPin Pin SS/CS del módulo RC522
     * @param rstPin Pin RST del módulo RC522
     */
    LectorRFID(uint8_t ssPin, uint8_t rstPin) : mfrc522(ssPin, rstPin) {}

    /**
     * @brief Inicializa el lector RFID
     */
    void iniciar() {
        SPI.begin(); // Inicia bus SPI
        mfrc522.PCD_Init(); // Inicia el lector RFID
        delay(4); // Pequeño retardo para inicialización
        Serial.println("Lector RFID inicializado");
    }

    /**
     * @brief Verifica si hay una tarjeta presente
     * @return true si hay una tarjeta, false en caso contrario
     */
    bool hayTarjeta() {
        // Verifica si hay una tarjeta nueva presente
        if (!mfrc522.PICC_IsNewCardPresent()) {
            tarjetaPresente = false;
            return false;
        }
        
        // Verifica si podemos leer la UID de la tarjeta
        if (!mfrc522.PICC_ReadCardSerial()) {
            tarjetaPresente = false;
            return false;
        }
        
        tarjetaPresente = true;
        return true;
    }

    /**
     * @brief Obtiene la UID de la tarjeta presente
     * @return String con la UID en formato hexadecimal
     */
    String obtenerUID() {
        if (!tarjetaPresente) return "";

        // Prevención de rebotes - evita múltiples lecturas en corto tiempo
        unsigned long ahora = millis();
        if (ahora - ultimaLectura < intervaloAntiRebote && ultimaUID != "") {
            return ultimaUID;
        }

        String uid = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            // Convierte cada byte a hexadecimal y lo concatena
            if (mfrc522.uid.uidByte[i] < 0x10) {
                uid += "0";
            }
            uid += String(mfrc522.uid.uidByte[i], HEX);
        }
        uid.toUpperCase();

        ultimaUID = uid;
        ultimaLectura = ahora;
        return uid;
    }

    /**
     * @brief Detiene la comunicación con la tarjeta
     */
    void detener() {
        mfrc522.PICC_HaltA(); // Detiene la comunicación con la tarjeta
        mfrc522.PCD_StopCrypto1(); // Detiene cualquier cifrado activo
    }

    /**
     * @brief Muestra información de la tarjeta por Serial
     */
    void mostrarInformacion() {
        if (!tarjetaPresente) return;
        
        Serial.println("--- Información de la tarjeta ---");
        Serial.print("UID: ");
        String uid = obtenerUID();
        Serial.println(uid);
        
        // Muestra el tipo de tarjeta (corregido el tipo de dato)
        MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
        Serial.print("Tipo: ");
        Serial.println(mfrc522.PICC_GetTypeName(piccType));
        
        Serial.println("--------------------------------");
    }
};