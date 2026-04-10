#include <avr/io.h>
#include "task_a.h"

void task_a_init()
{
    DDRB |= (1<<7);
}

void task_a_run()
{
    PORTB ^= (1<<7);
}