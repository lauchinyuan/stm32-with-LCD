#include "inoutput.h"
#include "usart.h"

/*************************************************
Function: fputc
Description: fputc 重定向
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
int fputc(int ch, FILE *f)
   {
      USART_SendData(USART_Used, (unsigned char) ch);
      while (!(USART_Used->SR & USART_FLAG_TXE));
      USART_ClearFlag(USART_Used, USART_FLAG_TXE); 
      return (ch);
   }
   
   
/*接收数据*/
int GetKey (void)  
{
      while (!(USART_Used->SR & USART_FLAG_RXNE));
      USART_ClearFlag(USART_Used, USART_FLAG_RXNE);
      return ((int)(USART_Used->DR & 0x1FF));
}
 

