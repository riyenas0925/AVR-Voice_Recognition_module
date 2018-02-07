#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
unsigned char rx_data[50] ={0};

void usart_init(unsigned int UBRR);
void tx_out(unsigned char data);
unsigned char rx_in(void);
void voice_module_init(void);
void voice_record(int k);	//7개중 2개만 녹음할수 있게 설정
void voice_load(void);

void usart_init(unsigned int UBRR)
{
	UCSRC=0x86;	//no parity, stop bit 1, data 8bit, asynchronus 
	UBRRH=UBRR >> 8;
	UBRRL=UBRR;	//Baud 9600으로 설정
	UCSRB=(1<<TXEN)|(1<<RXEN);	//TX, RX Enable
}

void tx_out(unsigned char data)
{
	while(!(UCSRA&(1<<UDRE)));
	UDR = data;
}

unsigned char rx_in(void)
{
	while(!(UCSRA&(1<<RXC)));
	return UDR;
}

void voice_module_init(void)
{
	tx_out(0xAA);
	tx_out(0x02);
	tx_out(0x31);
	tx_out(0x0A);
}

void voice_record(int voice_count)	//80개중 2개만 녹음할수 있게 설정
{
	int i, data;

	tx_out(0xAA);
	tx_out(0x03 + voice_count - 1);
	tx_out(0x20);

	for(i = 0; i < voice_count; i++)
	{
		tx_out(0x00 + i);
	}
	
	tx_out(0x0A);

	while(1)
	{
		data = rx_in();

		if(data == (0x00 + voice_count - 1))
		{
			for(i = 0; data != 0x0D; i++)
			{
				data = rx_in();
				rx_data[i] = data;
			}
		}
	
		if((rx_data[0] == 0x53)&&(rx_data[1] == 0x75))
		{
			PORTC =~ PORTC;
			break;
		}
	}
}

void voice_load(void)	//80개의 커맨드중 7개를 인식기로 불러들인다.
{
	tx_out(0xAA);
	tx_out(0x09);
	tx_out(0x30);
	tx_out(0x00);
	tx_out(0x01);
	tx_out(0x02);
	tx_out(0x03);
	tx_out(0x04);
	tx_out(0x05);
	tx_out(0x06);
	tx_out(0x0A);
}

int main(void)
{
	DDRC = 0xFF;
	usart_init(103);
	/*
	voice_module_init();
	_delay_ms(100);
	voice_record(2);
	*/
	_delay_ms(100);
	voice_load();

	while(1)
	{
		unsigned char data;
		int i;

		data = rx_in();

		if(data == 0xAA)
		{
			for(i = 0; data != 0x0A; i++)
			{
				data = rx_in();
				rx_data[i] = data;
			}
		}
		
		if(rx_data[5] == 0x00)
		{
			PORTC = 0xFF;
		}

		if(rx_data[5] == 0x01)
		{
			PORTC = 0x00;
		}
	}
}