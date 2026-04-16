#pragma once
#include <stdint.h>
#include <stdbool.h>

#define MS_PER_TICK    1 // sættes til 67. Resultat: Da frequency = 1000 / 67 = 14,925..., bliver det rundet ned til 14 (uoverensstemmelse i systemet), task_a blinker langsommere

typedef struct
{
  uint32_t period;  // delay in ms between each run
  void (*task_p)(); // pointer to task
  uint32_t ticks;   // when this gets to zero, it's time to run
} task_t;

void scheduler_init(task_t tasks[], uint8_t count );
void dispatcher();
