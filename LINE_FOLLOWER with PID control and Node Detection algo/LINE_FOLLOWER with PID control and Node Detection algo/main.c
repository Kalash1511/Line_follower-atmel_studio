/*
 * pid.c
 *
 * Created: 2/17/2018 7:05:00 PM
 * Author : hp
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
int a,i,p_error,max=100;
float Kp=1,Kd=0;
int pid,left_rpm,right_rpm,error;


void pwm_init()
{
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
	TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS11); // prescaling of 8
	ICR1 = 1000;         //top value for maximum output
}
void stop(void)
{
	PORTB=PORTB&(~(1<<PINB2));//m1 - m1 stop
	PORTB=PORTB&(~(1<<PINB0));//m1 -
	
	PORTB=PORTB&(~(1<<PINB3));//m2 - m2 stop
	PORTB=PORTB&(~(1<<PINB5));//m2 _
}

void right(void)
{
	OCR1A=1000;
	OCR1B=400;
	PORTB=PORTB|(1<<PINB2);//m1 + m1 forward
	PORTB=PORTB&(~(1<<PINB0));//m1 -
	PORTB=PORTB|(1<<PINB3);//m2 + m2 forward
	PORTB=PORTB&(~(1<<PINB5));//m2 -
}
void left(void)
{
	OCR1A=400;
	OCR1B=1000;
	PORTB=PORTB|(1<<PINB2);//m1 + m1 forward
	PORTB=PORTB&(~(1<<PINB0));//m1 -
	PORTB=PORTB|(1<<PINB3);//m2 + m2 forward
	PORTB=PORTB&(~(1<<PINB5));//m2 -
}

void forward(void)
{
		
		PORTB=PORTB|(1<<PINB2);//m1 + m1 forward
		PORTB=PORTB&(~(1<<PINB0));//m1 -
		PORTB=PORTB|(1<<PINB3);//m2 + m2 forward
		PORTB=PORTB&(~(1<<PINB5));//m2 -
			
}


void hard_right(void)
{
	while(error==max)
	{
		OCR1A=1000;
		OCR1B=100;
		PORTB=PORTB|(1<<PINB0);//m1 + m1 forward
		PORTB=PORTB&(~(1<<PINB2));//m1 -
		PORTB=PORTB|(1<<PINB3);//m2 + m2 forward
		PORTB=PORTB&(~(1<<PINB5));//m2 -
	}
}


void hard_left(void)
{
	while(error==max)
	{
		OCR1A=100;
		OCR1B=1000;
		PORTB=PORTB|(1<<PINB0);//m1 + m1 forward
		PORTB=PORTB&(~(1<<PINB2));//m1 -
		PORTB=PORTB|(1<<PINB3);//m2 + m2 forward
		PORTB=PORTB&(~(1<<PINB5));//m2 -
	}
}

void u_turn(void)
{
	PORTB=PORTB|(1<<PINB0);//m1 + m1 forward
	PORTB=PORTB&(~(1<<PINB2));//m1 -
	PORTB=PORTB&(~(1<<PINB3));//m2 - m2 backward
	PORTB=PORTB|(1<<PINB5);//m2 +
	_delay_ms(500);
}

void inch(void)
{
	forward();
	_delay_ms(100);
}

int pid_calc(void)
{
	int required=14,j=0;
	int pin_status[8];
	pin_status[0]=0;//0
	pin_status[1]=4;//2
	pin_status[2]=8;//4
	pin_status[3]=12;//6
	pin_status[4]=16;//8
	pin_status[5]=20;//10
	pin_status[6]=24;//12
	pin_status[7]=28;//14
	int pin_config[8];
	for(i=0;i<8;i++)
	{
		if(bit_is_clear(PINA,i)==1)
		{
			pin_config[i]=1;
		}
		else
		{
			pin_config[i]=0;
			j++;
		}
	}
	for(i=0;i<8;i++)
	{
		error+=(pin_status[i]*pin_config[i]);	
	}
	if(j!=0)
	{
		error/=j;
	}
	else
	{
		error=max;
	}
	error = Kp*(required-error) + Kd*(error-p_error);
	p_error=error;
	return error;
}


int main(void)
{
    /* Replace with your application code */
    pwm_init();
	
	while (1) 
    {
		
		forward();
		pid=pid_calc();
		left_rpm=100-(2*pid);
		right_rpm=100+(2*pid); //max value
		OCR1A=(4000*left_rpm)/300;
		OCR1B=(4000*right_rpm)/300;		
		 if(bit_is_set(PINA,0)&&bit_is_set(PINA,1)&&bit_is_set(PINA,2)&&bit_is_set(PINA,3)&&bit_is_set(PINA,4)&&bit_is_set(PINA,5)&&bit_is_set(PINA,6)&&bit_is_set(PINA,7))
		{
				u_turn();
				a=6;
		}
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
		else if(bit_is_clear(PINA,0)&&bit_is_clear(PINA,1)&&bit_is_clear(PINA,2)&&bit_is_clear(PINA,3)&&bit_is_clear(PINA,4)&&bit_is_clear(PINA,5)&&bit_is_clear(PINA,6)&&bit_is_clear(PINA,7))
		{
			stop();
			_delay_ms(50);
			inch();
			if(bit_is_set(PINA,0)&&bit_is_set(PINA,1)&&bit_is_set(PINA,2)&&bit_is_set(PINA,3)&&bit_is_set(PINA,4)&&bit_is_set(PINA,5)&&bit_is_set(PINA,6)&&bit_is_set(PINA,7))
			{
				forward();
			}
			else if(bit_is_clear(PINA,1)|bit_is_clear(PINA,2)|bit_is_clear(PINA,3)|bit_is_clear(PINA,4)|bit_is_clear(PINA,5)|bit_is_clear(PINA,6))
			{
				forward();
			}
			else
			{
				forward();
			}
		}
	}
			
}





