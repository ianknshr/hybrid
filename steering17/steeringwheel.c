
#include<avr/io.h>
#include<avr/interrupt.h>

#define FOSC 8000000UL
#define BAUD 9600UL
#define BAUD_PRESCALE FOSC/16/BAUD-1

#define F_CPU 8000000UL
#include<util/delay.h>

void usart_init(unsigned int ubrr)
{
    // set baud rate
    UBRR0H = (unsigned char) (ubrr>>8);
    UBRR0L = (unsigned char) ubrr;
    // enable receiver transmiter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // set frame format: 8data, 2 stop bit
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void serial_send(unsigned char data)
{
    // wait for empty transmit buffer
    while(!(UCSR0A & (1<<UDRE0)));
    
    // send
    UDR0 = data;
}

// OCR1A controls frequency
void setup_timer_interrupt()
{
    TCCR1A &= ~(1<<COM1A1);    // disconnect pin OC1A
    TCCR1A &= ~(1<<COM1A0);
    TCCR1A &= ~(1<<COM1B1);    // disconnect pin OC1B
    TCCR1A &= ~(1<<COM1B0);
    TCCR1A &= ~((1<<WGM10)|(1<<WGM11));    // setup proper behavior
    TCCR1B |= (1<<WGM12);
    TCCR1B &= ~(1<<WGM13);
    TCCR1B |= (1<<CS12);    // set prescalar to 256
    TCCR1B &= ~((1<<CS11)|(1<<CS10));
    TIMSK1 |= (1<<OCIE1A);    // trigger interrupt on compare match A
    OCR1A = 1562; // ~50 ms
}

// send status packet
ISR(TIMER1_COMPA_vect)
{
    serial_send(PINC);
}

//volatile int int_detected = 0;
ISR(PCINT1_vect)
{
    //int_detected = 1;
    //_delay_ms(10);
    serial_send(PINC);
    //_delay_ms(50);
}

int main()
{

    // setup
    DDRC &= ~(1<<PC0);
    DDRC &= ~(1<<PC1);
    DDRC &= ~(1<<PC2); // setup as input
    DDRC &= ~(1<<PC3);
    DDRC &= ~(1<<PC4);
    DDRC &= ~(1<<PC5); // setup as input

    // set clock prescalar to 1
    CLKPR &= ~(1<<CLKPS0);
    CLKPR &= ~(1<<CLKPS1);
    CLKPR &= ~(1<<CLKPS2);
    CLKPR &= ~(1<<CLKPS3);

    // enable interupts on PCINT[8:14]
    PCICR |= (1<<PCIE1);
    PCMSK1 |= (1<<PCINT9)|(1<<PCINT10);
    PCMSK1 |= (1<<PCINT11)|(1<<PCINT13);
    PCMSK1 |= (1<<PCINT8);	// ev boost button
    PCMSK1 &= ~(1<<PCINT12);
    PCMSK1 &= ~(1<<PCINT14);

    usart_init(BAUD_PRESCALE);
    setup_timer_interrupt();

    // enable global interupts
    sei();   

    // execution loop
    while(1)
    {
        /*if(int_detected)
        {
            _delay_ms(10);
            serial_send(PINC);
            _delay_ms(50);
            int_detected = 0;
        }*/
    }

    return 0;
}
