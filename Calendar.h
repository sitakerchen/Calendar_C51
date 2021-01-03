/*******************************************************************
*                                                                  *
* �ļ��� Calendar.h                                                     *
* ���ߣ� cxl Hunau                                                  *
* ���䣺 sitakerchen@gmail.com                                       *
*                                                                  *
*����Ȩ��Copyright(C)cxl  All Rights Reserved  *
*���������˳��������ѧϰ��ο���������ע����Ȩ��������Ϣ��        *
*                                                                  *
********************************************************************/
#ifndef  __Calendar_h__                //�������ָ��
#define  __Calendar_h__                //�����ظ�����

#include <string.h>
#include "DS1302.h"
#include "LCD1602.h"
#include "KeyScan.h"
#include "Calendar.h"

#define uchar unsigned char
#define uint  unsigned int
#define EXIT 4

/*****************��ʾ ��ǰʱ�䣬 ����*************************/
uchar* WeekString;

void calendar_showTime(SYSTEMTIME *Time)
{
	uint key = 0;
	DS1302_GetTime(Time);
	DateToStr(Time);
	lcd_disString(SLine + 1, Time->DateString, 1);
	WeekString = WeekToStr(Time);
	lcd_disString(SLine + 11, WeekString, 1); 
	
	while (key != EXIT)	//	���ڲ��õ��ǳ���ʵ�ְ���ɨ������жϣ�����ɨ�����һ�������ڣ�������֮���޷���Ч���������Ƽ�ʹ�ó����˳��ķ�����
	{
		key = read_key();
		DS1302_GetTime(Time);
		TimeToStr(Time);
		lcd_disString(FLine + 0x04, Time->TimeString, 0);	
	}
}

/*****************��ʾ ��ҳ��************************/
char code word1[] = "    Calendar";
char code word2[] = "K1:T K2:L K4:Exi";

void calendar_homePage()
{
	lcd_disString(FLine, word1, 1);
	lcd_disString(SLine, word2, 1);
}

/*****************Ӣ���·�ת�����ַ���**************/

uchar calendar_month_Word2value(uchar str[4])
{
	if (strcmp(str, "Jan"))
	{
		return 1;
	}
	else if (strcmp(str, "Feb"))
	{
	 	return 2;
	}else return 0; 	//	����д�ˣ������������		

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

#endif                            //����������