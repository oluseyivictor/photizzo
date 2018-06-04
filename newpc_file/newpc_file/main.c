/*
 * newpc_file.c
 *
 * Created: 5/28/2018 1:07:44 PM
 * Author : OLUSEYI
 */ 
#define F_CPU 16000000Ul
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "USART_irq.h"

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

//unsigned char Direction;

const unsigned char Smps_Table[60]={120, 118, 116, 114, 112, 110, 108, 106, 104, 102, 100, 98, 96, 94, 92, 90, 88, 86, 84, 82, 80, 78, 76, 74, 72,
	              70, 68, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14,
				  12, 10, 8, 6, 4, 2 , 0 };
				  
				  
const unsigned char MPPT_Table[41] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 
	26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80};
				  



uint32_t accumulator = 0;
int16_t value[6];
uint8_t count,channel,step,duty = 0;


	struct {
		uint8_t b0 : 1;
		uint8_t b1 : 1;
		uint8_t b2 : 1;
		uint8_t b3 : 1;
		uint8_t b4 : 1;
		uint8_t b5 : 1;
		uint8_t b6 : 1;
		uint8_t b7 : 1;
} TPort;

#define Direction TPort.b0
#define Updown TPort.b1

float sensitivity = 0.36786;
float Batt_Volt, Solar,N_Power,L_Power;
float current[4];
/* Static Variables */
static unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile unsigned char UART_RxHead;
static volatile unsigned char UART_RxTail;
static unsigned char UART_TxBuf[UART_TX_BUFFER_SIZE];
static volatile unsigned char UART_TxHead;
static volatile unsigned char UART_TxTail;
volatile uint8_t Sendflag = 0;
/* Prototypes */


#define LED_ON PORTB |= (1<<PORTB3)
#define LED_OFF PORTB &= ~(1<<PORTB3)
#define LED_TOGGLE PINB |= (1<<PINB3)



/* initialize UART */
void InitUART( unsigned char baudrate )
{
	unsigned char x;

	/*Set baud rate */
	UBRR0H = (unsigned char)(baudrate>>8);
	UBRR0L = (unsigned char)baudrate;
	/*Enable receiver and transmitter, Enable receive interrupt */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);

	x = 0; /* flush receive buffer */
	UART_RxTail = x;
	UART_RxHead = x;
	UART_TxTail = x;
	UART_TxHead = x;
}

ISR (USART_RX_vect)
{
	unsigned char data;
	unsigned char tmphead;
	data = UDR0; /* read the received data */
	/* calculate buffer index */
	tmphead = ( UART_RxHead + 1 ) & UART_RX_BUFFER_MASK;
	UART_RxHead = tmphead; /* store new index */
	if ( tmphead == UART_RxTail )
	{
		/* ERROR! Receive buffer overflow */
	}
	Sendflag = 1;
	UART_RxBuf[tmphead] = data; /* store received data in buffer */
}


ISR (USART_UDRE_vect)
{
	unsigned char tmptail;
	/* check if all data is transmitted */
	if ( UART_TxHead != UART_TxTail )
	{
		/* calculate buffer index */
		tmptail = ( UART_TxTail + 1 ) & UART_TX_BUFFER_MASK;
		UART_TxTail = tmptail; /* store new index */
		UDR0 = UART_TxBuf[tmptail]; /* start transmission */
	}
	else
	{
		UCSR0B &= ~(1<<UDRIE0); /* disable UDRE interrupt */
	}
}

/* Read and write functions */
unsigned char ReceiveByte( void )
{
	unsigned char tmptail;
	while ( UART_RxHead == UART_RxTail ) /* wait for incomming data */
	;
	tmptail = ( UART_RxTail + 1 ) & UART_RX_BUFFER_MASK;/* calculate buffer index */
	UART_RxTail = tmptail; /* store new index */
	return UART_RxBuf[tmptail]; /* return data */
}

void TransmitByte( unsigned char data )
{
	unsigned char tmphead;
	/* calculate buffer index */
	tmphead = ( UART_TxHead + 1 ) & UART_TX_BUFFER_MASK; /* wait for free space in buffer */
	while ( tmphead == UART_TxTail );
	UART_TxBuf[tmphead] = data; /* store data in buffer */
	UART_TxHead = tmphead; /* store new index */
	UCSR0B |= (1<<UDRIE0); /* enable UDRE interrupt */
}

//static int uart_putchar(char c, FILE *stream);

int TransmitByte_IO( char data,FILE *stream  )
{
	unsigned char tmphead;
	/* calculate buffer index */
	tmphead = ( UART_TxHead + 1 ) & UART_TX_BUFFER_MASK; /* wait for free space in buffer */
	while ( tmphead == UART_TxTail );
	UART_TxBuf[tmphead] = data; /* store data in buffer */
	UART_TxHead = tmphead; /* store new index */
	UCSR0B |= (1<<UDRIE0); /* enable UDRE interrupt */
return 0;     // this return value shouldn't be here 
}

unsigned char DataInReceiveBuffer( void )
{
	return ( UART_RxHead != UART_RxTail ); /* return 0 (FALSE) if the receive buffer is empty */
}

void USART_putstring(char* StringPtr){
	
	while(*StringPtr != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		TransmitByte(*StringPtr);    //Using the simple send function we send one char at a time
	StringPtr++;}        //We increment the pointer so we can read the next char
	
}



static FILE mystdout = FDEV_SETUP_STREAM(TransmitByte_IO, NULL,_FDEV_SETUP_WRITE);





ISR (TIMER1_OVF_vect)
{
	//if (Direction == 0xFF)
	if (Direction == 1)
	{
	
	


		 OCR1A = 0xFF;
		 //set PWM  for 25% duty cycle @ 16bit
		 OCR1B = Smps_Table[step];
		 //set PwM for 75% duty cycle @ 16bit 
		
		//Direction = 0x00;
		Direction = 0;
	}
	else{
		
	 OCR1B = 0xFF;
	 //set PWM  for 25% duty cycle @ 16bit
	 OCR1A = Smps_Table[step];
	 //set PwM for 75% duty cycle @ 16bit
		//Direction = 0xFF;
		Direction = 1;
	}

}


ISR (TIMER2_COMPA_vect)
{
	LED_ON;
	OCR2B =MPPT_Table[duty] ;

}
ISR(TIMER2_COMPB_vect)
{
	LED_OFF;
	
}


void adc_init()
{
	
	ADMUX  |= (1<<REFS0);     //SET VREF  TO  INTERNAL VCC
	ADCSRA  |=  (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // ENABLE INTERRUPTS  AND SET  THIS  TO 128  PRESCALER
}

uint16_t adc_read(uint8_t ch)
{
	ch &= 0b00000111;
	ADMUX  = (ADMUX & 0xF8) | ch;      // set  ADMUX  to  right adc  channel
	
	//START  SINGLE CONVERSION
	//WRITE 1 TO ADSC REGISTER
	ADCSRA |= (1<<ADSC);
	
	//WAIT TILL CONVERSION COMPLETE
	while(ADCSRA &  (1<<ADSC));
	return (ADC);
}


void Read_sample()
{
	for(channel=0; channel<6; channel++)
	{
		value[channel] = 0;
		accumulator = 0;
		for (count=0;  count<50; count++)
		{
			accumulator += adc_read(channel);
		}
		value[channel] = (accumulator/ 50);
	}
	 current[0] = (511-value[0])  * sensitivity;
	 current[1] = (511-value[1] )* sensitivity;
	 current[2]=(512-value[2])* sensitivity;
	 current[3]= (511-value[5]) * sensitivity;
	
	 Batt_Volt = (value[3] * 5.00)/ 1024;
	 Solar = (value[4] * 5.00)/ 1024;
	 
}


void Current_Feedback()
{
	if (current[1] < 10.0)         // Current Sense from SMPS
	{
		step++;
		if (step > 58) step = 58;
	}
	else
	{
		if (step > 0)
		{
			step --;
		}
	}

}

void mppt_PO()
{
	N_Power = Batt_Volt * current[0];
	if (N_Power < L_Power) Updown ^= 1;
	if(!Updown)
	{
		duty--;
		if (duty < 5) duty = 5;
	}
	else
	{
		duty++;
		if ( duty >= 40)  duty=40;
	}

	L_Power = N_Power;
}

void PWM1_Init()
{
		
		OCR1A = 0xFF;      //set PWM  for 25% duty cycle @ 16bit
		OCR1B = 0xFF;	    // set PWM for 50% duty cycle
		ICR1 = 177;           //45khz base frequency    //set top to 16bit
		TCCR1A |= (1 << COM1A1) | (1 << COM1B1)|(1 << COM1A0) | (1 << COM1B0) |(1 << WGM11);
		TCCR1B |= (1 << WGM12)|(1<<WGM13);       //SET as non inverting mode     // set as fast PWM
		TCCR1B |= (1 << CS10);                 // No prescale and start PWM2
		TIMSK1 |= (1<<TOIE1);               //enable timer 1 overflow interrupt
}

void PWM2_Init(uint16_t freq)
{

	TCCR2A |= (1 << WGM21);
	// Set to CTC Mode

	TIMSK2 |= (1 << OCIE2A)|(1 << OCIE2B);
	//Set interrupt on compare match

	TCCR2B |= (1 << CS21);
	// set prescaler to 64 and starts PWM

	OCR2A = freq;
	
}

int main(void)
{
	duty = 34;
	DDRB |= (1<<DDB1)|(1<<DDB0)|(1<<DDB2)|(1<<DDB3)| (1<<DDB5);
	DDRD |= (1<<DDD6)|(1<<DDD5)|(1<<DDD3);
	
	//PORTB &= ~(1<<PORTB0);
	 InitUART(MYUBRR); /* set the baudrate to 19,200 bps using a 3.6864MHz crystal */
	 stdout = &mystdout;
	
	adc_init();
     PWM2_Init(82);                           // OCR2A =(F_CPU/(freq * 8)-1);
	 PWM1_Init();

	 sei();                          //enable global interrupt
	
	
    /* Replace with your application code */
    while (1) 
    {
 
  //if (Sendflag)
  if (1)
  {
	  printf("%4.2f,%2.2f,%4.2f,%4.2f,%4.2f,%4.2f\n\r", current[0], current[1], current[2], current[3], Batt_Volt, Solar);
	  Sendflag = 0;
	  
  }
  
  Read_sample();
Current_Feedback();
//mppt_PO();
  
//step=27;
 

_delay_ms(1000);
    }
}

