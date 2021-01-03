/*******************************************************************
*                                                                  *
* 文件： KeyScan.h                                                     *
* 作者： cxl Hunau                                                  *
* 邮箱： sitakerchen@gmail.com                                       *
*                                                                  *
*【版权】Copyright(C)cxl  All Rights Reserved  *
*【声明】此程序仅用于学习与参考，引用请注明版权和作者信息！        *
*                                                                  *
********************************************************************/
#ifndef  __KeyScan_h__                //条件汇编指令
#define  __KeyScan_h__                //避免重复调用
#define uchar unsigned char
#define uint  unsigned int


sbit k1 = P3^2;	//	button 1
sbit k2 = P3^3;	//	button 2
sbit k3 = P3^4;
sbit k4 = P3^5;

void delay(uint ms)
{
	uchar k;
  	while(ms--)
  	{
    	for(k = 0; k < 110; k++);
  	}
}

uint read_key()		//	独立按键扫描
{
	if (k1 == 0)
	{
	 	delay(5);
		if (k1 == 0)
		{
			while (!k1);
			return 1;
		}
	}
	if (k2 == 0)
	{
	 	delay(5);
		if (k2 == 0)
		{
			while (!k2);
			return 2;
		}
	}
	if (k3 == 0)
	{
	 	delay(5);
		if (k3 == 0)
		{
			while (!k3);
			return 3;
		}
	}
	if (k4 == 0)
	{
	 	delay(5);
		if (k4 == 0)
		{
			while (!k4);
			return 4;
		}
	}	
	return 0;		
}

#endif                            //条件汇编结束