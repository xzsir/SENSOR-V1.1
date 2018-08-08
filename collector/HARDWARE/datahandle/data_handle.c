/***************************************
             ���ݴ������
****************************************/



#include "data_handle.h"
#include "usart.h"	
#include "string.h"
#include <string.h>


extern char ID1;
extern char ID2 ;

/*************************
��������int2str
�������ܣ�������ת��Ϊ�ַ���
����������n�������ַ���ָ��
��������ֵ����

����ʹ��sprintf����������itoa����
**************************/

void int2str(int n, char *str)
{
    char buf[10] = "";
    int i = 0;
    int len = 0;
    int temp = n < 0 ? -n: n;  // tempΪn�ľ���ֵ
  
    if (str == NULL)
   {
       return;
   }
   while(temp)
   {
       buf[i++] = (temp % 10) + '0';  //��temp��ÿһλ�ϵ�������buf
       temp = temp / 10;
   }
 
   len = n < 0 ? ++i: i;  //���n�Ǹ����������Ҫһλ���洢����
   str[i] = 0;            //ĩβ�ǽ�����0
   while(1)
   {
       i--;
       if (buf[len-i-1] ==0)
       {
           break;
       }
       str[i] = buf[len-i-1];  //��buf��������ַ������ַ���
   }
   if (i == 0 )
   {
        str[i] = '-';          //����Ǹ��������һ������
   }
 }

 
 /*************************
��������data_pack
�������ܣ����Ѽ��������ݴ��
��������������ָ���Դ�Ϊ���¶ȣ����ٶȣ�Ƶ�ʣ�����õ��ַ���ָ��
��������ֵ����

����ʹ��strcat�������ַ���ƴ�Ӻ���
 
itoa�������Ǳ�׼��ĺ���
 **************************/
 
 void data_pack(short *temp,short *acc,short *fre,char *pack)
 { 
    char s_temp[10]={0};
    char s_acc[10]={0};
    char s_fre[10]={0};
    char s_id1[10]={0};
    char s_id2[10]={0};
		
	  int2str(ID1, s_id1);
		int2str(ID2, s_id2);
	 	int2str(*(temp+0), s_temp);
		int2str(*(acc+0), s_acc);
		int2str(*(fre+0), s_fre);
		

		strcat (pack,"$>");
		strcat (pack,"I1");
	  strcat (pack,s_id2);
		strcat (pack,"I2");
	  strcat (pack,s_id2);
		strcat (pack,"t");
		strcat (pack,s_temp);
		strcat (pack,"g");
		strcat (pack,s_acc);
	  strcat (pack,"f");
		strcat (pack,s_fre);
    strcat (pack,"< ");
 
 }
 

