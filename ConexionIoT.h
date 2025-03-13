#include <WiFi.h>
#include <HTTPClient.h>

class ConexionIoT {
  private:
    const char* ssid = "Redmi Note 12";
    const char* password = "1q2w3e4r5t6y";
    const char* serverUrl = "http://192.168.151.46:3000";
    unsigned long previousMillisEstado;
    unsigned long previousMillisChapa;
    const long interval;

  public:
    ConexionIoT() : interval(3000), previousMillisEstado(0), previousMillisChapa(0) {
        conectarWiFi();
    }

    void conectarWiFi() {
        WiFi.begin(ssid, password);
        Serial.print("Conectando a WiFi");

        int intentos = 0;
        while (WiFi.status() != WL_CONNECTED && intentos < 20) {
            delay(500);
            Serial.print(".");
            intentos++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\n✅ Conectado a WiFi!");
        } else {
            Serial.println("\n❌ No se pudo conectar a WiFi.");
        }
    }

    void obtenerEstadoPuerta(String estado) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillisEstado >= interval) {
            previousMillisEstado = currentMillis;

            if (WiFi.status() == WL_CONNECTED) {
                HTTPClient http;
                String url = String(serverUrl) + "/estado";
                http.begin(url);
                http.addHeader("Content-Type", "application/x-www-form-urlencoded");

                String postData = "estado=" + estado;
                int responseCode = http.POST(postData);

                Serial.print("Estado enviado: ");
                Serial.println(responseCode);

                http.end();
            } else {
                Serial.println("Error: No hay conexión WiFi");
            }
        }
    }

    String recibirEstadoChapa() {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillisChapa >= interval) {
            previousMillisChapa = currentMillis;

            if (WiFi.status() == WL_CONNECTED) {
                HTTPClient http;
                String url = String(serverUrl) + "/estado";
                http.begin(url);

                int httpResponseCode = http.GET();
                if (httpResponseCode > 0) {
                    String respuesta = http.getString();
                    Serial.println("Estado recibido: " + respuesta);
                    http.end();
                    return respuesta;
                } else {
                    Serial.print("Error en GET: ");
                    Serial.println(httpResponseCode);
                }
                http.end();
            } else {
                Serial.println("Error: No hay conexión WiFi");
                conectarWiFi();
            }
        }
        return "ERROR";
    }
};
