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

#ifndef __ADC_H
#define __ADC_H

#include <stdint.h>
#include <avr/io.h>

#include "pins.h"

void 			adcInit					(uint8_t);

void 			adcStart				(void);
void 			adcStop					(void);

void 			adcSelectSource	(uint8_t pin);
uint16_t	adcRead					(void);

#endif
