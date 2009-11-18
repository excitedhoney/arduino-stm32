/*
  wiring_analog.c - analog input and output
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
//#include "stm32f10x_adc.h"
//#include "stm32f10x_tim.h"

#define ADC_SR_EOC  (1<<1)
#define ADC_CR1_DISCEN  (1<<11)
#define ADC_CR1_SCAN  (1<<8)
#define ADC_CR2_TSVREFE (1<<23)
#define ADC_CR2_SWSTART (1<<22)
#define ADC_CR2_DMA (1<<8)
#define ADC_CR2_RSTCAL (1<<3)
#define ADC_CR2_CAL (1<<2)
#define ADC_CR2_CONT (1<<1)
#define ADC_CR2_ADON (1<<0)

uint8_t analog_reference = DEFAULT;
uint8_t pwm_resolution = PWM8BIT;

void configTimerFreq(TIM_TypeDef * TIM, uint16_t maxcount)
{
	TIM->CR1 &= 1;
	TIM->CR2 = 0;
	TIM->PSC = 0;
	TIM->SMCR = 0;
	/* 12 bit PWM mode */
	/* PWM frequency is 72kHz/4096 */	
	TIM->ARR = 0xFFF; 

	/* Counter enable */
	TIM->CR1 |= 1;
};

void configTimerChannelPWM(TIM_TypeDef * TIM, uint8_t chn)
{
	uint32_t ccmr;

	/* Output compare disable */
	TIM->CCER &= ~(0xF<<(4*chn));

	/* Configure Output Compare */
	ccmr = *((uint32_t *)(&TIM->CCMR1)+(chn>>1));
	ccmr &=  ~(0xFF<<(8*(chn&1)));
	ccmr |= ( (TIM_CCMR_OCMode_PWM1 | TIM_CCMR_OCPreload_Enable)<<(8*(chn&1)) );
	*((uint32_t *)(&TIM->CCMR1)+(chn>>1)) = ccmr;

	/* Output compare enable */
	TIM->CCER |= (0x1<<(4*chn));

};

void setPWMResolution(uint8_t bits)
{
	pwm_resolution = bits;
}

void analogCalibrate()
{
    ADC1->CR2 |= ADC_CR2_RSTCAL; /* Reset calibration */
    while (ADC1->CR2 & ADC_CR2_RSTCAL); /* Wait for end of calibration reset */
    ADC1->CR2 |= ADC_CR2_CAL; /* Start calibration */
    while (ADC1->CR2 & ADC_CR2_CAL); /* Wait for end of calibration */
}


void analogEnable(int enable)
{
	if (enable==true)
	{
    	ADC1->CR2 |= ADC_CR2_ADON;
    }
    else
	{
    	ADC1->CR2 &= ~ADC_CR2_ADON;
    }    
}


void analogReference(uint8_t mode)
{
	// can't actually set the register here because the default setting
	// will connect AVCC and the AREF pin, which would cause a short if
	// there's something connected to AREF.
	analog_reference = mode;
}

int analogRead(uint8_t pin)
{
	uint8_t portpin = analog_chn_to_pin[pin];
	pinMode(portpin, ANALOG);

	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 (the default).
	
	ADC1->SQR3 = pin&0xF; /* Sample channel */

	// start the conversion
	ADC1->CR2 |= ADC_CR2_ADON;
	
	//  is cleared when the conversion finishes
    while (! ADC1->SR & ADC_SR_EOC); /* Wait for end of conversion */

	return ADC1->DR;
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint8_t pin, int val)
{
	TIM_TypeDef * TIM;
	uint8_t chn;

	TIM = digitalPinToTimer(pin);
	chn = digital_pin_to_timer_chn[pin];
	chn = chn-1;
	
	// We need to make sure the PWM output is enabled for those pins
	// that support it, as we turn it off when digitally reading or
	// writing with them.  Also, make sure the pin is in output mode
	// for consistenty with Wiring, which doesn't require a pinMode
	// call for the analog output pins.

	if (TIM != 0) {
		if (curPinMode[pin] != ALTOUT_PP)
		{
			pinMode(pin, ALTOUT_PP);	
			// Setup timer - 12 bit PWM
			// Frequency is 72/4096 MHz
			configTimerFreq(TIM, 0xFFF);
			configTimerChannelPWM(TIM, chn);
		}

		// set pwm duty cycle
		*((uint32_t *)(&TIM->CCR1)+(chn)) = (val<<(PWM12BIT-pwm_resolution));

	} else
	{
		pinMode(pin, OUTPUT);	
		if (val < 128)
			digitalWrite(pin, LOW);
		else
			digitalWrite(pin, HIGH);
	}
}
