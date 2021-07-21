/*
 * LCDMODULE.h
 *
 * Created: 13/10/2018 11:58:29 م
 *  Author: ideapad510
 */ 


#ifndef LCDMODULE_H_
#define LCDMODULE_H_

#include "standerdtypes.h"
#include "macros.h"
#include "microconfigrations.h"
/*initial connections*/
#define  RS   0 /*number of pin*/
#define  RW   1 /*number of pin*/
#define  E    2 /*number of pin*/
/*the control (RS,RW,E) port registers*/
#define LCD_Control_reg_DIR PORTA_DDRA_ADDRESS
#define LCD_Control_reg_OP  PORTA_OUT_ADDRESS
#define LCD_Control_reg_IP	PORTA_INMOD_ADDRESS
/*the data port registers*/
#define LCD_Data_Reg_DIR  PORTA_DDRA_ADDRESS
#define LCD_Data_Reg_OP	PORTA_OUT_ADDRESS
#define LCD_Data_Reg_IP	PORTA_INMOD_ADDRESS
#define PUT_AC_ADDRESS_IN(ADD) {readacddram(); ADD=ACddram ; }
/* Define Upper Data to Use The Higher 4 Pins PA4 --> PA7 */
#define Upper_Data_port 1
/* uncomment This line to Use The Lower Port */
#define  DELAY_MS_LCD 0.6
	/* init Settings */
	typedef enum{ cursordisable=0,
		          cursorenable=1,
		         }CURSOR;/* 1 to Enable , 0 to Disable */
				 
	typedef enum{ blinkdisable=0,
		blinkenable=1,
	}blink;/* 1 to Enable , 0 to Disable */
	
		typedef enum{ NOT_BUSY=0,
			BUSY=1,
			}BUSY_FLAG;/* 1 to Enable , 0 to Disable */
			
	#define eight_bitdata 0 /* 1 to Enable , 0 for 4 bit */
	#define _5x10_Dot	 0	/* 1 to Enable , 0 for 5 x 8 Dot */
	#define Two_lines	 1/* 1 to Enable , 0 for 1 Line */


	#if ( _5x10_Dot && Two_lines )
	#pragma GCC error "You cannot Have Two Lines LCD with 5 * 10"
	#endif
	
	
		/* List Of Command Codes */
		#define Clear_display 0x01
		#define Return_Home	  0x02
		#define Display_off   0x08
		#define Display_on(CURSORMOD,BlinkMOD)   (0x0C | (CURSORMOD<<1) | (BlinkMOD))
		#define CURSOR_Shift_Right	 0x14
		#define CURSOR_Shift_left	 0x10
		#define Display_shift_left    0x18
		#define Display_shift_Right	  0x1C
		#define Data_Set (0x20 |(_5x10_Dot<<2)|(Two_lines<<3)| (eight_bitdata<<4))
		#define DDRAM_CONST 0x80
		#define CGRAM_CONST 0x40
        /*DDRAM BASES*/
		#define LCD_1ST_BASE 0x00  /* from 0 to 15 in decimal*/
		#define LCD_2ND_BASE 0x40  /* from 64 to 79 in decimal*/
		#define LCD_3RD_BASE 0x10  /* from 16 to 31 in decimal*/
		#define LCD_4TH_BASE 0x50  /* from 80 to 95 in decimal*/
		/*LCD START DIRECTION*/
		#define LCD_LEFT 1
		#define LCD_RIGHT 0
		/*Macros for display shift and cursor shift*/
        #define LCD_Shift(DIR)	((DIR == LCD_LEFT)? Send_instruction(Display_shift_left) : Send_instruction(Display_shift_Right))
        #define LCD_CURSOR_Shift(DIR) ((DIR == LCD_LEFT)? Send_instruction(CURSOR_Shift_left) : Send_instruction(CURSOR_Shift_Right))
        /*the used functions*/
		
		void Send_instruction(uint8 instruction);
		 void readacddram();
        void LCD_init(CURSOR CURSORmod,blink blinkmod);
        void LCD_Clear(void);
        void LCD_DisplayChar(uint8 Data);
        void LCD_DisplayString(char * ptr);
        void LCD_Select_RowCol(uint8 Row , uint8 col);
        void LCD_DisplayInt(sint32 data);
        void LCD_cursor_blink_control(CURSOR CURSORmod,blink blinkmod);
        BUSY_FLAG checkthebusyflag();
		 void patternmaker (char* ptr,uint8 patternnumber);
		  void LCD_Displaypattern(uint8 patternumber);
		  void savevariable(uint8 var,uint8 address);/*the address from 0x40 to 0xcf ftom 0x40 to 0x7f GCRAM ,from 0x80 to 0xff ddram*/  /*free places in the ddram from 0xA0 to 0xBF from 0xE0 to 0xff*/
		   uint8 getvariable(uint8 address);
		   void LCD_LEAVE();
#endif /* LCDMODULE_H_ */