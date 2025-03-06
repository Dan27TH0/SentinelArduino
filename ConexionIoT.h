// Librerias
#include "ConexionWiFi.h"

// Conecta el circuito con la app y la web
class ConexionIoT{
  private:
    ConexionWiFi conexion;
    const char* serverUrl = "";
    unsigned long previousMillis;
    const long interval;
  
  public:
    ConexionIoT() : interval(3000) {
        conexion.conectarWiFi();
    }
  
  void enviarEstadoPuerta(String estado){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if (WiFi.status() == WL_CONNECTED) {
          HTTPClient http;

          String postData = "estado=" + estado;
          // Inicia la conexión HTTP
          http.begin(serverUrl);
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");

          http.POST(postData);
          http.end();
      }
    }
  }

  String recibirEstadoChapa(){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(serverUrl) + "";
        http.begin(url);
        http.GET();
        String respuesta = http.getString();
        http.end();
        return respuesta;
      }
    }
  }
};