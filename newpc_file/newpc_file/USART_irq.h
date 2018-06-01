/*
 * USART_irq.h
 *
 * Created: 5/30/2018 10:01:15 PM
 *  Author: OLUSEYI
 */ 


#ifndef USART_IRQ_H_
#define USART_IRQ_H_



/* UART Buffer Defines */
#define UART_RX_BUFFER_SIZE 128 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART_TX_BUFFER_SIZE 128 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1 )
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1 )


void InitUART( unsigned char baudrate );
unsigned char ReceiveByte( void );
void TransmitByte( unsigned char data );
void USART_putstring(char* StringPtr);
int TransmitByte_IO( char data,FILE *stream  );




#endif /* USART_IRQ_H_ */