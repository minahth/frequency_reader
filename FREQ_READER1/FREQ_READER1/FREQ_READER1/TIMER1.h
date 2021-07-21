﻿/*
 * TIMER1.h
 *
 * Created: 30/03/2020 11:57:00 م
 *  Author: ideapad510
 */ 


#ifndef TIMER1_H_
#define TIMER1_H_
#include "microconfigrations.h"


typedef enum{  DISABLE_TIMER1_INT=0,
	           ENABLE_TIMER1_INT,
         }T1_INT_DATATYPE;
	
typedef enum{  T1_NORMAL,
	           T1_PWM_8_BIT,
			   T1_PWM_9_BIT,
			   T1_PWM_10_BIT,
			   T1_CTC_OCR1A,
			   T1_FAST_PWM_8_BIT,
			   T1_FAST_PWM_9_BIT,
			   T1_FAST_PWM_10_BIT,
			   T1_PWM_PHASE_FREQ_CORRECTION_ICR1,
			   T1_PWM_PHASE_FREQ_CORRECTION_OCR1A,
			   T1_PWM_PHASE_CORRECTION_ICR1,
			   T1_PWM_PHASE_CORRECTION_OCR1A,
			   T1_CTC_ICR1,
			   T1_FAST_PWM_ICR1=14,
			   T1_FAST_PWM_OCR1A,  
}T1_MODE_DATATYPE;



typedef enum { T1_TIMER_OFF=0x00,
	T1_TIMER_1=0x01,
	T1_TIMER_8=0x02,
	T1_TIMER_64=0x03,
	T1_TIMER_256=0x04,
	T1_TIMER_1024=0x05,
	T1_TIMER_T1_SOURCE_FALLING=0x06,
	T1_TIMER_T1_SOURCE_RISING=0x07,
	
}T1_clock;


typedef enum{  T1_OCA1_DISCONNECT,
	T1_OCA1_TOGGLE,
	T1_OCA1_CLEAR,
	T1_OCA1_SET,
	
	
	
}T1_OCA1_DATATYPE;

typedef enum{  T1_OCB1_DISCONNECT,
	T1_OCB1_TOGGLE,
	T1_OCB1_CLEAR,
	T1_OCB1_SET,
}T1_OCB1_DATATYPE;
void  TIMER1_DISABLE();
void TIMER1_ENABLE(T1_OCA1_DATATYPE COM1A,
                   T1_OCB1_DATATYPE COM1B,
				   T1_MODE_DATATYPE WGM1,
				   T1_clock CS1,
				   unsigned short OCR1A_DATA,
				   unsigned short OCR1B_DATA,
				   T1_INT_DATATYPE TOIE1_state,
				   T1_INT_DATATYPE OCIE1B_state,
				   T1_INT_DATATYPE OCIE1A_state,
				   T1_INT_DATATYPE TICIE1_state);


void TIMER1_ENABLE_NORMAL_MODE(T1_INT_DATATYPE overflowint ,T1_clock);
void TIMER1_ENABLE_CTC_OCRCA_MODE(T1_INT_DATATYPE COMPER_A_MATCHint ,T1_clock clk,T1_OCA1_DATATYPE oca_type,unsigned long f_ocna);
void TIMER1_ENABLE_FREQ_DIV_MODE(unsigned char divider);


void TIMER1_FAST_PWM(T1_MODE_DATATYPE WGM1,T1_INT_DATATYPE overflowint,T1_INT_DATATYPE OCA_INT,T1_INT_DATATYPE OCB_INT,T1_INT_DATATYPE INPUTCAP,T1_clock clk,T1_OCA1_DATATYPE oca_type,T1_OCB1_DATATYPE ocb_type,unsigned short oca,unsigned short ocb);





unsigned short TIMER1_READ_COUNTER();
void TIMER1_SET_COUNTER(unsigned short x);
unsigned short TIMER1_READ_OCR1A();
void TIMER1_SET_OCR1A(unsigned short x);
unsigned short TIMER1_READ_OCR1B();
void TIMER1_SET_OCR1B(unsigned short x);
void T1_ENABLE_FOCA();
void T1_DISABLE_FOCA();
void T1_ENABLE_FOCB();
void T1_DISABLE_FOCB();
#endif /* TIMER1_H_ */