/*
 * round 1 PWM code.c
 *
 * Created: 2/18/2018 11:37:22 PM
 * Author : hp
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
int a=0;

void pwm_init()
{
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
	TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS11); // prescaling of 8 
	ICR1 = 1000;         //top value for maximum output
}

void inch(void)
{
	forward();
	_delay_ms(100);
	stop();
}

void forward(void)
{
	OCR1A=1000;
	OCR1B=1000;
	PORTB=PORTB|(1<<PINB2);//m1 + m1 forward 
	PORTB=PORTB&(~(1<<PINB0));//m1 -
	PORTB=PORTB|(1<<PINB3);//m2 + m2 forward
	PORTB=PORTB&(~(1<<PINB5));//m2 -
}
void backward(void)
{
	OCR1A=1000;
	OCR1B=1000;	
	PORTB=PORTB&(~(1<<PINB2));//m1 - m1 backward 
	PORTB=PORTB|(1<<PINB0);//m1 +
	PORTB=PORTB&(~(1<<PINB3));//m2 - m2 backward
	PORTB=PORTB|(1<<PINB5);//m2 +
}
void stop(void)
{
	PORTB=PORTB&(~(1<<PINB2));//m1 - m1 stop
	PORTB=PORTB&(~(1<<PINB0));//m1 -
	PORTB=PORTB&(~(1<<PINB3));//m2 - m2 stop
	PORTB=PORTB&(~(1<<PINB5));//m2 _
}
void hard_right(void)
{
	OCR1A=1000;
	OCR1B=300;
	PORTB=PORTB|(1<<PINB2);//m1 + m1 forward
	PORTB=PORTB&(~(1<<PINB0));//m1 -
	PORTB=PORTB&(~(1<<PINB3));//m2 - m2 backward
	PORTB=PORTB|(1<<PINB5);//m2 +	
	
}
void hard_left(void)
{
	OCR1A=300;
	OCR1B=1000;
	PORTB=PORTB|(1<<PINB2);//m1 + m1 forward
	PORTB=PORTB&(~(1<<PINB0));//m1 -
	PORTB=PORTB|(1<<PINB3);//m2 + m2 forward
	PORTB=PORTB&(~(1<<PINB5));//m2 -
}
void right(void)
{
	OCR1A=1000;
	OCR1B=200;
	PORTB=PORTB|(1<<PINB2);//m1 + m1 forward
	PORTB=PORTB&(~(1<<PINB0));//m1 -
	PORTB=PORTB|(1<<PINB3);//m2 + m2 forward
	PORTB=PORTB&(~(1<<PINB5));//m2 -
}
void left(void)
{
	OCR1A=200;
	OCR1B=1000;
	PORTB=PORTB|(1<<PINB2);//m1 + m1 forward
	PORTB=PORTB&(~(1<<PINB0));//m1 -
	PORTB=PORTB|(1<<PINB3);//m2 + m2 forward
	PORTB=PORTB&(~(1<<PINB5));//m2 -
}
void u_turn(void)
{
	OCR1A=1000;
	OCR1B=1000;
	PORTB=PORTB|(1<<PINB2);//m1 + m1 forward
	PORTB=PORTB&(~(1<<PINB0));//m1 -
	PORTB=PORTB&(~(1<<PINB3));//m2 - m2 backward
	PORTB=PORTB|(1<<PINB5);//m2 +
	_delay_ms(50);	
}
int main(void)
{
    /* Replace with your application code */
	pwm_init();
	DDRD = 0b00110000;
	DDRA=0b00000000;
	PORTA=0;
	DDRB=0b00101101;
	PORTB=0;
    while (1) 
    {
		//sab band tb forward	
		if(bit_is_clear(PINA,0)&&bit_is_clear(PINA,1)&&bit_is_clear(PINA,2)&&bit_is_clear(PINA,3)&&bit_is_clear(PINA,4)&&bit_is_clear(PINA,5)&&bit_is_clear(PINA,6)&&bit_is_clear(PINA,7))
		{
			forward();
		}
		
	
		//code 4
		else if(bit_is_clear(PINA,4)&&bit_is_clear(PINA,5)&&bit_is_clear(PINA,6)&&bit_is_clear(PINA,7))
		{
			stop();
			_delay_ms(50);
			inch();
			if(bit_is_set(PINA,0)&&bit_is_set(PINA,1)&&bit_is_set(PINA,2)&&bit_is_set(PINA,3)&&bit_is_set(PINA,4)&&bit_is_set(PINA,5)&&bit_is_set(PINA,6)&&bit_is_set(PINA,7))
			{
				hard_right();
			}
			else if(bit_is_clear(PINA,1)|bit_is_clear(PINA,2)|bit_is_clear(PINA,3)|bit_is_clear(PINA,4)|bit_is_clear(PINA,5)|bit_is_clear(PINA,6))
			{
				forward();
			}
			else
			{
				left();
			}
		}
		else if(bit_is_clear(PINA,3)&&bit_is_clear(PINA,4)&&bit_is_clear(PINA,5)&&bit_is_clear(PINA,6))
		{
			stop();
			_delay_ms(50);
			inch();
			if(bit_is_set(PINA,0)&&bit_is_set(PINA,1)&&bit_is_set(PINA,2)&&bit_is_set(PINA,3)&&bit_is_set(PINA,4)&&bit_is_set(PINA,5)&&bit_is_set(PINA,6)&&bit_is_set(PINA,7))
			{
				hard_right();
			}
			else if(bit_is_clear(PINA,1)|bit_is_clear(PINA,2)|bit_is_clear(PINA,3)|bit_is_clear(PINA,4)|bit_is_clear(PINA,5)|bit_is_clear(PINA,6))
			{
				forward();
			}
			else
			{
				left();
			}
		}

		else if(bit_is_clear(PINA,2)&&bit_is_clear(PINA,3)&&bit_is_clear(PINA,4)&&bit_is_clear(PINA,5))
		{
			forward();
			a=1;
		}
		
		else if(bit_is_clear(PINA,0)&&bit_is_clear(PINA,1)&&bit_is_clear(PINA,2)&&bit_is_clear(PINA,3))
		{
			stop();
			_delay_ms(50);
			inch();
			if(bit_is_set(PINA,0)&&bit_is_set(PINA,1)&&bit_is_set(PINA,2)&&bit_is_set(PINA,3)&&bit_is_set(PINA,4)&&bit_is_set(PINA,5)&&bit_is_set(PINA,6)&&bit_is_set(PINA,7))
			{
				hard_left();
			}
			else if(bit_is_clear(PINA,1)|bit_is_clear(PINA,2)|bit_is_clear(PINA,3)|bit_is_clear(PINA,4)|bit_is_clear(PINA,5)|bit_is_clear(PINA,6))
			{
				forward();
			}
			else
			{
				hard_right();
			}
		}
		else if(bit_is_clear(PINA,1)&&bit_is_clear(PINA,2)&&bit_is_clear(PINA,3)&&bit_is_clear(PINA,4))
		{
			stop();
			_delay_ms(50);
			inch();
			if(bit_is_set(PINA,0)&&bit_is_set(PINA,1)&&bit_is_set(PINA,2)&&bit_is_set(PINA,3)&&bit_is_set(PINA,4)&&bit_is_set(PINA,5)&&bit_is_set(PINA,6)&&bit_is_set(PINA,7))
			{
				hard_left();
			}
			else if(bit_is_clear(PINA,1)|bit_is_clear(PINA,2)|bit_is_clear(PINA,3)|bit_is_clear(PINA,4)|bit_is_clear(PINA,5)|bit_is_clear(PINA,6))
			{
				forward();
			}
			else
			{
				hard_right();
			}
		}
		

		//code for 3

		else if(bit_is_clear(PINA,5)&&bit_is_clear(PINA,6)&&bit_is_clear(PINA,7))
		{
			hard_right();
			a=5;
		}
		
		else if(bit_is_clear(PINA,4)&&bit_is_clear(PINA,5)&&bit_is_clear(PINA,6))
		{
			right();
			a=3;
		}
		
		else if(bit_is_clear(PINA,2)&&bit_is_clear(PINA,3)&&bit_is_clear(PINA,4))
		{
			forward();
			a=1;
		}

		else if(bit_is_clear(PINA,3)&&bit_is_clear(PINA,4)&&bit_is_clear(PINA,5))
		{
			forward();
			a=1;
		}

		else if(bit_is_clear(PINA,1)&&bit_is_clear(PINA,2)&&bit_is_clear(PINA,3))
		{
			left();
			a=2;
		}

		

		else if(bit_is_clear(PINA,0)&&bit_is_clear(PINA,1)&&bit_is_clear(PINA,2))
		{
			hard_left();
			a=4;
		}


		//code for 2

		else if(bit_is_clear(PINA,6)&&bit_is_clear(PINA,7))
		{
			hard_right();
			a=5;
		}

		else if(bit_is_clear(PINA,5)&&bit_is_clear(PINA,6))
		{
			right();
			a=3;
		}

		else if(bit_is_clear(PINA,2)&&bit_is_clear(PINA,3))
		{
			forward();
			a=1;
		}
		
		else if(bit_is_clear(PINA,3)&&bit_is_clear(PINA,4))
		{
			forward();
			a=1;
		}

		else if(bit_is_clear(PINA,4)&&bit_is_clear(PINA,5))
		{
			forward();
			a=1;
		}

		else if(bit_is_clear(PINA,1)&&bit_is_clear(PINA,2))
		{
			left();
			a=2;
		}

		else if(bit_is_clear(PINA,0)&&bit_is_clear(PINA,1))
		{
			hard_left();
			a=4;
		}



		//code for 1

		else if(bit_is_clear(PINA,6))
		{
			hard_right();
			a=5;
		}

		else if(bit_is_clear(PINA,7))
		{
			hard_right();
			a=5;
		}

		else if(bit_is_clear(PINA,5))
		{
			right();
			a=3;
		}

		else if(bit_is_clear(PINA,3))
		{
			forward();
			a=1;
		}

		else if(bit_is_clear(PINA,4))
		{
			forward();
			a=1;
		}

		else if(bit_is_clear(PINA,2))
		{
			left();
			a=2;
		}


		else if(bit_is_clear(PINA,0))
		{
			hard_left();
			a=4;
		}
		
		else if(bit_is_set(PINA,0)&&bit_is_set(PINA,1)&&bit_is_set(PINA,2)&&bit_is_set(PINA,3)&&bit_is_set(PINA,4)&&bit_is_set(PINA,5)&&bit_is_set(PINA,6)&&bit_is_set(PINA,7))
		{
			if(a==2|a==3|a==4|a==5)
			{
				u_turn();
			}
			else if(a==1)
			{
				hard_right();
			}
			else
			{
				left();
			}
		}


		else
		{

			if(a==1)
			{
				forward();
			}
			
			else if(a==2)
			{
				left();
			}

			else if(a==3)
			{
				right();
			}
   
			else if(a==4)
			{
				hard_left();
			}

			else if(a==5)
			{
				hard_right();
			}

			else
			{
				backward();
			}
		}
    }
}



