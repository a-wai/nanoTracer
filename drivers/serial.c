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

#include "serial.h"

static	char		txBuffer[512];
static	int16_t	txPos = -1;
static	int16_t	txLen = -1;

// Init the serial interface
void serialInit(uint32_t br)
{
	uint16_t ubrr;

  // Calculate baud rate (using /16 prescaler for the 16MHz Arduino clock)
	ubrr = F_CPU / (16 * br) - 1;
	UBRR0H = (uint8_t) (ubrr >> 8);
	UBRR0L = (uint8_t) ubrr;

	// Disable Double Speed & Multi Processor Communication
	UCSR0A &= ~(_BV(U2X0) | _BV(MPCM0));
	// Disable USART0 interrupts & clear UCSZ02 (8-bit transmission)
	UCSR0B &= ~(_BV(TXCIE0) | _BV(UDRIE0) | _BV(UCSZ02));
	// Enable USART0 & RX complete interrupt
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
	// Set asynchronous link, no parity bit, 1 stop bit
	UCSR0C &= ~(_BV(UMSEL01) | _BV(UMSEL00) | _BV(UPM01) | _BV(UPM00) | _BV(USBS0));
	// Set 8 data bits
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}

// Wait for character reception and retrieve its value
char	serialReceive(void)
{
  return UDR0;
}

// Send a character
void serialWrite()
{
	if (txPos >= 0)
	{
		UDR0 = txBuffer[txPos];
		txPos++;
		if (txPos >= txLen)
		{
			txPos = -1;
		}
	}
	else
	{
		// Disable TXC interrupt
		UCSR0B &= ~_BV(TXCIE0);
	}
}

// Send a string over the serial line
void serialSend(char *str)
{
  txPos = 0;
  txLen = strlen(str);
  strcpy(txBuffer, str);
  
  serialWrite();
	UCSR0B |= _BV(TXCIE0);
}

void serialDebug(char *str)
{
	int i = 0;

	UCSR0B &= ~_BV(TXCIE0);

	while (str[i])
	{
		while (!(UCSR0A & _BV(UDRE0)));

		UDR0 = str[i];
		i++;
	}
}
