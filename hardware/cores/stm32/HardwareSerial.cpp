/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Modified 23 November 2006 by David A. Mellis
  Modified 27 July 2009 by Magnus Lundin
*/

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "wiring.h"
#include "wiring_private.h"

#include "HardwareSerial.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_usart.h"
//#include "stm32f10x_nvic.h"
#define USART_Enable 0x2000
#define USART_RXNEIE 0x0020

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which rx_buffer_head is the index of the
// location to which to write the next incoming character and rx_buffer_tail
// is the index of the location from which to read.
#define RX_BUFFER_SIZE 128

struct ring_buffer {
  unsigned char buffer[RX_BUFFER_SIZE];
  int head;
  int tail;
};

ring_buffer rx_buffer1 = { { 0 }, 0, 0 };
ring_buffer rx_buffer2 = { { 0 }, 0, 0 };
ring_buffer rx_buffer3 = { { 0 }, 0, 0 };

inline void store_char(unsigned char c, ring_buffer *rx_buffer)
{
	int i = (rx_buffer->head + 1) % RX_BUFFER_SIZE;

	// if we should be storing the received character into the location
	// just before the tail (meaning that the head would advance to the
	// current location of the tail), we're about to overflow the buffer
	// and so we don't write the character or advance the head.
	if (i != rx_buffer->tail) {
		rx_buffer->buffer[rx_buffer->head] = c;
		rx_buffer->head = i;
	}
}


void USART1_IRQHandler(void)
{
	unsigned char c = USART1->DR;
	store_char(c, &rx_buffer1);
}

void USART2_IRQHandler(void)
{
	unsigned char c = USART2->DR;
	store_char(c, &rx_buffer2);
}

void USART3_IRQHandler(void)
{
	unsigned char c = USART3->DR;
	store_char(c, &rx_buffer3);
}


// Constructors ////////////////////////////////////////////////////////////////

HardwareSerial::HardwareSerial(ring_buffer *rx_buffer,
  USART_TypeDef * usart,
  uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udre)
{
  _rx_buffer = rx_buffer;
  _usart = usart;
  _rxen = rxen;
  _txen = txen;
  _rxcie = rxcie;
  _udre = udre;
}

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(long speed)
{
	// Enable perhipherial clock
	if (_usart == USART1 )
	{
		RCC->APB2ENR = RCC_APB2ENR_USART1EN;
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		NVIC->ISER[USART1_IRQn/32] = (1<<(USART1_IRQn%32));
		// Configure peripherial pins, TX - PA9, RX - PA10
		GPIOA->CRH = (GPIOA->CRH & 0xFFFFF00F) | 0x000004A0;
	}
	if (_usart == USART2 )
	{
		// USART2 runs on APB1 clock which is HCLK/2 = 36 MHz
		RCC->APB1ENR = RCC_APB1ENR_USART2EN;
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		NVIC->ISER[USART2_IRQn/32] = (1<<(USART2_IRQn%32));
		speed = speed*2;
		// Configure peripherial pins, TX - PA2, RX - PA3
		GPIOA->CRL = (GPIOA->CRL & 0xFFFF00FF) | 0x00004A00;
	}
	if (_usart == USART3 )
	{
		// USART3 runs on APB1 clock which is HCLK/2 = 36 MHz
		RCC->APB1ENR = RCC_APB1ENR_USART3EN;
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		NVIC->ISER[USART3_IRQn/32] = (1<<(USART3_IRQn%32));
		speed = speed*2;
		// Configure peripherial pins, TX - PB10, RX - PB11
		GPIOB->CRH = (GPIOB->CRH & 0xFFFF00FF) | 0x00004A00;
	}
	// Configure USART
	_usart->CR1 = 0;
	_usart->CR2 = 0;
	_usart->CR3 = 0;
	_usart->SR = 0;
	_usart->BRR = MCK / speed;
	_usart->CR1 = USART_CR1_RE | USART_CR1_TE | USART_RXNEIE | USART_CR1_UE;

}

uint8_t HardwareSerial::available(void)
{
	return (RX_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % RX_BUFFER_SIZE;
}

int HardwareSerial::read(void)
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (_rx_buffer->head == _rx_buffer->tail) {
		return -1;
	} else {
		unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
		_rx_buffer->tail = (_rx_buffer->tail + 1) % RX_BUFFER_SIZE;
		return c;
	}
}

void HardwareSerial::flush()
{
	// don't reverse this or there may be problems if the RX interrupt
	// occurs after reading the value of rx_buffer_head but before writing
	// the value to rx_buffer_tail; the previous value of rx_buffer_head
	// may be written to rx_buffer_tail, making it appear as if the buffer
	// don't reverse this or there may be problems if the RX interrupt
	// occurs after reading the value of rx_buffer_head but before writing
	// the value to rx_buffer_tail; the previous value of rx_buffer_head
	// may be written to rx_buffer_tail, making it appear as if the buffer
	// were full, not empty.
	_rx_buffer->head = _rx_buffer->tail;
}

void HardwareSerial::write(uint8_t c)
{
	while (!(_usart->SR & USART_SR_TXE))
		;

	_usart->DR = c;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

HardwareSerial Serial1(&rx_buffer1, USART1, 0, 0, 0, 0);
HardwareSerial Serial2(&rx_buffer2, USART2, 0, 0, 0, 0);
HardwareSerial Serial3(&rx_buffer3, USART3, 0, 0, 0, 0);

