#include "blink.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "blink.h"

static blink_callback_t cb = NULL;
static send_text_callback_t tcb = NULL;

void blink_init(uint16_t frequency, blink_callback_t callback, send_text_callback_t text_callback)
{
  cb = callback;
  tcb = text_callback;

  if (frequency == 0)
  {
    return;
  }

  // Init
  DDRA |= (1 << PA1);
  DDRB |= (1 << PB7);
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12); // Prescaler: F_CPU/256, CTC mode
  TIMSK1 = (1 << OCIE1A);              // Enable Timer Compare match interrupt
  OCR1A = F_CPU / (2 * 256 * frequency);

  cb = callback;

  // Init send_text
  TCCR1B = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12); // Prescaler: F_CPU/256, CTC mode
  TIMSK1 = (1 << OCIE1B);              // Enable Timer Compare match interrupt
  OCR1B = F_CPU / (2 * 256 * frequency);

  tcb = text_callback;
}

ISR(TIMER1_COMPA_vect)
{
  if (NULL != cb)
  {
    cb();
  }
}

ISR(TIMER1_COMPB_vect)
{
  if (NULL != tcb)
  {
    tcb();
  }
}