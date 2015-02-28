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

#include "adc.h"

static	uint8_t	adcContinue = 0;

void adcSelectSource(uint8_t pin)
{
  // Sets the MUX3..0 bits of ADMUX accordingly with the selected source
  // See ATMega328P datasheet for details
  switch(pin)
  {
    case PIN_A0:
      ADMUX &= ~(_BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0));
      break;
    case PIN_A1:
      ADMUX &= ~(_BV(MUX3) | _BV(MUX2) | _BV(MUX1));
      ADMUX |= _BV(MUX0);
      break;
    case PIN_A2:
      ADMUX &= ~(_BV(MUX3) | _BV(MUX2) | _BV(MUX0));
      ADMUX |= _BV(MUX1);
      break;
    case PIN_A3:
      ADMUX &= ~(_BV(MUX3) | _BV(MUX2));
      ADMUX |= _BV(MUX1) | _BV(MUX0);
      break;
    case PIN_A4:
      ADMUX &= ~(_BV(MUX3) | _BV(MUX1) | _BV(MUX0));
      ADMUX |= _BV(MUX2);
      break;
    case PIN_A5:
      ADMUX &= ~(_BV(MUX3) | _BV(MUX1));
      ADMUX |= _BV(MUX2) | _BV(MUX0);
      break;
    default:
      break;
  }
}

void adcInit(uint8_t adPin)
{
  // Disable digital registers for PORTC
  DIDR0 = 0x3F;
  // Set voltage reference to Vcc (REFS0)
  ADMUX = _BV(REFS0);
  // Select ADC source pin
  adcSelectSource(adPin);
  // Enable ADC - Prescaler setting /128 for best accuracy
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  // Set ADC to single conversion mode
  ADCSRB = 0;
  // Start 1st conversion
	BITSET(ADCSRA, ADSC);
}

void	adcStop(void)
{
	adcContinue = 0;
	BITCLR(ADCSRA, ADIE);
}

void	adcStart(void)
{
	// Clear ADC interrupt flag
	BITSET(ADCSRA, ADIF);

  // Start ADC conversion & enable ADC interrupt
  ADCSRA |= _BV(ADSC) | _BV(ADIE);

	adcContinue = 1;
}


uint16_t	adcRead(void)
{
  uint16_t value = 0;

	// Clear ADC interrupt flag
	BITSET(ADCSRA, ADIF);

	// Read value
  // Value is stored on 10 bits : 2 MSB in ADCH, 8 LSB in ADCL
  value = ADCL;
  value += (ADCH << 8);

  if (adcContinue)
  {
    // Start new ADC conversion
    ADCSRA |= _BV(ADSC);
  }

  return value;
}
