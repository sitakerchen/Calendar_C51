/*******************************************************************
*                                                                  *
* 文件： LCD1602.h                                                     *
*                                                                  *
* 作者： gguoqing, , cxl Hunau                                                  *
* 网站： www.willar.com(伟纳电子)                                  *
* 邮箱： gguoqing@willar.com                                       *
*                                                                  *
*【版权】Copyright(C)伟纳电子 www.willar.com  All Rights Reserved  *
*【声明】此程序仅用于学习与参考，引用请注明版权和作者信息！        *
*                                                                  *
********************************************************************/
#ifndef  __lcd_h__                //条件汇编指令
#define  __lcd_h__                //避免重复调用
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
#endif                            //条件汇编结束