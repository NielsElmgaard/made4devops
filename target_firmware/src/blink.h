#pragma once
#include <stdint.h>

typedef void (*blink_callback_t)();
typedef void (*send_text_callback_t)();


void blink_init(uint16_t frequency, blink_callback_t callback, send_text_callback_t text_callback);
