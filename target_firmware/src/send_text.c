#include "send_text.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "send_text.h"

static send_text_callback_t cb = NULL;

void send_text_init(uint16_t frequency, send_text_callback_t callback)
{
  if (frequency == 0)
  {
    return;
  }

  // Init

  // TCCRx - Timer/Counter Control Register. The pre-scaler can be configured here.
  // TCNTx - Timer/Counter Register. The actual timer value is stored here.
  // OCRx - Output Compare Register
  // ICRx - Input Capture Register (only for 16bit timer)
  // TIMSKx - Timer/Counter Interrupt Mask Register. To enable/disable timer interrupts.
  // TIFRx - Timer/Counter Interrupt Flag Register. Indicates a pending timer interrupt.

  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // Prescaler: 1024
  OCR1A = (uint16_t)((F_CPU / 1024) * frequency) - 1;
  TIMSK1 = (1 << OCIE1A); // Enable Timer Compare match interrupt

  cb = callback;
}

ISR(TIMER1_COMPA_vect)
{
  if (NULL != cb)
  {
    cb();
  }
}
