
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "USART_128.h"
#include <string.h>

char globe_RFID_data[12];

/*
3389742-.650033B92E(0x0D)
2377548-.650024474C0x0D
2373131-.650024360B0x0D
2381661-.650024575D0x0D
2370845-.6500242D1D0x0D
!
*/
void copy_string(char *a,char *b,int start)
{
	int c;
	for (c = 0;c<12;c++)
	{
		*(a+c) = *(b+c+start);
	}
}

int compare_string(char *a,char *b)
{
	int c;
	for (c = 0;c<12;c++)
	{
		if (*(a+c) != *(b+c))
		{
		return 1;
		}
	}
	return 0;
}

void check_RFID(void)
{
	int check,m;
	char RFID_data_check[5][12] = 
	{
		{0x0A,'6','5','0','0','3','3','B','9','2','E',0x0D},
		{0x0A,'6','5','0','0','2','4','4','7','4','C',0X0D},
		{0x0A,'6','5','0','0','2','4','3','6','0','B',0X0D},
		{0x0A,'6','5','0','0','2','4','5','7','5','D',0X0D},
		{0x0A,'6','5','0','0','2','4','2','D','1','D',0X0D},
	};
	char RFID_data_check_temp[12];
	send_command(0x01);	

	check = 1;
	
	for (m=0;(check != 0) && (m < 5);m++)
	{
		copy_string(RFID_data_check_temp,RFID_data_check,(m)*12);
		check = compare_string(globe_RFID_data,RFID_data_check_temp);
		if ((m == 4)&&(check != 0))
		{
			m = 5;
			break;
		}
	}
	send_command(0x01);
	
	switch(m-1)
	{
		case 0 :
		{
			goto_location(1,1);
			send_string("Hello Meet");
			goto_location(1,2);
			send_string("card 1");
			break;
		}
		case 1 :
		{
			goto_location(1,1);
			send_string("Hello Dhruv");
			goto_location(1,2);
			send_string("card 2");
			break;
		}
		case 2 :
		{
			goto_location(1,1);
			send_string("Hello siddarth");
			goto_location(1,2);
			send_string("card 3");
			break;
		}
		case 3 :
		{
			goto_location(1,1);
			send_string("XYZ");
			goto_location(1,2);
			send_string("card 4");
			break;
		}
		case 4:
		{	
			goto_location(1,1);
			send_string("ABC");								
			goto_location(1,2);
			send_string("card 5");
			break;
		}
		case 5:
		{
			goto_location(1,2);
			send_string("invalid card");
			break;
		}
	}
	_delay_ms(4000);
}

int main(void)
{
	int n;
	char RFID_data[12];
	config_LCD();
	USART_Init(51,1);
	send_command(0x01);
	goto_location(1,1);
	send_string(" **** HELLO ****");
	goto_location(1,2);
	send_string("RFID CARD READER");
	_delay_ms(2000);
	send_command(0x01);
	goto_location(1,1);
	send_string("MAKERSPACE 2017");
	goto_location(1,2);
	send_string(" **WELCOME**");
	_delay_ms(2000);
	while(1)
	{
	send_command(0X01);
	goto_location(1,1);
	send_string("Please Place the");
	goto_location(1,2);
	send_string("RFID card");
	UCSR1B|=(1<<RXEN1);
	UCSR1B&=~(1<<RXEN1);
	UCSR1B|=(1<<RXEN1);
	for(n=0;n<12;n++)
	{
		RFID_data[n] = USART_Receive(1);
	}
	UCSR1B&=~(1<<RXEN1);
	send_command(0x01);
	goto_location(1,1);
	send_string("CARD ID is :-");
	goto_location(1,2);
	send_string(RFID_data);
	strcpy(globe_RFID_data,RFID_data);
	_delay_ms(2000);
	check_RFID();
	}
	
}