/*******************************************************************
*                                                                  *
* 文件： Calendar.h                                                     *
* 作者： cxl Hunau                                                  *
* 邮箱： sitakerchen@gmail.com                                       *
*                                                                  *
*【版权】Copyright(C)cxl  All Rights Reserved  *
*【声明】此程序仅用于学习与参考，引用请注明版权和作者信息！        *
*                                                                  *
********************************************************************/
#ifndef  __Calendar_h__                //条件汇编指令
#define  __Calendar_h__                //避免重复调用

#include <string.h>
#include "DS1302.h"
#include "LCD1602.h"
#include "KeyScan.h"
#include "Calendar.h"

#define uchar unsigned char
#define uint  unsigned int
#define EXIT 4

/*****************显示 当前时间， 日期*************************/
uchar* WeekString;

void calendar_showTime(SYSTEMTIME *Time)
{
	uint key = 0;
	DS1302_GetTime(Time);
	DateToStr(Time);
	lcd_disString(SLine + 1, Time->DateString, 1);
	WeekString = WeekToStr(Time);
	lcd_disString(SLine + 11, WeekString, 1); 
	
	while (key != EXIT)	//	由于采用的是程序实现按键扫描而非中断，所以扫描会有一个窗口期，窗口期之外无法有效采样，故推荐使用长按退出的方法。
	{
		key = read_key();
		DS1302_GetTime(Time);
		TimeToStr(Time);
		lcd_disString(FLine + 0x04, Time->TimeString, 0);	
	}
}

/*****************显示 主页面************************/
char code word1[] = "    Calendar";
char code word2[] = "K1:T K2:L K4:Exi";

void calendar_homePage()
{
	lcd_disString(FLine, word1, 1);
	lcd_disString(SLine, word2, 1);
}

/*****************英文月份转数字字符串**************/

uchar calendar_month_Word2value(uchar str[4])
{
	if (strcmp(str, "Jan"))
	{
		return 1;
	}
	else if (strcmp(str, "Feb"))
	{
	 	return 2;
	}else return 0; 	//	懒得写了，可以自行添加		

}

/******************************************************/

char* trans2LunarCalendar()
{

	return "85-85-85"  ;
}


void calendar_editAndLookupLunarDate()
{	   
//	DS1302_SetTime()
	

}

#endif                            //条件汇编结束