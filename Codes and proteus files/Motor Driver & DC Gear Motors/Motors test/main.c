#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRA|=(0b1111<<4);
	
	PORTA=(PORTA&0x0F)|(0b0101<<4);//forward
	_delay_ms(2000);
	PORTA=(PORTA&0x0F)|(0b1010<<4);//reverse
	_delay_ms(2000);
	PORTA=(PORTA&0x0F)|(0b1001<<4);//left
	_delay_ms(2000);
	PORTA=(PORTA&0x0F)|(0b0110<<4);//right
	while (1)
	{
	}
}
