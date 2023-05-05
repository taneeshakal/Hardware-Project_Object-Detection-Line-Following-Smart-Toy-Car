#define F_CPU 8000000 
#include <util/delay.h> 
#include <avr/io.h>  
#include <avr/interrupt.h>
#include <stdio.h>

uint8_t rchar;

int main(void)
{
	DDRC=0xFF;
	DDRA|=(0b1111<<4);
	
	/*Set baud rate */
	UBRRH = (unsigned char)(51>>8);
	UBRRL = (unsigned char)51;
	/*Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<RXCIE);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
	sei();
	
	while (1)
	{
		if (rchar==107)
		{PORTC|=(1<<0);
			rchar=0;
		}
	}
}

ISR(USART_RXC_vect){//F B L R s
	
	rchar=UDR;
	if (rchar)
	{
		switch(rchar){
			case 70:PORTA=(PORTA&0x0F)|(0b0101<<4);break;//fowrd
			case 66:PORTA=(PORTA&0x0F)|(0b1010<<4);break;//back
			case 76:PORTA=(PORTA&0x0F)|(0b1001<<4);break;//left
			case 82:PORTA=(PORTA&0x0F)|(0b0110<<4);break;//right
			case 83:PORTA=(PORTA&0x0F)|(0b0000<<4);break;//stop
		}
		_delay_ms(100);
		rchar=0;
	}
}

 