#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

// Inclua aqui outras configurações necessárias do FreeRTOS...

// Habilitar a verificação de estouro de pilha
#define configCHECK_FOR_STACK_OVERFLOW 2

// Incluir o protótipo da função de gancho para estouro de pilha
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);

// Restante das configurações do FreeRTOS...

#endif // FREERTOS_CONFIG_H
