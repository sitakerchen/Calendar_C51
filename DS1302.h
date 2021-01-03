///////////////////////////////////////////////////////////////////////////////////////////////////////
//File : DS1302.H
//Author : NULL, cxl hunau
//Created: 2020/12/31
#ifndef __DS1302_A_H__
#define __DS1302_A_H__
#include <reg52.h>
#include <intrins.h>
//���ż�λ�Ķ���
sbit DS1302_CLK = P1^0; //ʱ������
sbit DS1302_IO = P1^1; //��������
sbit DS1302_RST = P1^2; //��λ����
sbit ACC0 = ACC^0;
sbit ACC7 = ACC^7;
//�궨��������
#define unchar unsigned char
#define unint unsigned int
#define DS1302_SECOND 0x80
#define DS1302_MINUTE 0x82
#define DS1302_HOUR 0x84
#define DS1302_WEEK 0x8A
#define DS1302_DAY 0x86
#define DS1302_MONTH 0x88
#define DS1302_YEAR 0x8C
#define DS1302_RAM(X) (0xC0+(X)*2) //���ڼ���RAM��ַ�ĺ�����
//����ʱ������
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
//��������
void DS1302_InputByte(unchar Data); //д��һ���ֽ�(�ڲ�����)
unchar DS1302_OutputByte(void); //��ȡһ���ֽ�(�ڲ�����)
void Write1302(unchar Address, unchar Data); //��ָ����ַд��ָ��������
unchar Read1302(unchar Address); //��ȡDS1302ָ����ַ�е�����
void DS1302_SetProtect(bit Flag); //DS1302�Ƿ�д�뱣��
void DS1302_SetTime(unchar Address, unchar Value);//����ʱ�亯��
void DS1302_GetTime(SYSTEMTIME *Time); //����ʱ��
void DateToStr(SYSTEMTIME *Time); //����ת�����ַ�
void TimeToStr(SYSTEMTIME *Time); //ʱ��ת�����ַ�
unchar* WeekToStr(SYSTEMTIME *Time); // ����ת�����ַ�
void DS1302_Stop(bit flag); //ֹͣ������ʱ��
void DS1302_Initial (void); //DS1302��ʼ��
//д��һ���ֽ�(�ڲ�����)
void DS1302_InputByte(unchar Data)
{
unchar i;
ACC = Data;
for(i=8; i>0; i--)
{
DS1302_CLK = 0;
DS1302_IO = ACC0; //�ɵ�λ����λ
DS1302_CLK = 1; //������д������
ACC >>= 1;
}
}
//��ȡһ���ֽ�(�ڲ�����)
unchar DS1302_OutputByte(void)
{
unchar i;
for(i=8; i>0; i--)
{
DS1302_CLK = 0; //�½��ض���DS1302������
ACC >>= 1; //�����ɵ͵���λ,ֻ����7����Чλ!!!
ACC7 = DS1302_IO; //��һ������
DS1302_CLK = 1;
}
return (ACC);
}
//��ָ����ַд��ָ��������
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
//��ȡ1302ָ����ַ�е�����
unchar Read1302(unchar Address)
{
unchar Data;
DS1302_RST = 0;
DS1302_CLK = 0;
DS1302_RST = 1; //RST����,�������ݴ���
DS1302_InputByte(Address|0x01);//��ȡָ����ַ����ָ��
Data = DS1302_OutputByte();
DS1302_CLK = 1;
DS1302_RST = 0;
DS1302_IO = 0;	//	������һ���ܽ�������ʾ85�����⣡
return (Data);
}
//�Ƿ�д�뱣��
void DS1302_SetProtect(bit Flag)
{
if(Flag)
Write1302(0x8E,0x80); //0x8e�����ֽڵ�ַ,bit7=WP WP=1 ��ֹ����д��DS1302
else
Write1302(0x8E,0x00); //WP=0 ��������д��DS1302
}
//����ʱ�亯��
void DS1302_SetTime(unchar Address, unchar Value)
{
DS1302_SetProtect(0);
Write1302(Address,((Value/10)<<4|(Value%10)));
DS1302_SetProtect(1);
}
//����ʱ��
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
//����ת�����ַ�
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
//ʱ��ת�����ַ�
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
// ����ת��Ϊ�ַ���
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

//��ʼ��DS1302
void DS1302_Initial (void)
{
unchar Second=Read1302(DS1302_SECOND);
if(Second&0x80)//bit7=CH CH=0 ����������,CH=1����ֹͣ����
DS1302_SetTime(DS1302_SECOND,0);
}
//ֹͣ������ʱ��
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