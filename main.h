#include <16F886.h>
#device ADC=10
#use delay(internal=8000000)
#use timer(tick=100us,bits=32,NOISR)

#define TICK_TYPE unsigned int32
#use FIXED_IO( A_outputs=PIN_A7,PIN_A6 )
#use FIXED_IO( B_outputs=PIN_B1 )
#use FIXED_IO( C_outputs=PIN_C7,PIN_C6,PIN_C5,PIN_C4,PIN_C3,PIN_C2,PIN_C1,PIN_C0 )
#define LED9   PIN_A6
#define LED10   PIN_A7
#define TRIAC   PIN_B1
#define LED1   PIN_C0
#define LED2   PIN_C1
#define LED3   PIN_C2
#define LED4   PIN_C3
#define LED5   PIN_C4
#define LED6   PIN_C5
#define LED7   PIN_C6
#define LED8   PIN_C7


