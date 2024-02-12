#include "button_module.h"
#include "config.h"
#include "led_module.h"
#include "mqtt_module.h"
#include "wifi_module.h"

OneButton touch_btn = OneButton(BUTTON_PIN, false, false);
int currentPattern = 0;

void setup_buttons() {
    // ...outras inicializações...

    // Configuração do botão
    touch_btn.setClickMs(200);
    touch_btn.setPressMs(500);
    touch_btn.attachClick(handleTap);
    touch_btn.attachDoubleClick(handleDoubleTap);
    touch_btn.attachLongPressStart(handleLongTapStart);
    touch_btn.attachDuringLongPress(handleDuringLongPress);
    pinMode(BUTTON_PIN, INPUT);
}

void loop_buttons(){

}

void handleDoubleTap() {
    static bool lightToggle = false;

  // Turn off lamp if on
  if (lampState != OFF)
  {
    lightToggle = true;
  }

  // Toggle Lamp
  if (lightToggle)
  {
    lampState = OFF;
    lightToggle = false;
    stateChange = true;
  }
  else
  {
    lampState = SOLID;
    colorType = HSV_COLOR;
    color1[0] = 0;
    color1[1] = 0;
    color1[2] = 255;
    lightToggle = true;
    stateChange = true;
  }
}

void handleTap() {
  
  Serial.println("Tapped!");
  StaticJsonDocument<256> doc;

  doc["state"] = lampState;

  JsonObject color = doc.createNestedObject("color");
  JsonArray color_values = color.createNestedArray("values");
  JsonObject color_values_0 = color_values.createNestedObject();
  JsonObject color_values_1 = color_values.createNestedObject();
  if (colorType == RGB_COLOR)
  {
    color["type"] = "rgb";
    color_values_0["r"] = color1[0];
    color_values_0["g"] = color1[1];
    color_values_0["b"] = color1[2];
    color_values_1["r"] = color2[0];
    color_values_1["g"] = color2[1];
    color_values_1["b"] = color2[2];
  }
  else if (colorType == HSV_COLOR)
  {
    color["type"] = "hsv";
    color_values_0["h"] = color1[0];
    color_values_0["s"] = color1[1];
    color_values_0["v"] = color1[2];
    color_values_1["h"] = color2[0];
    color_values_1["s"] = color2[1];
    color_values_1["v"] = color2[2];
  }

  mqttMessage = "";
  serializeJson(doc, mqttMessage);
  bool success = client.publish(mqtt_out_topic.c_str(), mqttMessage.c_str());
  if (success)
  {
    Serial.println("Successfully sent!");
  }
  else
  {
    Serial.println("Failed to send...");
  }
}

void handleLongTapStart() {
    Serial.println("Long Tap Start!");
  currentPattern = 0;
  FastLED.clear();
  FastLED.show();
}

void handleDuringLongPress() {
    static long lastPressed = 0;
  long duration = 800;
  long nextTime = lastPressed + duration;
  if (millis() >= nextTime)
  {
    if (touch_btn.isLongPressed())
    {
      Serial.println("During Long Tap!");
      Serial.print("currentPattern:");
      Serial.println(currentPattern);
      currentPattern++;
      switch (currentPattern)
      {

      // Red
      case 1:
        lampState = SOLID;
        colorType = HSV_COLOR;
        color1[0] = 4;
        color1[1] = 255;
        color1[2] = 180;
        stateChange = true;
        break;
      // Orange
      case 2:
        lampState = SOLID;
        colorType = HSV_COLOR;
        color1[0] = 11;
        color1[1] = 255;
        color1[2] = 180;
        stateChange = true;
        break;
      // Yellow
      case 3:
        lampState = SOLID;
        colorType = HSV_COLOR;
        color1[0] = 40;
        color1[1] = 255;
        color1[2] = 180;
        stateChange = true;
        break;
      // Green
      case 4:
        lampState = SOLID;
        colorType = HSV_COLOR;
        color1[0] = 78;
        color1[1] = 255;
        color1[2] = 180;
        stateChange = true;
        break;
      // Blue
      case 5:
        lampState = SOLID;
        colorType = HSV_COLOR;
        color1[0] = 160;
        color1[1] = 255;
        color1[2] = 180;
        stateChange = true;
        break;
      // Pink
      case 6:
        lampState = SOLID;
        colorType = HSV_COLOR;
        color1[0] = 225;
        color1[1] = 255;
        color1[2] = 180;
        stateChange = true;
        break;
      // Purple
      case 7:
        lampState = SOLID;
        colorType = HSV_COLOR;
        color1[0] = 200;
        color1[1] = 255;
        color1[2] = 180;
        stateChange = true;
        break;
      // White
      case 8:
        lampState = SOLID;
        colorType = HSV_COLOR;
        color1[0] = 0;
        color1[1] = 0;
        color1[2] = 180;
        stateChange = true;
        currentPattern = 0;
        break;
      }
      lastPressed = millis();
    }
  }
}
