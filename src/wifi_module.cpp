#include "wifi_module.h"

// Aqui você coloca as definições de WIFI_SSID_1, WIFI_PASS_1, etc.
// e implementa setup_wifi() e loop_wifi() com o código relevante.
#include "wifi_module.h"
#include "config.h"

WiFiMulti wifiMulti;

void setup_wifi() {
  wifiMulti.addAP(WIFI_SSID_1, WIFI_PASS_1);
  wifiMulti.addAP(WIFI_SSID_2, WIFI_PASS_2);

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop_wifi() {
  // Aqui você pode adicionar qualquer lógica que precisa ser executada em cada ciclo do loop
}
