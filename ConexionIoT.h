#ifndef CONEXION_IOT_H
#define CONEXION_IOT_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

class ConexionIoT {
  private:
    // Configuración WiFi
    const char* ssid = "INTERMEXD2AF";
    const char* password = "n2z4dRpZDg";
    
    // Configuración EMQX Cloud
    const char* mqtt_server = "kfa70af4.ala.eu-central-1.emqxsl.com";
    const int mqtt_port = 8883; // Puerto seguro para MQTT
    const char* mqtt_user = "danohx";
    const char* mqtt_pass = "270105Dt";

    WiFiClientSecure espClient; // Cambiado a WiFiClientSecure para SSL
    PubSubClient client;
    String ultimoMensaje;

    void reconnect() {
      while (!client.connected()) {
        Serial.print("Intentando conexión MQTT...");
        if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
          Serial.println("conectado");
          client.subscribe("sentinel/estadoPuerta");
        } else {
          Serial.print("falló, rc=");
          Serial.print(client.state());
          Serial.println(" - Reintentando en 5 segundos");
          delay(5000);
        }
      }
    }

  public:
    ConexionIoT() : client(espClient) {
      // Configurar certificado raíz (opcional para EMQX Cloud)
      espClient.setInsecure(); // Omite verificación de certificado (solo para desarrollo)
      
      client.setServer(mqtt_server, mqtt_port);
      client.setCallback([this](char* topic, byte* payload, unsigned int length) {
        this->callback(topic, payload, length);
      });
    }

    void conectarWiFi() {
      Serial.print("Conectando a Wi-Fi...");
      WiFi.begin(ssid, password);

      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }

      Serial.println("\nConectado a Wi-Fi!");
      Serial.print("Dirección IP: ");
      Serial.println(WiFi.localIP());
    }

    void callback(char* topic, byte* payload, unsigned int length) {
      Serial.print("Mensaje recibido [");
      Serial.print(topic);
      Serial.print("]: ");

      ultimoMensaje = "";
      for (unsigned int i = 0; i < length; i++) {
        ultimoMensaje += (char)payload[i];
      }
      Serial.println(ultimoMensaje);
    }

    String mensajeTopic() const {
      return ultimoMensaje;
    }

    void publicar(const char* topic, const char* mensaje) {
      if (!client.connected()) {
        reconnect();
      }
      client.publish(topic, mensaje);
      Serial.print("Mensaje publicado [");
      Serial.print(topic);
      Serial.print("]: ");
      Serial.println(mensaje);
    }

    void loop() {
      if (!client.connected()) {
        reconnect();
      }
      client.loop();
    }
};

#endif