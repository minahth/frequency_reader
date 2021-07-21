/*
 * LCDMODULE.c
 *
 * Created: 13/10/2018 11:58:16 م
 *  Author: ideapad510
 */ 
#include "LCDMODULE.h"

#include <stdlib.h>
volatile  uint8 ACddram;

void LCD_init(CURSOR CURSORmod,blink blinkmod)
{
	/* Set The Control Pins Direction to OUtput */
	*LCD_Control_reg_DIR |=(1<<E) | (1<<RW) | (1<<RS);
	
	#if eight_bitdata
	/* Set the Direction of the 8 Data pins to Output */
	*LCD_Data_Reg_DIR =0xFF;
	#else
	
	#if Upper_Data_port
	/* Set The Direction of The Upper Pins to OP*/
	*LCD_Data_Reg_DIR |=(0xF0);
	#else
	/* Set The Direction of The LOWER Pins to OP*/
	*LCD_Data_Reg_DIR|=(0x0F);
	#endif
	Send_instruction(Return_Home);
	#endif
	
	Send_instruction(Data_Set);
	Send_instruction(Display_on(CURSORmod,blinkmod));
	Send_instruction(Clear_display);
}


void Send_instruction(uint8 instruction)
{
	/* RW, RS Should be 0 when Writing instructions to LCD */
	clearbit(*LCD_Control_reg_OP,RS);
	clearbit(*LCD_Control_reg_OP,RW);
	/* Delay of Address Set 60ns minimum */
	_delay_ms(DELAY_MS_LCD);
	
	/* Enable LCD E=1 */
	setbit(*LCD_Control_reg_OP,E);
	/* delay for processing PW min 450ns */
	_delay_ms(DELAY_MS_LCD);
	
	#if eight_bitdata
	
	*LCD_Data_Reg_OP = instruction;
	/* Data set-up time delay  Tdsw = 195 ns */
	_delay_ms(DELAY_MS_LCD);
	/* disable LCD E=0 */
	clearbit(*LCD_Control_reg_OP,E);
	/* Data Hold delay Th = 10ns */
	_delay_ms(DELAY_MS_LCD);
	#else
	
	#if Upper_Data_port
	/* clear the Prev Data */
	*LCD_Data_Reg_OP &=0x0F;
	*LCD_Data_Reg_OP |= (instruction & 0xF0);
	
	#else	/* Lower Port */
	/* clear the Prev Data */
	LCD_Data_Reg_OP &=0xF0;
	LCD_Data_Reg_OP |= ((instruction >> 4) );
	#endif
	/* Data set-up time delay  Tdsw = 195 ns */
	_delay_ms(DELAY_MS_LCD);
	
	/* disable LCD E=0 */
	clearbit(*LCD_Control_reg_OP,E);
	/* Data Hold delay Th = 10ns */
	_delay_ms(DELAY_MS_LCD);
	
	/* Enable LCD E=1 */
	setbit(*LCD_Control_reg_OP,E);
	/* delay for processing PW min 450ns */
	_delay_ms(DELAY_MS_LCD);
	#if Upper_Data_port
	/* clear the Prev Data */
	*LCD_Data_Reg_OP &=0x0F;
	*LCD_Data_Reg_OP |= ((instruction <<4) );
	
	#else	/* Lower Port */
	/* clear the Prev Data */
	*LCD_Data_Reg_OP &=0xF0;
	*LCD_Data_Reg_OP |= ((instruction) & 0x0f);
	#endif
	/* Data set-up time delay  Tdsw = 195 ns */
	_delay_ms(DELAY_MS_LCD);
	/* disable LCD E=0 */
	clearbit(*LCD_Control_reg_OP,E);
	/* Data Hold delay Th = 10ns */
	_delay_ms(DELAY_MS_LCD);
	#endif
}

void LCD_DisplayChar(uint8 Data)
{
	/* RW  Should be 0  and RS should be 1 when Writing Data to LCD */
	setbit(*LCD_Control_reg_OP,RS);
	clearbit(*LCD_Control_reg_OP,RW);
	/* Delay of Address Set 60ns minimum */
	_delay_ms(DELAY_MS_LCD);
	/* Enable LCD E=1 */
	setbit(*LCD_Control_reg_OP,E);
	/* delay for processing PW min 450ns */
	_delay_ms(DELAY_MS_LCD);
	#if eight_bitdata
	*LCD_Data_Reg_OP=Data;
	#else
	#if Upper_Data_port
	/* clear the Prev Data */
	*LCD_Data_Reg_OP &=0x0F;
	/* Send MSB (4-bits )*/
	*LCD_Data_Reg_OP |= (Data & 0xF0);
	
	#else /* Lower Port */
	/* clear the Prev Data */
	*LCD_Data_Reg_OP &=0xF0;
	/* Send MSB (4-bits )*/
	*LCD_Data_Reg_OP |= ((Data >> 4) & 0x0f);
	#endif
	
	/* Data set-up time delay  Tdsw = 195 ns */
	_delay_ms(DELAY_MS_LCD);
	/* disable LCD E=0 */
	clearbit(*LCD_Control_reg_OP,E);
	/* Data Hold delay Th = 10ns */
	_delay_ms(DELAY_MS_LCD);
	/* Enable LCD E=1 */
	setbit(*LCD_Control_reg_OP,E);
	
	#if Upper_Data_port
	/* clear the Prev Data */
	*LCD_Data_Reg_OP &=0x0F;
	*LCD_Data_Reg_OP |= ((Data <<4) & 0xF0);
	/* delay for processing PW min 450ns */
	_delay_ms(DELAY_MS_LCD);
	#else	/* Lower Port */
	/* clear the Prev Data */
	*LCD_Data_Reg_OP &=0xF0;
	*LCD_Data_Reg_OP |= ((Data) & 0x0f);
	#endif
	
	#endif
	/* Data set-up time delay  Tdsw = 195 ns */
	_delay_ms(DELAY_MS_LCD);
	/* disable LCD E=0 */
	clearbit(*LCD_Control_reg_OP,E);
	/* Data Hold delay Th = 10ns */
	_delay_ms(DELAY_MS_LCD);
	 LCD_LEAVE();
}


void LCD_Clear(void)
{
	/* Clear Screen */
	Send_instruction(Clear_display);
}

	void LCD_DisplayString(char * ptr)
	{
		while(*ptr != '\0')
		{
			LCD_DisplayChar(*ptr);
			ptr++;
		}
	}
	
	void LCD_Select_RowCol(uint8 Row , uint8 col)
	{
		uint8 Address=0;
		
		switch(Row)
		{
			case 0:
			/* When Writing To LCD 1st Col The Base Address is 0x00 */
			Address=col + LCD_1ST_BASE;
			break;
			case 1:
			/* When Writing To LCD 2nd Col The Base Address is 0x40 */
			Address=col + LCD_2ND_BASE;
			break;
			case 2:
			/* When Writing To LCD 1st Col The Base Address is 0x10 */
			Address = col + LCD_3RD_BASE;
			break;
			case 3:
			/* When Writing To LCD 1st Col The Base Address is 0x50 */
			Address=col+ LCD_4TH_BASE;
			break;
		}
		
		Send_instruction(DDRAM_CONST|Address);
		 LCD_LEAVE();
	}
	
	
	void LCD_DisplayInt(sint32 data)
	{
		/* String to hold the ascii result */
		char Result[17];
		/* 10 for decimal */
		itoa(data,Result,10);
		/* Display The Result */
		LCD_DisplayString(Result);
	}
	   void LCD_cursor_blink_control(CURSOR CURSORmod,blink blinkmod)
	   {
		   Send_instruction(Display_on(CURSORmod,blinkmod)) ;
	   }
	   
	     BUSY_FLAG checkthebusyflag()
		 {  BUSY_FLAG temp;
			 gpio_inputconfg(LCD_Data_Reg_DIR,LCD_Data_Reg_OP,PULLUP_mod,7);
			  clearbit(*LCD_Control_reg_OP,E);
			 _delay_ms(DELAY_MS_LCD);
			 clearbit(*LCD_Control_reg_OP,RS);
			 setbit(*LCD_Control_reg_OP,RW);
			  setbit(*LCD_Control_reg_OP,E);
			 _delay_ms(DELAY_MS_LCD);
			 temp=isbitset(*LCD_Data_Reg_IP,7)>>7;
			  clearbit(*LCD_Control_reg_OP,E);
			  gpio_outputconfg(LCD_Data_Reg_DIR,LCD_Data_Reg_IP,OUTPASS,7);
			 return temp; 
		 }
		 void readacddram()
		 { clearbit(*LCD_Control_reg_OP,E);
			  _delay_ms(DELAY_MS_LCD);
			  *LCD_Data_Reg_DIR=0x00;
			 clearbit(*LCD_Control_reg_OP,RS);
			 setbit(*LCD_Control_reg_OP,RW);
			 /* Delay of Address Set 60ns minimum */
			 _delay_ms(DELAY_MS_LCD);
			 setbit(*LCD_Control_reg_OP,E);
			  _delay_ms(DELAY_MS_LCD);
			  ACddram=  *LCD_Data_Reg_IP;
			  clearbit(*LCD_Control_reg_OP,E);
			  _delay_ms(DELAY_MS_LCD);
			  *LCD_Data_Reg_DIR=0xFF;
		 }
	
		 void patternmaker (char* ptr,uint8 patternumber)

		 {   
			 unsigned char i;
             /* readacddram();*/
			  
			Send_instruction(CGRAM_CONST|(patternumber*8)); //this make read and write is done on cgram
			 for(i=0;i<8;i++)
			 {LCD_DisplayChar(ptr[i]);
				 
			 }
			/* Send_instruction(DDRAM_CONST|ACddram);*/
			 
			 
		 }
		 void LCD_Displaypattern(uint8 patternumber)
		 {
			 LCD_DisplayChar(patternumber);
		 }
		 
		 void savevariable(uint8 var,uint8 address)
		 {  readacddram();
			 Send_instruction(address);
			 LCD_DisplayChar(var);
	     	 Send_instruction(DDRAM_CONST|ACddram);
						  
			 
		 }
		 uint8 getvariable(uint8 address)
		 {readacddram();
			  Send_instruction(address);
			   clearbit(*LCD_Control_reg_OP,E);
			   _delay_ms(DELAY_MS_LCD);
			   *LCD_Data_Reg_DIR=0x00;
			   setbit(*LCD_Control_reg_OP,RS);
			   setbit(*LCD_Control_reg_OP,RW);
			   /* Delay of Address Set 60ns minimum */
			   _delay_ms(DELAY_MS_LCD);
			   setbit(*LCD_Control_reg_OP,E);
			   _delay_ms(DELAY_MS_LCD);
			   uint8 var=  *LCD_Data_Reg_IP;
			   clearbit(*LCD_Control_reg_OP,E);
			   _delay_ms(DELAY_MS_LCD);
			   *LCD_Data_Reg_DIR=0xFF;
			 return var;
			 
		 }
		 
		 void LCD_LEAVE()
		 {
		 	clearbit(*LCD_Control_reg_OP,E);
			 clearbit(*LCD_Control_reg_OP,RS);
			 clearbit(*LCD_Control_reg_OP,RW);
			 #if eight_bitdata
			 *LCD_Data_Reg_OP=0;
			#else
			 #if Upper_Data_port
			 *LCD_Data_Reg_OP &=0x0F;
			 #else
			 *LCD_Data_Reg_OP &=0xF0;
			 #endif
			 #endif
		 }
		 