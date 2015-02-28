/*
 * Copyright (C) 2015 Arnaud Ferraris.
 *
 * This file is part of nanoTracer.
 *
 * nanoTracer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * nanoTracer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with nanoTracer.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PINS_H
#define PINS_H

#include <avr/io.h>

// Register mapping for digital pins 0-7

#define PORT_DIG1   PORTD
#define DDR_DIG1    DDRD
#define PIN_DIG1    PIND
#define PIN_D0      PD0
#define PIN_D1      PD1
#define PIN_D2      PD2
#define PIN_D3      PD3
#define PIN_D4      PD4
#define PIN_D5      PD5
#define PIN_D6      PD6
#define PIN_D7      PD7

// Register mapping for digital pins 8-13

#define PORT_DIG2   PORTB
#define DDR_DIG2    DDRB
#define PIN_DIG2    PINB
#define PIN_D8      PB0
#define PIN_D9      PB1
#define PIN_D10     PB2
#define PIN_D11     PB3
#define PIN_D12     PB4
#define PIN_D13     PB5

// Register mapping for analog pins

#define PORT_ADC    PORTC
#define DDR_ADC     DDRC
#define PIN_ADC			PINC
#define PIN_A0      PC0
#define PIN_A1      PC1
#define PIN_A2      PC2
#define PIN_A3      PC3
#define PIN_A4      PC4
#define PIN_A5      PC5

#define BITSET(port,bit)  port |= _BV(bit)
#define BITCLR(port,bit)  port &= ~_BV(bit)

#endif
