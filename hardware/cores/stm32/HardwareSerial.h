/*
  HardwareSerial.h - Hardware serial library for Wiring
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
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>

#include "Print.h"
#include "stm32f10x_map.h"

struct ring_buffer;

class HardwareSerial : public Print
{
  private:
    ring_buffer *_rx_buffer;
    USART_TypeDef * _usart;
    uint8_t _rxen;
    uint8_t _txen;
    uint8_t _rxcie;
    uint8_t _udre;
  public:
    HardwareSerial(ring_buffer *rx_buffer,
	  USART_TypeDef * usart,
      uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udre);
    void begin(long);
    uint8_t available(void);
    int read(void);
    void flush(void);
    virtual void write(uint8_t);
    using Print::write; // pull in write(str) and write(buf, size) from Print
};

#define Serial Serial1
extern HardwareSerial Serial1;
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;

#ifdef __cplusplus
extern "C" {
#endif
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
#ifdef __cplusplus
};
#endif

#endif

