#pragma once
#include <stdint.h>

typedef void (*send_text_callback_t)();


void send_text_init(uint16_t frequency, send_text_callback_t callback);
