// Calculator test with fakes

#include "fff.h"
#include "unity.h"
#include "serial_com.h"
#include <util/delay.h>
#include <avr/io.h>

#include "calculator.h"

DEFINE_FFF_GLOBALS;

struct calctask_t
{
    int32_t operand1;
    int32_t operand2;
    int32_t result;
} calctask;

FAKE_VALUE_FUNC(int, add_op, struct calctask_t *);
FAKE_VALUE_FUNC(int, mul_op, struct calctask_t *);
FAKE_VALUE_FUNC(int, div_op, struct calctask_t *);

void setUp(void)
{
    static int uart_initialized = 0;

    if (!uart_initialized)
    {
        uart_init();
        uart_initialized = 1;
        DDRB = 0xff;
        for (int i = 0; i < 20; i++)
        {
            PORTB = 0xff;
            _delay_ms(50.0);
            PORTB = 0x0;
            _delay_ms(50.0);
        }
    }
    RESET_FAKE(add_op);
    RESET_FAKE(mul_op);
    RESET_FAKE(div_op);
    FFF_RESET_HISTORY();
}

void test_fff(void)
{
    add_op_fake.return_val = 3;
    int c = add_op(&calctask);
    TEST_ASSERT_EQUAL(3, c);
    TEST_ASSERT_TRUE(add_op_fake.call_count == 1);
}

void test_mul_op_67_42_Returns_2814(void)
{
    mul_op_fake.return_val = 2814;
    int c = mul_op(&calctask);
    TEST_ASSERT_EQUAL(2814, c);
    TEST_ASSERT_TRUE(mul_op_fake.call_count == 1);
}

void test_div_op_67_42_Returns_1(void)
{
    div_op_fake.return_val = 1;
    int c = div_op(&calctask);
    TEST_ASSERT_EQUAL(1, c);
    TEST_ASSERT_TRUE(div_op_fake.call_count == 1);
}