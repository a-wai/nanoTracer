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

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "pins.h"

#include "drivers/serial.h"
#include "drivers/adc.h"

#define MAX_NEGATIVE_VOLTAGE	6

#define BTN_PORT 							PORT_DIG1
#define BTN_DDR  							DDR_DIG1
#define BTN_PIN  							PIN_D2

#define	PIN_CURRENT						PIN_A0
#define	PIN_VOLTAGE						PIN_A1
#define	PIN_GRID							PIN_A2

typedef enum
{
	MODE_NONE								= 0,
	MODE_START_ACQUISITION 	= 1,
	MODE_CONVERSION_DONE		= 2,
	MODE_SERIAL_SENT				= 3,
	MODE_SERIAL_RECEIVED		= 4,
	MODE_PROCESS_RESULTS		= 5,
	MODE_PRINT_RESULTS			= 6
} program_mode_t;

#endif /* MAIN_H_ */
