/*
 * freq_reader.c
 *
 * Created: 9/13/2020 8:12:50 PM
 *  Author: ideapad510
 */ 

#include "microconfigrations.h"
#include "timer0.h"
#include "TIMER1.h"
#include "LCDMODULE.h"
#include "minohelper.h"
#include <avr/io.h>
#include <avr/interrupt.h>
volatile unsigned long FREQ;
volatile unsigned long OLD_FREQ;
volatile unsigned short MOST_FREQ;
volatile unsigned long x=0;
volatile unsigned char FREQANCES[8];
volatile unsigned char NEW_BUFFER=0;
ISR(TIMER1_COMPA_vect)
{
	FREQ=((unsigned long)MOST_FREQ<<8)|TCNT0;
	TCNT0=0;
	MOST_FREQ=0;
	TIMER1_SET_COUNTER(0);
	NEW_BUFFER=1;
}
ISR(TIMER0_OVF_vect)
{
	MOST_FREQ++;
}
/*
ISR(BADISR_vect)
{
	x++;
}*/
int main(void)
{
	LCD_init(cursordisable,blinkdisable);
	LCD_DisplayString("FREQ=");
	MOST_FREQ=0;
	TIMER0_ENABLE(TIMER_T0_SOURCE_FALLING,NORMAL_MOD,OC0_DISCONNECTED,OVER_FLOW_INT_ENABLE,0);
	TIMER1_ENABLE(T1_OCA1_DISCONNECT,T1_OCB1_DISCONNECT,T1_NORMAL,T1_TIMER_1024,0x3d09,0,DISABLE_TIMER1_INT,DISABLE_TIMER1_INT,ENABLE_TIMER1_INT,DISABLE_TIMER1_INT);
	global_interupt_enable();
	
	
    while(1)
    {
		
		 if (NEW_BUFFER)
		 {
			 global_interupt_disable();
			 OLD_FREQ=FREQ;
			 global_interupt_enable();
			 LCD_Select_RowCol(0,5);
			 LCD_DisplayString("            ");
			 LCD_Select_RowCol(0,5);
			 FREQANCES[inttostring(FREQANCES,OLD_FREQ)]='\0';
			 LCD_DisplayString(FREQANCES);
			 LCD_DisplayString("HZ");
			 NEW_BUFFER=0;
		 }
		
      /*  if (OLD_FREQ !=FREQ)
        {
			OLD_FREQ=FREQ;
			global_interupt_enable();
			LCD_Select_RowCol(0,5);
			LCD_DisplayString("            ");
			LCD_Select_RowCol(0,5);
			FREQANCES[inttostring(FREQANCES,OLD_FREQ)]='\0';
			LCD_DisplayString(FREQANCES);
			LCD_DisplayString("HZ");
        }*/
	/*	global_interupt_enable();*/
		
		
    }
}