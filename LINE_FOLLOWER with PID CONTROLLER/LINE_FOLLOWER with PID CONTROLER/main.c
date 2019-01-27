/*
 * PID CONTROLLER.c
 *
 * Created: 7/28/2018 6:26:21 PM
 * Author : hp
 */ 
#define F_CPU 8000000 

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
int i,max,j;


void pwm_init()
{
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
	TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS11); // prescaling of 8
	ICR1 = 4000;         //top value for maximum output
}



int pin_status[8];
int pin_config[8];

double error,p_error,t_error,integral=0,differential=0,kp,ki,kd,required;

int pid_calc (void)
{
	error=0;
	j=0;
	t_error=0;
	required=7;
	
	//setting pin status
	pin_status[0]=0;//0
	pin_status[1]=2;//2
	pin_status[2]=4;//4
	pin_status[3]=6;//6
	pin_status[4]=8;//8
	pin_status[5]=10;//10
	pin_status[6]=12;//12
	pin_status[7]=14;//14

	//checking pins
	for(i=0;i<8;i++)
	{
		if(bit_is_set(PIND,i)==1)
		{
			pin_config[i]=1;
			j++;
		}
		else
		{
			pin_config[i]=0;
			
		}
	}

	//calculating error
	for(i=0;i<8;i++)
	{
		error+=(pin_status[i]*pin_config[i]);
	}
	error=required-error;
	error/=j;
	integral=p_error+error;
	differential=(error-p_error);
	t_error=(kp*error+ki*integral+kd*differential);
	p_error=error;
	return t_error;
}


void forward(void)
{
	
	PORTB=PORTB|(1<<PINB2);//m1 + m1 forward
	//PORTB=PORTB&(~(1<<PINB0));//m1 -
	PORTB=PORTB|(1<<PINB3);//m2 + m2 forward
	//PORTB=PORTB&(~(1<<PINB5));//m2 -
	
}


int main(void)
{
	
	/* Replace with your application code */
	pwm_init();
	DDRD=0b00000000;
	PORTD=0;
	DDRB=0b00111110;
	PORTB=0;
	
	while (1)
	{
		
		forward();
		double pid,left_rpm,right_rpm;
		pid=pid_calc();
		OCR1A=2000;
		OCR1B=2000;
		left_rpm=200+(pid);
		right_rpm=200-(pid); //max value
		OCR1A=(4000*left_rpm)/300;
		OCR1B=(4000*right_rpm)/300;
	}
}

