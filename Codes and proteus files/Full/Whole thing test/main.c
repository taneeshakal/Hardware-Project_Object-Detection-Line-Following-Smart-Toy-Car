#define F_CPU 8000000 
#include <util/delay.h> 
#include <avr/io.h>  
#include <avr/interrupt.h>

volatile uint16_t TimerCal=0;
uint16_t ultraINT0=0;
volatile uint8_t rchar; 
int main(void)
{
   /*IR Remote*/
   DDRA&=~((1<<0)|(1<<1));
   DDRC=0xFF;
   DDRA|=(0b1111<<4);
    
    DDRD|=(1<<3); 
	TCCR0|=(1<<WGM01);
	TCCR0|=(1<<CS01);
	TCNT0=0;
	OCR0=10;
	
	TIMSK|=(1<<OCIE0);
	
	
	/*Set baud rate */
	UBRRH = (unsigned char)(51>>8);
	UBRRL = (unsigned char)51;
	
	UCSRB = (1<<RXEN)|(1<<RXCIE);
	
	UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);
	
	sei();
   
    while (1) 
    {
		PORTD&=~(1<<3);
		_delay_us(50);
		PORTD|=(1<<3);
		_delay_us(50);
		PORTD&=~(1<<3);
		
		
		while(!(PIND&(1<<2)))
		TimerCal=0;
		while((PIND&(1<<2)))
		ultraINT0=TimerCal;
		
		
		if (ultraINT0<83)
		{
			PORTC|=(1<<7);
			PORTA=(PORTA&0x0F)|(0b0000<<4);
		}
		
		else
		{
		    if (PINA&(1<<0))  
		        {
			        PORTA=(PORTA&0x0F)|(0b0110<<4);
		            PORTC^=(1<<0);
		        }
				
		    else if((PINA&(1<<1)))
		        {
		            PORTA=(PORTA&0x0F)|(0b1001<<4);
		            PORTC^=(1<<1); 
		        }
				
		    else
		        {
		            PORTC&=~(1<<7);	
		            PORTA=(PORTA&0x0F)|(0b0101<<4);
		            PORTC&=~(0b11<<0);
		        }
		
		}
		
		
		_delay_ms(100);
    }
}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCal++;
	TCNT0=0;
	
}

ISR(USART_RXC_vect){//F B L R s
	
	rchar=UDR;
	if (rchar)
	{
		switch(rchar){
		case 70:PORTA=(PORTA&0x0F)|(0b0101<<4);break;//foward
		case 66:PORTA=(PORTA&0x0F)|(0b1010<<4); break;//back
		case 76:PORTA=(PORTA&0x0F)|(0b1001<<4);PORTC^=(1<<1);break;//left
		case 82:PORTA=(PORTA&0x0F)|(0b0110<<4);PORTC^=(1<<0);break;//right
		case 83:PORTA=(PORTA&0x0F)|(0b0000<<4);break;//stop
		}
		_delay_ms(100);
		rchar=0;
	}
}

