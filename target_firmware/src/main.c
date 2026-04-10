#include <util/delay.h>
#include <avr/io.h>
#include "serial_com.h"
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>

/* Exercise 7.2

int main()
{
    unsigned int counter = 0;
    // Initialize the UART for serial communication
    uart_init();
    // Set all pins of PORTB as output
    DDRB = 0xff;
    do
    {
        // Turn led on
        PORTB = 0xff;
        // Print message to serial

        printf("Counter: %d\n\r",counter++);
        // Wait 0,25 seconds and then turn led off again
        PORTB = 0x0;
        // Wait 1 second and then repeat loop
        _delay_ms(1000);
    } while (1);
}
    */

/* Exercise 7.3
int main()
{
// Initialize the UART for serial communication
uart_init();
// Buffer for reading input from serial/stdin
uint8_t input_buffer[100];
do
{
    // Print message to serial
    printf("Hallo from Arduino!\n\r");
    // Read input from serial/stdin and ignore it
    scanf("%s", input_buffer);
    printf("You wrote: %s, Arschloch!\n\r", input_buffer);
} while (1);
}
*/

/* Exercise 7.4
int main()
{

    // Initialize the UART for serial communication
    uart_init();
    DDRB = 0xff;
    // Buffer for reading input from serial/stdin
    int8_t input_buffer[100];

    do
    {
        // Turn led on

        printf("Write number:\n\r");

        scanf("%s", input_buffer);

        printf("You wrote: %s, Arschloch!\n\r", input_buffer);

        int times = atoi(input_buffer);

        for (int i = 0; i < times; i++)
        {
            PORTB = 0xff;
            _delay_ms(67);
            PORTB = 0x0;
            _delay_ms(67);
        }

        // Print message to serial

    } while (1);
}
    */

/* Exercise 7.5
int main()
{
    // Initialize the UART for serial communication
    uart_init();
    // Set prescaler to /1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
    while (1)
    {
        printf("TCNT1 %u\n\r", TCNT1);
        _delay_ms(500);
    }
}
    */

/* Exercise 7.6
#define PRESCALER 10        // Number of clock ticks to wait before timer firings. 10 bits...1024
#define CLOCK_RATE 16000000 // Clock value

// Number of timer ticks in a second. Number of clock ticks to wait until getting an interrupt
#define COUNTER_VALUE ((CLOCK_RATE >> PRESCALER) / 2) // Divide by 2 for twice a second (both LED_ON and LED_OFF in that second)
// TIMER1 compare Interrupt-Service-Routine
// fires when counter value matches
ISR(TIMER1_COMPA_vect)
{
PORTB ^= _BV(PORTB7); // Tells the compiler that this function is an ISR. Toggle output pin every time the timer fires. Toggle LED on PB7
}
int main()
{
// Initialize the UART for serial communication
uart_init();
// Buffer for reading input from serial/stdin
uint8_t input_buffer[100];

DDRB |= _BV(PORTB7); // Set PB7 as output

// Output-Compare-register for timer 1: Configure timer
OCR1A = COUNTER_VALUE; // Set compare match value (~1 second)

// Timer/Counter1 Control Register
TCCR1A = 0x00;                       // Normal mode
TCCR1B |= (1 << WGM12);              // CTC mode (clear timer on compare) -- compare to OCR1A. If match: clear counter and give interrupt
TCCR1B |= (1 << CS10) | (1 << CS12); // Set prescaler to 1024 (divide the clock by 1024: 16 MHz -> 16 KHz). To slow things down and wait a bit longer before counter overflows

// I want an interrupt: Set bit in register, otherwise flag is set in TIFR1 register and interrupt service won't run.
TIMSK1 |= (1 << OCIE1A); // Set interrupt mask bit

sei(); // Enable global interrupts

do
{
    // Print message to serial
    printf("Hallo from Arduino!\n\r");
    // Read input from serial/stdin and ignore it
    scanf("%s", input_buffer);
    printf("You wrote: %s, Arschloch!\n\r", input_buffer);
} while (1);
}
*/

/* Exercise 7.7
#define PRESCALER 1024
#define CLOCK_RATE 16000000

#define TIMER1_COUNTER_VALUE ((CLOCK_RATE / PRESCALER) / 20) // Task 1 blink the onboard LED with a frequency of 10 Hz.

volatile uint8_t button_tick = 0; // Task 2 checks a button every 50 ms and transmit a text message over the UART/stdout if the button is pressed.

// Task 1 ISR
ISR(TIMER1_COMPA_vect)
{
    PORTB ^= _BV(PORTB7);
}

// Task 2 ISR
ISR(TIMER0_COMPA_vect)
{
    button_tick++;
    if (button_tick >= 5) // 50 * 10 ms = 50 ms since TIMER0 fires approx every 10 ms
    {
        button_tick = 0;
        if (!(PINA & (1 << PINA0))) // Check if button is pressed (active low/pullup)
        {
            printf("BUTTON_PRESSED\n\r");
        }
    }
}

int main()
{
    uart_init();
    uint8_t input_buffer[100];

    DDRB |= _BV(PORTB7);    // LED
    DDRA &= ~(1 << DDA0);    // Button as Input
    PORTA |= (1 << PORTA0); // Enable pull-up for button

    // Task 1 (TIMER1)
    OCR1A = TIMER1_COUNTER_VALUE;

    TCCR1A = 0x00;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS10) | (1 << CS12);

    TIMSK1 |= (1 << OCIE1A);

    // Task 2 (TIMER0)
    OCR0A = ((CLOCK_RATE / 1024) * 0.01); // = 156

    TCCR0A = 0x00;
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS00) | (1 << CS02);

    TIMSK0 |= (1 << OCIE0A);

    sei();

    while (1)
    {
        // Task 3
        if (scanf("%s", input_buffer) == 1)
        {
            if (strcmp((char *)input_buffer, "BUTTON_PRESSED") == 0)
            {
                printf("Task 3 received the button trigger!\n\r");
            }
            else
            {
                printf("Echo: %s\n\r", input_buffer);
            }
        }
    }
}
    */

// Exercise 9.2
#include "blink.h"
void blink()
{
    PORTB ^= (1 << PB7);
    PORTA ^= (1 << PA1);
}

void send_text(){
    printf("Hello\r\n");
}

int main()
{
    uart_init();
    blink_init(1024, blink,send_text);
    sei();

    while (1)
    {
        ;
    }
}


/* Exercise 9.3
#include "scheduler.h"
#include "task_a.h"
#include "task_b.h"
#include "task_c.h"

task_t task_list[] =
{
  // period in ms, task to run, ready? (to run)
  {.period=100, .task_p=task_a_run, .ticks=0},
  {.period=1010, .task_p=task_b_run, .ticks=0},
  {.period=3000, .task_p=task_c_run, .ticks=0}
};
uint8_t task_count = sizeof(task_list)/sizeof(task_t);

int main()
{
  // Init
  uart_init();
  task_a_init();
  task_b_init();
  scheduler_init(task_list, task_count);
  sei();

  // Run
  while(1)
  {
    dispatcher();
  }
  return 1;
}
*/