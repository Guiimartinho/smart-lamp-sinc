#include "main.h"

// Variáveis globais
SemaphoreHandle_t mutex_leds;  // Semáforo para proteger o acesso aos LEDs

// Protótipos das funções das tasks
void TaskWifi(void *pvParameters);
void TaskMQTT(void *pvParameters);
void TaskLEDs(void *pvParameters);
void TaskButtons(void *pvParameters);
void TaskOTA(void *pvParameters);
void TaskMonitorStack(void *pvParameters);

// Função de gancho para estouro de pilha
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    Serial.print("Estouro de pilha na tarefa: ");
    Serial.println(pcTaskName);

    // Inicialização dos LEDs
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
      
    // Loop de piscar LEDs em vermelho
    while (1) {
        blinkRedLEDs(3, 2000);  // Piscar 3 vezes a cada 2 segundos

        // Aguardar 1 minuto
        for (int i = 0; i < 30; i++) {  // 30 iterações de 2 segundos cada
            delay(2000);
        }

        // Reiniciar o sistema
        ESP.restart();  // Para ESP32 ou ESP8266
    }
}

void setup()
{
    Serial.begin(115200);
    setup_wifi();
    setup_mqtt();
    setup_leds();
    setup_buttons();
    execOTA();

    // Criação do semáforo para proteger o acesso aos LEDs
    mutex_leds = xSemaphoreCreateMutex();

    // Criação das tasks do FreeRTOS
    xTaskCreatePinnedToCore(TaskWifi, "TaskWifi", 1000, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(TaskMQTT, "TaskMQTT", 1000, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(TaskLEDs, "TaskLEDs", 1000, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(TaskButtons, "TaskButtons", 1000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(TaskOTA, "TaskOTA", 1000, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(TaskMonitorStack, "TaskMonitorStack", 1000, NULL, 1, NULL, 0);
}

// Implementações das funções das tasks
void TaskWifi(void *pvParameters)
{
    UBaseType_t uxHighWaterMark;
    for (;;)
    {
        loop_wifi();
        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        Serial.print("Espaço mínimo restante na pilha da Task-Wifi: ");
        Serial.println(uxHighWaterMark);
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay de 100ms
    }
}

void TaskOTA(void *pvParameters)
{
    UBaseType_t uxHighWaterMark;
    for (;;)
    {
        // OTA Metod
        execOTA();
        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        Serial.print("Espaço mínimo restante na pilha da Task-OTA: ");
        Serial.println(uxHighWaterMark);
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay de 100ms
    }
}

void TaskMQTT(void *pvParameters)
{
    UBaseType_t uxHighWaterMark;
    for (;;)
    {
        loop_mqtt();
        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        Serial.print("Espaço mínimo restante na pilha da Task-MQTT: ");
        Serial.println(uxHighWaterMark);
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay de 100ms
    }
}

void TaskLEDs(void *pvParameters)
{
    UBaseType_t uxHighWaterMark;
    for (;;)
    {
        // Adquire o semáforo para proteger o acesso aos LEDs
        xSemaphoreTake(mutex_leds, portMAX_DELAY);

        loop_leds();

        // Libera o semáforo
        xSemaphoreGive(mutex_leds);

        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        Serial.print("Espaço mínimo restante na pilha da Task-LEDS: ");
        Serial.println(uxHighWaterMark);
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay de 100ms
    }
}

void TaskButtons(void *pvParameters)
{
    UBaseType_t uxHighWaterMark;
    for (;;)
    {
        loop_buttons();
        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        Serial.print("Espaço mínimo restante na pilha da Task-BUTTON: ");
        Serial.println(uxHighWaterMark);
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay de 100ms
    }
}

void TaskMonitorStack(void *pvParameters)
{
    UBaseType_t uxHighWaterMark;
    for (;;)
    {
        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        Serial.print("Stack High Water Mark for TaskMonitorStack: ");
        Serial.println(uxHighWaterMark);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void loop()
{
    // No FreeRTOS, o loop pode ser vazio ou utilizado para operações de baixa prioridade
}