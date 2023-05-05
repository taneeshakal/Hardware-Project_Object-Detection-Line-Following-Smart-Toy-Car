#define F_CPU 8000000
#include <avr/io.h>


int main(void)
{
	DDRC=0xFF;
	PORTC=0xFF;
	while (1)
	{
	}
}
