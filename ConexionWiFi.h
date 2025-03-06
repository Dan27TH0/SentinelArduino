// Librerias
#include <WiFi.h>
#include <HTTPClient.h>

class ConexionWiFi {
  public:
    const char* ssid = ""; // Nombre de tu red WiFi
    const char* password = ""; // Contraseña de tu red WiFi

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
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("\n❌ No se pudo conectar a WiFi.");
    }
  }
};
