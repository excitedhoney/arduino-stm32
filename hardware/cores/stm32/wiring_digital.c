/*
  wiring_digital.c - digital input and output functions
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $
*/

#include "wiring_private.h"
#include "pins_stm32.h"

/* 
Mode values and pin output configurations
0	INPUT	Floating input -> converted to 4
1	OUTPUT	Push pull output, 2MHz 
2			Push pull output, 10MHz 
3			Push pull output, 50MHz 
4			Floating input
5			Open drain output, 2MHz 
6			Open drain output, 10MHz 
7			Open drain output, 50MHz 


*/
void pinMode(uint8_t pin, uint8_t mode)
{
	uint8_t bit = digitalPinToBit(pin);
	uint8_t modeoffs = (4*bit)%32;
	vu32 * pCR;
	GPIO_TypeDef * port = digitalPinToPort(pin);

	// If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
//	uint8_t timer = digitalPinToTimer(pin);
//	if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	if (port == NOT_A_PIN) return;

	if (bit<8)
		 pCR = &port->CRL;
	else
		 pCR = &port->CRH;

	if (mode == INPUT) 
	{
		*pCR = ((*pCR) & ~(0xF<<modeoffs)) | (0x4<<modeoffs); /* Floating input */
	}
	else if (mode == ANALOG) 
	{
		*pCR = ((*pCR) & ~(0xF<<modeoffs)) | (0x0<<modeoffs); /* Analog input */
	}
	else
	{
		*pCR = ((*pCR) & ~(0xF<<modeoffs)) | ((0xF&mode)<<modeoffs);
	}
}

// Forcing this inline keeps the callers from having to push their own stuff
// on the stack. It is a good performance win and only takes 1 more byte per
// user than calling. (It will take more bytes on the 168.)
//
// But shouldn't this be moved into pinMode? Seems silly to check and do on
// each digitalread or write.
//
static inline void turnOffPWM(uint8_t timer) __attribute__ ((always_inline));
static inline void turnOffPWM(uint8_t timer)
{
#if 0
	if (timer == TIMER1A) cbi(TCCR1A, COM1A1);
	if (timer == TIMER1B) cbi(TCCR1A, COM1B1);

#if defined(__AVR_ATmega8__)
	if (timer == TIMER2) cbi(TCCR2, COM21);
#else
	if (timer == TIMER0A) cbi(TCCR0A, COM0A1);
	if (timer == TIMER0B) cbi(TCCR0A, COM0B1);
	if (timer == TIMER2A) cbi(TCCR2A, COM2A1);
	if (timer == TIMER2B) cbi(TCCR2A, COM2B1);
#endif
#endif
}

void __attribute__((optimize(3))) digitalWrite(uint8_t pin, uint8_t val)
{
	uint8_t bit = digitalPinToBit(pin);
	GPIO_TypeDef * port = digitalPinToPort(pin);

	if (port == NOT_A_PIN) return;

	if (val == LOW) port->BRR = (1<<bit);
	else port->BSRR = (1<<bit);
}

int __attribute__((optimize(3)))  digitalRead(uint8_t pin)
{
	uint8_t bit = digitalPinToBit(pin);
	GPIO_TypeDef * port = digitalPinToPort(pin);

	if (port == NOT_A_PIN) return LOW;

	if (port->IDR & 1<<bit) return HIGH;
	return LOW;
}
