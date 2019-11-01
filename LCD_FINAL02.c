#include "LCD.h"

void flash_enable(void)
{
	CONTROL_PIN_PORT |= 1<<ENABLE;
	asm volatile ("nop");
	asm volatile ("nop");
	//_delay_us(10);
	CONTROL_PIN_PORT &= ~(1<<ENABLE);
}

void send_char(unsigned char character)
{
	lcd_busy();
	DATA_PORT=character;
	CONTROL_PIN_PORT |= (1<<CMD_CHAR);
	CONTROL_PIN_PORT &= ~(1<<W_R);
	flash_enable();
	DATA_PORT=0x00;
	_delay_us(800);
}

void send_command(unsigned char command)
{
	lcd_busy();
	DATA_PORT=command;
	CONTROL_PIN_PORT &= ~(1<<CMD_CHAR);
	CONTROL_PIN_PORT &= ~(1<<W_R);
	flash_enable();
	DATA_PORT=0x00;
	_delay_us(800);
}

void lcd_busy(void)
{
	DDR_DATA_PORT = 0;
	CONTROL_PIN_PORT &= ~(1<<CMD_CHAR);
	CONTROL_PIN_PORT |= (1<<W_R);
	
	while(DATA_PORT >= 0X80)
	//while(bit_is_clear(DATA_READ,D7))
	{
		flash_enable();
	}
	DDR_DATA_PORT = 0xFF;
}

void send_string(char *string)
{
	while(*string > 0)
	{
		send_char(*string);
		string++;
	}
}

void config_LCD(void)
{
	_delay_ms(15);
	DDR_CONTROL_PIN |= (1<<ENABLE)|(1<<CMD_CHAR)|(1<<W_R);
	_delay_ms(100);
	send_command(0x01);//cls screen
	send_command(0x38);//8 bit 2 line mode
	_delay_ms(10);
	send_command(0x38);//8 bit 2 line mode
	_delay_ms(10);
	send_command(0x38);//8 bit 2 line mode
	_delay_ms(10);
	send_command(0b00001110);//LCD on ,cursor on,blink off
}

void send_double(double numb)
{
	char a[10];
	dtostrf(numb,8,2,a);
	send_string(a);
}

void send_integer(double numb)
{
	char b[10];
	itoa(numb,b,10);
	send_string(b);
}

void goto_location(int x,int y)
{
	if (y == 2)
	{
		send_command(0x80 + 64 + x -1);
	}
	else if (y == 1)
	{
		send_command(0x80 + x -1);
	}
}