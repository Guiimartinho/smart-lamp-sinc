#include "mqtt_module.h"

WiFiClient espClient;
PubSubClient client(espClient);

// Declare DEVICE_ID as a global String variable
String DEVICE_ID;

String mqtt_root_topic = MQTT_TOPIC_ROOT;
String mqtt_set_topic;
String mqtt_status_topic;
String mqtt_out_topic = mqtt_root_topic + "/" + PAIR_DEVICE_ID + "/set";
String mqttMessage;

void setup_mqtt()
{
  // Retrieve the unique identifier for the chip
  uint64_t chipid = ESP.getEfuseMac(); // Use appropriate method for your ESP model
  DEVICE_ID = String((uint32_t)(chipid >> 32), HEX) + String((uint32_t)chipid, HEX);

  // Now that DEVICE_ID is set, concatenate the other MQTT topics
  mqtt_set_topic = mqtt_root_topic + "/" + DEVICE_ID + "/set";
  mqtt_status_topic = mqtt_root_topic + "/" + DEVICE_ID + "/status";

  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(mqtt_callback);
  reconnect();
  Serial.println(DEVICE_ID);
  Serial.println(mqtt_status_topic);
  Serial.println(mqtt_set_topic);
  Serial.println(mqtt_out_topic);

  client.publish(mqtt_status_topic.c_str(), "init");
}

void loop_mqtt()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect(DEVICE_ID.c_str(), MQTT_USERNAME, MQTT_PASSWORD))
    {
      Serial.println("Connected to MQTT");
      // Publish initialization message
      client.subscribe(mqtt_set_topic.c_str());
    }
    else
    {
      Serial.print("Failed to connect to MQTT: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

/*********** MQTT Callback ***********/
void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  if (doc["state"].is<int>())
  {
    int newState = doc["state"];
    JsonObject color_doc = doc["color"];
    const char *color_type = color_doc["type"];
    JsonArray colors = color_doc["values"].as<JsonArray>();

    switch (newState)
    {
    // OFF
    case 0:
    {
      lampState = OFF;
      stateChange = true;
      break;
    }

    // SOLID
    case 1:
    {
      lampState = SOLID;
      JsonObject color = colors[0];

      if (strcmp(color_type, "rgb") == 0)
      {
        colorType = RGB_COLOR;
        color1[0] = color["r"];
        color1[1] = color["g"];
        color1[2] = color["b"];
      }
      else if (strcmp(color_type, "hsv") == 0)
      {
        colorType = HSV_COLOR;
        color1[0] = color["h"];
        color1[1] = color["s"];
        color1[2] = color["v"];
      }
      stateChange = true;
      break;
    }

    // STRIPES
    case 2:
    {
      lampState = STRIPES;
      JsonObject c1 = colors[0];
      JsonObject c2 = colors[1];

      if (strcmp(color_type, "rgb") == 0)
      {
        colorType = RGB_COLOR;
        color1[0] = c1["r"];
        color1[1] = c1["g"];
        color1[2] = c1["b"];
        color2[0] = c2["r"];
        color2[1] = c2["g"];
        color2[2] = c2["b"];
      }
      else if (strcmp(color_type, "hsv") == 0)
      {
        colorType = HSV_COLOR;
        color1[0] = c1["h"];
        color1[1] = c1["s"];
        color1[2] = c1["v"];
        color2[0] = c2["h"];
        color2[1] = c2["s"];
        color2[2] = c2["v"];
      }
      stateChange = true;
      break;
    }

    // GRADIENT
    case 3:
    {
      lampState = GRADIENT;
      JsonObject c1 = colors[0];
      JsonObject c2 = colors[1];

      if (strcmp(color_type, "rgb") == 0)
      {
        colorType = RGB_COLOR;
        color1[0] = c1["r"];
        color1[1] = c1["g"];
        color1[2] = c1["b"];
        color2[0] = c2["r"];
        color2[1] = c2["g"];
        color2[2] = c2["b"];
      }
      else if (strcmp(color_type, "hsv") == 0)
      {
        colorType = HSV_COLOR;
        color1[0] = c1["h"];
        color1[1] = c1["s"];
        color1[2] = c1["v"];
        color2[0] = c2["h"];
        color2[1] = c2["s"];
        color2[2] = c2["v"];
      }
      stateChange = true;
      break;
    }

    // MOVING GRADIENT
    case 4:
    {
      lampState = MOVING_GRADIENT;
      JsonObject c1 = colors[0];
      JsonObject c2 = colors[1];

      if (strcmp(color_type, "rgb") == 0)
      {
        colorType = RGB_COLOR;
        color1[0] = c1["r"];
        color1[1] = c1["g"];
        color1[2] = c1["b"];
        color2[0] = c2["r"];
        color2[1] = c2["g"];
        color2[2] = c2["b"];
        fill_gradient_RGB(gradient, 0, CRGB(color1[0], color1[1], color1[2]), GRADIENT_LENGTH - 1, CRGB(color2[0], color2[1], color2[2]));
      }
      else if (strcmp(color_type, "hsv") == 0)
      {
        colorType = HSV_COLOR;
        color1[0] = c1["h"];
        color1[1] = c1["s"];
        color1[2] = c1["v"];
        color2[0] = c2["h"];
        color2[1] = c2["s"];
        color2[2] = c2["v"];
        fill_gradient(gradient, 0, CHSV(color1[0], color1[1], color1[2]), GRADIENT_LENGTH - 1, CHSV(color2[0], color2[1], color2[2]), SHORTEST_HUES);
      }

      stateChange = true;
      break;
    }

    // BREATHE
    case 5:
    {
      lampState = BREATHE;
      JsonObject color = colors[0];

      if (strcmp(color_type, "rgb") == 0)
      {
        colorType = RGB_COLOR;
        color1[0] = color["r"];
        color1[1] = color["g"];
        color1[2] = color["b"];
      }
      else if (strcmp(color_type, "hsv") == 0)
      {
        colorType = HSV_COLOR;
        color1[0] = color["h"];
        color1[1] = color["s"];
        color1[2] = (color["v"] == 0) ? 10 : color["v"];
      }
      stateChange = true;
      break;
    }

    // RAINBOW
    case 6:
    {
      lampState = RAINBOW;
      stateChange = true;
      break;
    }

    default:
    {
      stateChange = false;
      break;
    }
    }
  }
}
