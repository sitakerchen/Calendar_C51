#include <reg52.h>
#include <intrins.h>
#include "DS1302.h"
#include "KeyScan.h"
#include "LCD1602.h"
#include "Calendar.h"

/***************init*******************/
uchar code DateStr[] = __DATE__;	//	xx-xx-xx
uchar code TimeStr[] = __TIME__;	//  xx:xx:xx 
uchar tem[4];
uchar year, month, day, h, m, s;


void init_All(SYSTEMTIME *Time, uint i)
{
	DS1302_Initial();
	lcd_init();
	delayms(10);

	year = (DateStr[10] - '0') + (DateStr[9] - '0') * 10;
	for (i = 0; i < 3; ++ i ) tem[i] = DateStr[i];
	tem[3] = '\0';
   	month = calendar_month_Word2value(tem);
	day = (DateStr[5] - '0') + (DateStr[4] - '0') * 10;

	h = (DateStr[1] - '0') + (DateStr[0] - '0') * 10;
	m = (DateStr[4] - '0') + (DateStr[3] - '0') * 10;
	s = (DateStr[7] - '0') + (DateStr[6] - '0') * 10;
	DS1302_Stop(1);

	DS1302_SetTime(DS1302_YEAR, year);
	DS1302_SetTime(DS1302_MONTH, month);
	DS1302_SetTime(DS1302_DAY, day);
	DS1302_SetTime(DS1302_HOUR, h);
	DS1302_SetTime(DS1302_MINUTE, m);
	DS1302_SetTime(DS1302_SECOND, s);

	DS1302_Stop(0);
}
/**************************************/

/****************main********************/

/****************test module*************/

//void main() // test
//{
//	DS1302_Initial();
//	lcd_init();
//	delayms(10);
//	year = (DateStr[10] - '0') + (DateStr[9] - '0') * 10;
//	while (1)
//	{
//		lcd_disString(FLine, TimeStr);
//		lcd_disString(SLine, DateStr);
//	}
//}

/*****************test end***************/

void main()
{
	/***param define***/
	SYSTEMTIME time;
	uint key = 0;
	int flag = 0;
	uint i = 0;
	uint countDown = 2;
	/***define end*****/

	init_All(&time, 0);
		
	while (1)
	{
		if (flag == 0)
		{
			lcd_wcmd(0x01);
			calendar_homePage();
			for (i = countDown; i >= 1; -- i)
			{	
				lcd_wcmd(FLine + 15);
				lcd_wdat(i + '0'); 
				delayms(1000);							
			}

			lcd_wcmd(FLine + 15);
			lcd_wdat(' ');

		 	flag = 1;
		}
		key = read_key();

		switch (key)
		{
			case 1:	//	current time
				lcd_wcmd(0x01);  
				calendar_showTime(&time);
				flag = 0;
				break;
			case 2:	// lunar date
				lcd_wcmd(0x01); 
				calendar_editAndLookupLunarDate();
				flag = 0;
				break;
			case 3:	//	empty
				
				flag = 0;	
				break;
			case 4: 
				// exit key
				flag = 0;
				break;
			default:
			   	//	do nothing
				break;
		}
	}
}