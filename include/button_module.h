#ifndef BUTTON_MODULE_H
#define BUTTON_MODULE_H

#include <OneButton.h>
#include "led_module.h"
#include "config.h"

// Variáveis globais (se necessário)
extern OneButton touch_btn; 
// Protótipos das funções
void setup_buttons();
void loop_buttons();
void handleDoubleTap();
void handleTap();
void handleLongTapStart();
void handleDuringLongPress();


#endif // BUTTON_MODULE_H
