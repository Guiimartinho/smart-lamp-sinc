#ifndef OTA_LAMP_H
#define OTA_LAMP_H

#include "wifi_module.h"
#include "mqtt_module.h"
#include "Update.h"

extern long contentLength;
extern bool isValidContentType;
extern String host;
extern int port;
extern String bin;

extern WiFiClient espClient;

String getHeaderValue(String header, String headerName);

void execOTA();

#endif //   OTA_LAMP_H
