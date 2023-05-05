#define F_CPU 8000000 //internal frequency of the mcu
#include <avr/io.h> //standard input output header file of AVR
#include <avr/interrupt.h> //enables the interrupt function
#include <util/delay.h> //used to pause the program for a certain amount of time(used to rintroduce any delay)
#include <stdio.h> //input output header file
#include <stdbool.h>
#include <stdint.h>
#include "USART.h"

char txdata[20];
volatile uint16_t TimerCal=0;// variable for collect echo data
uint16_t ultraINT0=0;

int main(void)
{	USART_Init(9600);
	
	/*Ultrasonic Timer Part*/
	DDRD|=(1<<3); //define pinD3 as output (this can be also written as DDRD=DDRD|(1<<3);
	TCCR0|=(1<<WGM01);//Clear timer on compare match (when TCNT0 is equal to OCR0 timer will again begin ticking from 0)
	TCCR0|=(1<<CS01);//Start timer  prescaler =8 (Prescaler is a kind of slow down for Timer incrementing, instead of incrementing each clock cycle it can be adjusted to every 2nd, 4th.,8th. etc)
	TCNT0=0; //Timer counter register, sets the initial value of the timer
	OCR0=10; //Output compare register (to compare the TCNT0 value)
	/*register value= time*(clock speed/prescale)
	register value=0.000001*(8000000/1)
	register value=10*/
	TIMSK|=(1<<OCIE0);//enable timer Compare inturrept (TIMSK - Timer Counter Interrupt Mask)(OCIE0 - Output Compare Match Interrupt Enable)
	sei(); //(this command is used to enable the interrupt)
	
    while (1) 
    {
		PORTD&=~(1<<3);//TRIG pin low (This can be also written as PORTD=PORTD&(~(1<<3));
		_delay_us(50);//wait 50 micro sec
		PORTD|=(1<<3);//TRIG pin high (This can be also written as PORTD=PORTD|(1<<3);
		_delay_us(50);//wait 50 micro sec
		PORTD&=~(1<<3);////TRIG pin low
		while(!(PIND&(1<<2)))//wait for pulse
		TimerCal=0;//rest timer
		while((PIND&(1<<2)))////wait for pulse down
		ultraINT0=TimerCal;//copy timer value
		sprintf(txdata," %u\r\n",ultraINT0);
		USART_TxString(txdata);
		
		_delay_ms(100);
    }
}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCal++;
	TCNT0=0;
	
}


