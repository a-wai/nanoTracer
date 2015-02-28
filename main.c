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

#include "main.h"
#include "tracer.h"

static program_mode_t	mode = MODE_NONE;

static void	mcuInit(void)
{
	// Enable sleep mode in idle mode
  SMCR = _BV(SE);

  // Disable ADC power reduction
  BITCLR(PRR, PRADC);
}

static void	ioInit(void)
{
  // Configure pin D2 as input
  BITCLR(BTN_PORT, BTN_PIN);
  BITCLR(BTN_DDR, BTN_PIN);

  // Enable external interrupt on falling edge for pin D2
  EICRA = _BV(ISC00) | _BV(ISC01);
  EIMSK = _BV(INT0);
}

static void	pwmInit(void)
{
  // Configure pin D6 as output
  BITSET(PORT_DIG1, PIN_D6);
  BITSET(DDR_DIG1, PIN_D6);

  // Configure TIMER0 for fast PWM
  TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(CS00);
  TIMSK0 = 0;
  OCR0A = 200;
}

int main (void)
{
	char 						cmd;
	program_mode_t	willDo = MODE_NONE;

	wdt_disable();

	cli();

	mcuInit();

	ioInit();
  pwmInit();
  serialInit(500000);
  adcInit(PIN_VOLTAGE);

  sei();

	serialDebug("Starting...\r\n");

  for(;;)
  {
  	if (willDo)
  	{
  		mode = willDo;
  		willDo = MODE_NONE;
  	}
  	else
  	{
  		asm volatile ("sleep\n\t");
  	}

    switch (mode)
    {
			case MODE_START_ACQUISITION:
				tracerStartAcquisition();
				break;
			case MODE_CONVERSION_DONE:
				tracerProcessAdc();
				break;
			case MODE_SERIAL_RECEIVED:
				cmd = serialReceive();
				switch(cmd)
				{
					case '#':
						willDo = MODE_START_ACQUISITION;
						break;
					default:
						tracerSelectTube(cmd);
						break;
				}
				break;
			case MODE_SERIAL_SENT:
				serialWrite();
				break;
			default:
				break;
		}

    mode = MODE_NONE;
  }

  return 0;
}

ISR(INT0_vect)
{
	mode = MODE_START_ACQUISITION;
}

ISR(ADC_vect)
{
	mode = MODE_CONVERSION_DONE;
}

ISR(USART_RX_vect)
{
	mode = MODE_SERIAL_RECEIVED;
}

ISR(USART_TX_vect)
{
	mode = MODE_SERIAL_SENT;
}
