/*
  pins_arduino.c - pin definitions for the Arduino board
  Part of Arduino / Wiring Lite

  Copyright (c) 2005 David A. Mellis

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

  $Id: pins_arduino.c 522 2008-11-01 22:06:13Z mellis $
*/

#include "wiring_private.h"
#include "pins_stm32.h"

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)


GPIO_TypeDef * digital_pin_to_port_PGM[] = {
	GPIOA, /* 0 = PA0 */
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA, /* 8 = PA8 */
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOB, /* 16 = PB0 */
	GPIOB,
	GPIOB,
	GPIOB,
	GPIOB,
	GPIOB,
	GPIOB,
	GPIOB,
	GPIOB, /* 24 = PB8 */
	GPIOB,
	GPIOB,
	GPIOB,
	GPIOB,
	GPIOB,
	GPIOB,
	GPIOB,
	GPIOC, /* 32 = PC0 */
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC, /* 40 = PC8 */
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC
};

uint8_t digital_pin_to_bitnumber[] = {
	0, /* 0, port A */
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8, /* 8, port A */
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	0, /* 16, port B */
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8, /* 24, port B */
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	0, /* 32, port C */
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8, /* 40, port C */
	9,
	10,
	11,
	12,
	13,
	14,
	15,
};

const TIM_TypeDef * digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, /* 0 - port A */
	TIM2,
	TIM2,
	TIM2,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIM3,
	TIM3,
	NOT_ON_TIMER, /* 8 - port A */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIM3, /* 0 - port B */
	TIM3,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIM4,
	TIM4,
	TIM4, /* 8 - port B */
	TIM4,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 0 - port C */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIM3, // FULL REMAP
	TIM3,
	TIM3, /* 8 - port C */
	TIM3,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

const uint8_t digital_pin_to_timer_chn[] = {
	NOT_ON_TIMER, /* 0 - port A */
	2,
	3,
	4,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	1,
	2,
	NOT_ON_TIMER, /* 8 - port A */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	3, /* 0 - port B */
	4,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	1,
	2,
	3, /* 8 - port B */
	4,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 0 - port C */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	31, // FULL REMAP
	32,
	33, /* 8 - port C */
	34,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

const GPIO_TypeDef * analog_chn_to_port[] = {
	GPIOA, /* 0 - port A */
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOB, 
	GPIOB,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOC
};

const uint8_t analog_chn_to_pin[] = {
	0, /* 0 - port A */
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	16, //PB0, 
	17, //PB1,
	32, //PC0
	33,
	34,
	35,
	36,
	37
};

