/*******************************************************************
*                                                                  *
* �ļ��� LCD1602.h                                                     *
*                                                                  *
* ���ߣ� gguoqing, , cxl Hunau                                                  *
* ��վ�� www.willar.com(ΰ�ɵ���)                                  *
* ���䣺 gguoqing@willar.com                                       *
*                                                                  *
*����Ȩ��Copyright(C)ΰ�ɵ��� www.willar.com  All Rights Reserved  *
*���������˳��������ѧϰ��ο���������ע����Ȩ��������Ϣ��        *
*                                                                  *
********************************************************************/
#ifndef  __lcd_h__                //�������ָ��
#define  __lcd_h__                //�����ظ�����
#define uchar unsigned char
#define uint  unsigned int
#define FLine 0x80
#define SLine 0xc0
extern  void delayms(uint ms);
extern  bit lcd_busy();
extern  void lcd_wcmd(uchar cmd);
extern  void lcd_wdat(uchar dat);
extern  void lcd_init();
extern  void lcd_pos(uchar pos);
extern 	void lcd_disString(uchar ad, uchar *s, uint ifDelay);
#endif                            //����������