///////////////////////////////////////////////////////////////////////////////////////////////////////
//File : DS1302.H
//Author : NULL, cxl hunau
//Created: 2020/12/31
#ifndef __DS1302_A_H__
#define __DS1302_A_H__
#include <reg52.h>
#include <intrins.h>
//引脚及位的定义
sbit DS1302_CLK = P1^0; //时钟引脚
sbit DS1302_IO = P1^1; //数据引脚
sbit DS1302_RST = P1^2; //复位引脚
sbit ACC0 = ACC^0;
sbit ACC7 = ACC^7;
//宏定义便于理解
#define unchar unsigned char
#define unint unsigned int
#define DS1302_SECOND 0x80
#define DS1302_MINUTE 0x82
#define DS1302_HOUR 0x84
#define DS1302_WEEK 0x8A
#define DS1302_DAY 0x86
#define DS1302_MONTH 0x88
#define DS1302_YEAR 0x8C
#define DS1302_RAM(X) (0xC0+(X)*2) //用于计算RAM地址的宏运算
//定义时间类型
typedef struct _SYSTEMTIME_
{
unchar Second;
unchar Minute;
unchar Hour;
unchar Week;
unchar Day;
unchar Month;
unchar Year;
unchar DateString[9];
unchar TimeString[9];
}SYSTEMTIME;
//函数声明
void DS1302_InputByte(unchar Data); //写入一个字节(内部函数)
unchar DS1302_OutputByte(void); //读取一个字节(内部函数)
void Write1302(unchar Address, unchar Data); //在指定地址写入指定的数据
unchar Read1302(unchar Address); //读取DS1302指定地址中的数据
void DS1302_SetProtect(bit Flag); //DS1302是否写入保护
void DS1302_SetTime(unchar Address, unchar Value);//设置时间函数
void DS1302_GetTime(SYSTEMTIME *Time); //读出时间
void DateToStr(SYSTEMTIME *Time); //日期转化成字符
void TimeToStr(SYSTEMTIME *Time); //时间转化成字符
unchar* WeekToStr(SYSTEMTIME *Time); // 星期转化成字符
void DS1302_Stop(bit flag); //停止，开启时钟
void DS1302_Initial (void); //DS1302初始化
//写入一个字节(内部函数)
void DS1302_InputByte(unchar Data)
{
unchar i;
ACC = Data;
for(i=8; i>0; i--)
{
DS1302_CLK = 0;
DS1302_IO = ACC0; //由低位到高位
DS1302_CLK = 1; //上升沿写入数据
ACC >>= 1;
}
}
//读取一个字节(内部函数)
unchar DS1302_OutputByte(void)
{
unchar i;
for(i=8; i>0; i--)
{
DS1302_CLK = 0; //下降沿读出DS1302的数据
ACC >>= 1; //读出由低到高位,只能移7次有效位!!!
ACC7 = DS1302_IO; //第一个数据
DS1302_CLK = 1;
}
return (ACC);
}
//在指定地址写入指定的数据
void Write1302(unchar Address, unchar Data)
{
DS1302_RST = 0;
DS1302_CLK = 0;
DS1302_RST = 1;
DS1302_InputByte(Address);
DS1302_InputByte(Data);
DS1302_CLK = 1;
DS1302_RST = 0;
}
//读取1302指定地址中的数据
unchar Read1302(unchar Address)
{
unchar Data;
DS1302_RST = 0;
DS1302_CLK = 0;
DS1302_RST = 1; //RST拉高,启动数据传送
DS1302_InputByte(Address|0x01);//读取指定地址数据指令
Data = DS1302_OutputByte();
DS1302_CLK = 1;
DS1302_RST = 0;
DS1302_IO = 0;	//	加上这一行能解决随机显示85的问题！
return (Data);
}
//是否写入保护
void DS1302_SetProtect(bit Flag)
{
if(Flag)
Write1302(0x8E,0x80); //0x8e控制字节地址,bit7=WP WP=1 禁止数据写入DS1302
else
Write1302(0x8E,0x00); //WP=0 允许数据写入DS1302
}
//设置时间函数
void DS1302_SetTime(unchar Address, unchar Value)
{
DS1302_SetProtect(0);
Write1302(Address,((Value/10)<<4|(Value%10)));
DS1302_SetProtect(1);
}
//读出时间
void DS1302_GetTime(SYSTEMTIME *Time)
{
unchar ReadValue;
ReadValue = Read1302(DS1302_SECOND);
Time->Second = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0f);
ReadValue = Read1302(DS1302_MINUTE);
Time->Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0f);
ReadValue = Read1302(DS1302_HOUR);
Time->Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0f);
ReadValue = Read1302(DS1302_DAY);
Time->Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0f);
ReadValue = Read1302(DS1302_WEEK);
Time->Week = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0f);
ReadValue = Read1302(DS1302_MONTH);
Time->Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0f);
ReadValue = Read1302(DS1302_YEAR);
Time->Year = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0f);
}
//日期转化成字符
void DateToStr(SYSTEMTIME *Time)
{
Time->DateString[0] = Time->Year/10 + '0';
Time->DateString[1] = Time->Year%10 + '0';
Time->DateString[2] = '-';
Time->DateString[3] = Time->Month/10 + '0';
Time->DateString[4] = Time->Month%10 + '0';
Time->DateString[5] = '-';
Time->DateString[6] = Time->Day/10 + '0';
Time->DateString[7] = Time->Day%10 + '0';
Time->DateString[8] = '\0';
}
//时间转化成字符
void TimeToStr(SYSTEMTIME *Time)
{
Time->TimeString[0] = Time->Hour/10 + '0';
Time->TimeString[1] = Time->Hour%10 + '0';
Time->TimeString[2] = ':';
Time->TimeString[3] = Time->Minute/10 + '0';
Time->TimeString[4] = Time->Minute%10 + '0';
Time->TimeString[5] = ':';
Time->TimeString[6] = Time->Second/10 + '0';
Time->TimeString[7] = Time->Second%10 + '0';
Time->TimeString[8] = '\0';
}
// 星期转化为字符串
unchar* WeekToStr(SYSTEMTIME *Time)
{
	unint i;
	unchar *z;
	i = Time->Week;
	switch(i)
	{
		case 1: z = "SUN"; break;
		case 2: z = "MON"; break;
		case 3: z = "TUE"; break;
		case 4: z = "WEN"; break;
		case 5: z = "THU"; break;
		case 6: z = "FRI"; break;
		case 7: z = "SAT"; break;
	}
	return z;
}

//初始化DS1302
void DS1302_Initial (void)
{
unchar Second=Read1302(DS1302_SECOND);
if(Second&0x80)//bit7=CH CH=0 振荡器允许工作,CH=1振荡器停止工作
DS1302_SetTime(DS1302_SECOND,0);
}
//停止，开启时钟
void DS1302_Stop(bit flag)
{
	unchar Data;
	Data = Read1302(DS1302_SECOND);
	DS1302_SetProtect(0);
	if (flag)
		Write1302(DS1302_SECOND, Data | 0x80);
	else
		Write1302(DS1302_SECOND, Data & 0x7f);
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////