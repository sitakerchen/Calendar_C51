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

uchar* trans2LunarCalendar(int y, int m, int d);
void calendar_editAndLookupLunarDate();
uchar calendar_month_Word2value(uchar str[4]);
void calendar_homePage();
void calendar_showTime(SYSTEMTIME *Time);
/*****************显示 当前时间， 日期*************************/
uchar* WeekString;

void calendar_showTime(SYSTEMTIME *Time)
{
	uint key = 0;
	uint flag = 1;
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
		if (key == 1)
		{
			flag ^= 1;
			if (flag == 0)
			{
				//	show lunar date
				DS1302_GetTime(Time);
				DateToStr(Time);
				lcd_disString(SLine + 1, trans2LunarCalendar(10, 10, 10), 0); 
			}else
			{
				// The Gregorian calendar
				DS1302_GetTime(Time);
				DateToStr(Time);
				lcd_disString(SLine + 1, Time->DateString, 0);
			}	
		}	
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
	if (strcmp(str, "Jan") == 0)
	{
		return 1;
	}
	else if (strcmp(str, "Feb") == 0)
	{
	 	return 2;
	}
	else if (strcmp(str, "Mar") == 0)
	{
	 	return 2;
	}
	else if (strcmp(str, "Ari") == 0)
	{
	 	return 2;
	}
	else if (strcmp(str, "Jue") == 0)
	{
	 	return 2;
	}
	else return 0; 	//	懒得写了，可以自行添加		

}
/*******************在LCD直接写日期******************/
void calendar_wdate(uchar add,uchar date)
{
	uchar shi,ge;
	shi=date/10;
	ge=date%10;
	lcd_wcmd(FLine + add);
	lcd_wdat(0x30+shi);
	lcd_wdat(0x30+ge);
}
/*******************公历转农历*************************/
uint LunarCalendarDay;
uint code LunarCalendarTable[199] =
{
	0x04AE53,0x0A5748,0x5526BD,0x0D2650,0x0D9544,0x46AAB9,0x056A4D,0x09AD42,0x24AEB6,0x04AE4A,/*1901-1910*/
	0x6A4DBE,0x0A4D52,0x0D2546,0x5D52BA,0x0B544E,0x0D6A43,0x296D37,0x095B4B,0x749BC1,0x049754,/*1911-1920*/
	0x0A4B48,0x5B25BC,0x06A550,0x06D445,0x4ADAB8,0x02B64D,0x095742,0x2497B7,0x04974A,0x664B3E,/*1921-1930*/
	0x0D4A51,0x0EA546,0x56D4BA,0x05AD4E,0x02B644,0x393738,0x092E4B,0x7C96BF,0x0C9553,0x0D4A48,/*1931-1940*/
	0x6DA53B,0x0B554F,0x056A45,0x4AADB9,0x025D4D,0x092D42,0x2C95B6,0x0A954A,0x7B4ABD,0x06CA51,/*1941-1950*/
	0x0B5546,0x555ABB,0x04DA4E,0x0A5B43,0x352BB8,0x052B4C,0x8A953F,0x0E9552,0x06AA48,0x6AD53C,/*1951-1960*/
	0x0AB54F,0x04B645,0x4A5739,0x0A574D,0x052642,0x3E9335,0x0D9549,0x75AABE,0x056A51,0x096D46,/*1961-1970*/
	0x54AEBB,0x04AD4F,0x0A4D43,0x4D26B7,0x0D254B,0x8D52BF,0x0B5452,0x0B6A47,0x696D3C,0x095B50,/*1971-1980*/
	0x049B45,0x4A4BB9,0x0A4B4D,0xAB25C2,0x06A554,0x06D449,0x6ADA3D,0x0AB651,0x093746,0x5497BB,/*1981-1990*/
	0x04974F,0x064B44,0x36A537,0x0EA54A,0x86B2BF,0x05AC53,0x0AB647,0x5936BC,0x092E50,0x0C9645,/*1991-2000*/
	0x4D4AB8,0x0D4A4C,0x0DA541,0x25AAB6,0x056A49,0x7AADBD,0x025D52,0x092D47,0x5C95BA,0x0A954E,/*2001-2010*/
	0x0B4A43,0x4B5537,0x0AD54A,0x955ABF,0x04BA53,0x0A5B48,0x652BBC,0x052B50,0x0A9345,0x474AB9,/*2011-2020*/
	0x06AA4C,0x0AD541,0x24DAB6,0x04B64A,0x69573D,0x0A4E51,0x0D2646,0x5E933A,0x0D534D,0x05AA43,/*2021-2030*/
	0x36B537,0x096D4B,0xB4AEBF,0x04AD53,0x0A4D48,0x6D25BC,0x0D254F,0x0D5244,0x5DAA38,0x0B5A4C,/*2031-2040*/
	0x056D41,0x24ADB6,0x049B4A,0x7A4BBE,0x0A4B51,0x0AA546,0x5B52BA,0x06D24E,0x0ADA42,0x355B37,/*2041-2050*/
	0x09374B,0x8497C1,0x049753,0x064B48,0x66A53C,0x0EA54F,0x06B244,0x4AB638,0x0AAE4C,0x092E42,/*2051-2060*/
	0x3C9735,0x0C9649,0x7D4ABD,0x0D4A51,0x0DA545,0x55AABA,0x056A4E,0x0A6D43,0x452EB7,0x052D4B,/*2061-2070*/
	0x8A95BF,0x0A9553,0x0B4A47,0x6B553B,0x0AD54F,0x055A45,0x4A5D38,0x0A5B4C,0x052B42,0x3A93B6,/*2071-2080*/
	0x069349,0x7729BD,0x06AA51,0x0AD546,0x54DABA,0x04B64E,0x0A5743,0x452738,0x0D264A,0x8E933E,/*2081-2090*/
	0x0D5252,0x0DAA47,0x66B53B,0x056D4F,0x04AE45,0x4A4EB9,0x0A4D4C,0x0D1541,0x2D92B5          /*2091-2099*/
};
int code MonthAdd[12] = {0,31,59,90,120,151,181,212,243,273,304,334};

int LunarCalendar(int year,int month,int day)
{
	int Spring_NY,Sun_NY,StaticDayCount;
	int index,flag;
	//Spring_NY 
	//Sun_NY 
	if ( ((LunarCalendarTable[year-1901] & 0x0060) >> 5) == 1)
		Spring_NY = (LunarCalendarTable[year-1901] & 0x001F) - 1;
	else
		Spring_NY = (LunarCalendarTable[year-1901] & 0x001F) - 1 + 31;
	Sun_NY = MonthAdd[month-1] + day - 1;
	if ( (!(year % 4)) && (month > 2))
		Sun_NY++;
	//StaticDayCount
	//index 
	//flag 
	if (Sun_NY >= Spring_NY)
	{
		Sun_NY -= Spring_NY;
		month = 1;
		index = 1;
		flag = 0;
		if ( ( LunarCalendarTable[year - 1901] & (0x80000 >> (index-1)) ) ==0)
			StaticDayCount = 29;
		else
			StaticDayCount = 30;
		while (Sun_NY >= StaticDayCount)
		{
			Sun_NY -= StaticDayCount;
			index++;
			if (month == ((LunarCalendarTable[year - 1901] & 0xF00000) >> 20) )
			{
				flag = ~flag;
				if (flag == 0)
					month++;
			}
			else
				month++;
			if ( ( LunarCalendarTable[year - 1901] & (0x80000 >> (index-1)) ) ==0)
				StaticDayCount=29;
			else
				StaticDayCount=30;
		}
		day = Sun_NY + 1;
	}
	else
	{
		Spring_NY -= Sun_NY;
		year--;
		month = 12;
		if ( ((LunarCalendarTable[year - 1901] & 0xF00000) >> 20) == 0)
			index = 12;
		else
			index = 13;
		flag = 0;
		if ( ( LunarCalendarTable[year - 1901] & (0x80000 >> (index-1)) ) ==0)
			StaticDayCount = 29;
		else
			StaticDayCount = 30;
		while (Spring_NY > StaticDayCount)
		{
			Spring_NY -= StaticDayCount;
			index--;
			if (flag == 0)
				month--;
			if (month == ((LunarCalendarTable[year - 1901] & 0xF00000) >> 20))
				flag = ~flag;
			if ( ( LunarCalendarTable[year - 1901] & (0x80000 >> (index-1)) ) ==0)
				StaticDayCount = 29;
			else
				StaticDayCount = 30;
		}
		day = StaticDayCount - Spring_NY + 1;
	}
	LunarCalendarDay |= day;
	LunarCalendarDay |= (month << 6);
	if (month == ((LunarCalendarTable[year - 1901] & 0xF00000) >> 20))
		return 1;
	else
		return 0;
}

uchar* trans2LunarCalendar(int year, int month, int day)
{
	const char *ChDay[] = {"*","1","2","3","4","5",
	                       "6","7","8","9","10",
	                       "11","12","13","14","15",
	                       "16","17","18","19","20",
	                       "21","22","23","24","25",
	                       "26","27","28","29","30"
	                      };
	const char *ChMonth[] = {"*","1","2","3","4","5",
	                       "6","7","8","9","10",
	                       "11","12"};
	uchar str[13] = "";
	
	if (LunarCalendar(year,month,day))
	{
		strcat(str,"-");
		strcat(str,ChMonth[(LunarCalendarDay & 0x3C0) >> 6]);
	}
	else
		strcat(str,ChMonth[(LunarCalendarDay & 0x3C0) >> 6]);
	strcat(str,"-");
	strcat(str,ChDay[LunarCalendarDay & 0x3F]);
	str[12] = '\0'; 
	return str;
}
/*********************判断闰年*************************/
uchar Calendar_judge(uchar y)
{
	int tem = 2000;
	tem += y;	
	if ( (tem % 4 == 0 && tem % 100 != 0 ) || ( tem % 400 == 0 ) ) return 1;
	return 0;
}
/*******************************************************/


char m2d[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};	//	how many days a month

void calendar_editAndLookupLunarDate(SYSTEMTIME* Time)
{
	uint key = 0, slnum = 0, tem = 0;
	char y, m, d;
	
	DS1302_GetTime(Time);
	DateToStr(Time);

	y =	Time->Year; 
	m = Time->Month;
	d = Time->Day; 

	lcd_disString(FLine + 4, Time->DateString, 1);	   
	while (key != EXIT)
	{
		key = read_key();
		if (key == 1)
		{
			slnum++;
			if (slnum == 1)
			{
//				DS1302_Stop(1);	//	暂停计时
				lcd_wcmd(FLine + 10);	// 日 位置
				lcd_wcmd(0x0f);	//	反白�
			}
			if (slnum == 2)
			{
				lcd_wcmd(FLine + 7);	
			}
			if (slnum == 3)
			{
				lcd_wcmd(FLine + 4);
			}
			if (slnum == 4)
			{
			 	slnum = 0;
				lcd_wcmd(0x0c);
//				DS1302_Stop(0);
			}					
		}
		if (slnum != 0)
		{
			if (key == 2)
			{
					if(slnum == 1)
					{
						d ++;					//增加日
						tem = (m == 2)?Calendar_judge(y):0;
						if(d >= m2d[(int) m ] + 1 + tem) d=0;			//判断是否超出范围
						calendar_wdate(10, d);		//显示
						lcd_wcmd(FLine + 10);
					}
					if(slnum == 2)
					{
						m ++;					  //增加月
						if(m >= 13) m = 0;				  //判断是否超出范围
						calendar_wdate(7, m);		  //显示
						lcd_wcmd(FLine + 7);
					}
					if(slnum == 3)
					{
						y ++;					  //增加年
						calendar_wdate(4, y);		  //显示
						lcd_wcmd(FLine + 4);
					}	
			}
			if (key == 3)
			{
					if(slnum == 1)
					{
						d --;					//增加日
						if(d <= -1 ) d = m2d[(int) m] + (m == 2)?Calendar_judge(y):0;			//判断是否超出范围
						calendar_wdate(10, d);		//显示
						lcd_wcmd(FLine + 10);
						
					}
					if(slnum == 2)
					{
						m --;					  //增加月
						if(m <= -1) m = 12;				  //判断是否超出范围
						calendar_wdate(7, m);		  //显示
						lcd_wcmd(FLine + 7);
					}
					if(slnum == 3)
					{
						y --;					  //增加年
						if ( y <= -1 ) y = 0;
						calendar_wdate(4, y);		  //显示
						lcd_wcmd(FLine + 4);
					}	
			}
		}
		else
		{
			if (key == 3)
			{
				lcd_disString(SLine + 4, trans2LunarCalendar(y, m, d), 1); 					
			}
		}

	}
	lcd_wcmd(0x0c);	//	关闭光标闪烁	
	
}

#endif                            //条件汇编结束