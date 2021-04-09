#include <stdint.h>
#include <pic12f617.h>

__code uint16_t __at(_CONFIG) __configword = _FOSC_INTOSCIO &
                                             _WDTE_OFF &
                                             _PWRTE_OFF &
                                             _MCLRE_OFF &
                                             _CP_OFF &
                                             _IOSCFS_4MHZ &
                                             _BOREN_ON &
                                             _WRT_OFF;

volatile uint8_t tick_reached;
volatile uint8_t sys_ticks = 0;

/* make VSCode happy */
#ifdef __SDCC
#define ISR __interrupt 0
#else
#define ISR
#endif

void isr(void) ISR
{
    if (TMR1IF == 1)
    {
        TMR1IF = 0;       /* clear timer1 interrupt flag */
        TMR1H = 0xD8;     /* ((55535 >> 8) & 0xff) 10ms */
        TMR1L = 0xEF;     /* ((55535 >> 0) & 0xff) 10ms */
        tick_reached = 1; /* set tick flag */
    }
}

int main(void)
{
    /* LED is on GP5 */
    TRISIO &= ~(1u << 5); /* config GP5 output */
    ANSEL &= ~(1u << 5);  /* config GP5 digital I/O */
    WPU &= ~(1u << 5);    /* disable GP5 pullup */

    /**
     * 10ms = 0.01s
     * (0.01 * (fOSC/4/pre)) = 65535 - TMR1
     * TMR1 = 65535 - (0.01 * (fOSC/4/pre))
     *      = 65535 - (0.01 * (4000000/4/1))
     *      = 55535
     */
    TMR1CS = 0;   /* Timer1 clock source INTOSC */
    T1CKPS1 = 0;  /* Timer1 prescale 1 */
    T1CKPS0 = 0;  /* Timer1 prescale 1 */
    TMR1H = 0xD8; /* ((55535 >> 8) & 0xff) 10ms */
    TMR1L = 0xEF; /* ((55535 >> 0) & 0xff) 10ms */
    TMR1ON = 1;   /* enable timer1 */
    TMR1IE = 1;   /* enable timer1 interrupt */

    PEIE = 1; /* enable peripheral interrupt */
    GIE = 1;  /* enable global interrupt */

    while (1)
    {
        if (tick_reached)
        {
            tick_reached = 0;
            sys_ticks++;
            if (sys_ticks == 50) /* blink the LED every 10 * 50 ms */
            {
                GPIO ^= (1u << 5); /* toggle GP5 output */
                sys_ticks = 0;
            }
        }
    }
}
