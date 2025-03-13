// Librerias
#include <WiFi.h>
#include <HTTPClient.h>

class ConexionWiFi {
  public:
    const char* ssid = "NETWEY-DB-2.4G"; // Nombre de tu red WiFi
    const char* password = "NetWey2023"; // Contraseña de tu red WiFi

  // Método para conectar a WiFi con un tiempo límite de espera
  void conectarWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Conectando a WiFi");

    int intentos = 0;
    while (WiFi.status() != WL_CONNECTED && intentos < 20) { // Máximo 10 segundos de espera
      delay(500);
      Serial.print(".");
      intentos++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n✅ Conectado a WiFi!");
    } else {
      Serial.println("\n❌ No se pudo conectar a WiFi.");
      // Aquí podrías activar un modo offline o reintentar la conexión.
    }
  }
};
