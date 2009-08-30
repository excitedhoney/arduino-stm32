/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

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

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_STM32_h
#define Pins_STM32_h

#include <stm32f10x_map.h>
#include <stm32f10x_gpio.h>

#define NOT_A_PIN 0
#define NOT_A_PORT 0
#define NOT_ON_TIMER 0

extern GPIO_TypeDef * digital_pin_to_port_PGM[];
extern uint8_t digital_pin_to_bitnumber[];

extern const TIM_TypeDef * digital_pin_to_timer_PGM[];
extern const uint8_t digital_pin_to_timer_chn[];

extern const GPIO_TypeDef * analog_chn_to_port[];
extern const uint8_t analog_chn_to_pin[];

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
// 
// These perform slightly better as macros compared to inline functions
//
#define digitalPinToPort(P) ( digital_pin_to_port_PGM[P] )
#define digitalPinToBit(P) ( digital_pin_to_bitnumber[P] )
#define digitalPinToTimer(P) ( digital_pin_to_timer_PGM[P] )
//#define analogInPinToBit(P) (P)
#define portOutputRegister(P) ( (volatile uint8_t *)( port_to_output_PGM[P]) )
#define portInputRegister(P) ( (volatile uint8_t *)( port_to_input_PGM[P]) )
#define portModeRegister(P) ( (volatile uint8_t *)( port_to_mode_PGM[P]) )

#endif
