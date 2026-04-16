#include <avr/io.h>
#include "task_e.h"

void task_e_init()
{
    DDRA |= (1 << PA1);
}

void task_e_run()
{
    PORTA ^= (1 << PA1);
}