#include "send_text.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "send_text.h"

static send_text_callback_t cb = NULL;

void send_text_init(uint16_t seconds, send_text_callback_t callback)
{
  if (seconds == 0)
  {
    return;
  }

  TCCR3A = 0;

  TCCR3B = (1 << WGM32) | (1 << CS32) | (1 << CS30); // CTC, 1024 Prescaler

  TCCR3B |= (1 << CS32) | (1 << CS30);

  OCR3A = (uint16_t)((F_CPU / 1024UL) * seconds - 1);

  TIMSK3 = (1 << OCIE3A);
  cb = callback;
}

ISR(TIMER3_COMPA_vect)
{
  if (NULL != cb)
  {
    cb();
  }
}
