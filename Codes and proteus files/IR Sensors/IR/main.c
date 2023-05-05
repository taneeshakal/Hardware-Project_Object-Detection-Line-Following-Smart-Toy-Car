#define F_CPU 8000000 
#include <util/delay.h> 
#include <avr/io.h> 
#include <avr/interrupt.h>

int main(void)
{
	/*IR Remote*/
	DDRA&=~((1<<0)|(1<<1));//ir as input
	DDRA|=(0b1111<<4);//motor
	
	
	while (1)
	{
		if (PINA&(1<<0))
		{
			PORTA=(PORTA&0x0F)|(0b0110<<4);//right
		}
		
		else if((PINA&(1<<1)))
		{
			PORTA=(PORTA&0x0F)|(0b1001<<4);//left
		}
		
		else
		{
			PORTA=(PORTA&0x0F)|(0b0101<<4);//go
		}
	}
}



