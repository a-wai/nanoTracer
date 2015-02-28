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

#include "drivers/adc.h"

#include "tracer.h"

#define   MAX_GRID_VOLTAGE  (10 * MAX_NEGATIVE_VOLTAGE)

uint16_t values[64];

static uint8_t startAcquisition = 0;
static uint16_t oldValue = 0;
static uint8_t measureCount = 0;
static uint8_t currentStep = 0;
static uint8_t maxSteps = 0;

static uint8_t gridVoltages[MAX_STEPS];

void tracerStartAcquisition(void)
{
  if (maxSteps > 0)
  {
    adcSelectSource(PIN_VOLTAGE);
    startAcquisition = 1;
    oldValue = 0xFFFF;
    measureCount = 0;
    currentStep = 0;
    tracerSetGridVoltage(gridVoltages[currentStep]);
    adcStart();
  }
  else
  {
    serialSend("Tube type not defined, measurement aborted...\r\n");
  }
}

void tracerSelectTube(char tube)
{
  uint8_t maxVoltage, voltageStep, voltage, i = 0;
  char family = 0;
  char msg[100];

  switch (tube)
  {
    case '1': // ECC81
      voltageStep = 10;
      maxVoltage = 60;
      break;
    case '2': // ECC82
      voltageStep = 20;
      maxVoltage = 200;
      break;
    case '3': // ECC83
      voltageStep = 5;
      maxVoltage = 40;
      break;
    case '8': // ECC88
      voltageStep = 10;
      maxVoltage = 60;
      break;
    case '9': // ECC99
      voltageStep = 20;
      maxVoltage = 200;
      family = tube;
      break;
    default:
      voltageStep = 0;
      maxVoltage = 0;
  }

  if (!family)
  {
    family = '8';
  }

  if (maxVoltage > MAX_GRID_VOLTAGE)
  {
    maxVoltage = MAX_GRID_VOLTAGE;
  }

  for (voltage = maxVoltage; voltage > 0; voltage -= voltageStep)
  {
    gridVoltages[i] = voltage;
    i++;
  }

  if (i)
  {
    gridVoltages[i] = 0;

    sprintf(msg, "Selected tube type ECC%c%c - \r\nMeasuring for %d grid voltages from -%dV to 0V\r\n", family, tube,
        i + 1, gridVoltages[0] / 10);
    serialSend(msg);
  }

  maxSteps = i;
}

void tracerSetGridVoltage(uint8_t voltage)
{
  uint16_t tmp = (uint16_t) voltage;

  tmp *= 255;
  tmp /= 100;
  OCR0A = (uint8_t) tmp;
}

program_mode_t tracerProcessAdc(void)
{
  static uint8_t acquiring;
  uint16_t adcValue;
  program_mode_t ret = MODE_NONE;

  if (startAcquisition)
  {
    acquiring = 0;
    startAcquisition = 0;
  }

  adcValue = adcRead();
  if (oldValue == 0xFFFF)
  {
    oldValue = adcValue;
  }

  if (!acquiring)
  {
    if ((oldValue < 100) && (adcValue > oldValue))
    {
      acquiring = 1;
    }
    oldValue = adcValue;
  }
  else
  {
    values[measureCount] = adcValue;
    measureCount++;

    if (measureCount % 2)
    {
      adcSelectSource(PIN_VOLTAGE);
      if (oldValue > adcValue)
      {
        values[measureCount - 1] = oldValue;
        acquiring = 0;
        tracerNextStep();
      }
      oldValue = adcValue;
    }
    else
    {
      adcSelectSource(PIN_CURRENT);
    }
  }

  return ret;
}

void tracerNextStep(void)
{
  tracerProcessResults();

  if (currentStep < maxSteps)
  {
    currentStep++;
    tracerSetGridVoltage(gridVoltages[currentStep]);
  }
  else
  {
    tracerSetGridVoltage(MAX_GRID_VOLTAGE);
    tracerStopAcquisition();
  }

  oldValue = 0xFFFF;
  measureCount = 0;
}

void tracerStopAcquisition(void)
{
  adcSelectSource(PIN_VOLTAGE);
  adcStop();
}

void tracerProcessResults(void)
{
  char print[512];
  char tmp[10];
  uint32_t uAnode, uGrille;
//	uint16_t	intGrille, decGrille;

  sprintf(print, "Vg = %d\r\n", gridVoltages[currentStep]);

  measureCount = (measureCount / 2) * 2;
  values[measureCount] = values[measureCount - 2];

  for (int i = 2; i < measureCount; i += 2)
  {
    uAnode = values[i] + values[i + 2];
    uAnode *= 505;
    uAnode /= 2048;

#if 1
    uGrille = values[i + 1];
    uGrille *= 505;
    uGrille /= 1024;

    sprintf(tmp, "%lu:%lu\r\n", uAnode, uGrille);
#else
    uGrille = values[i + 1];
    uGrille *= 735;
    uGrille = 512000 - uGrille;

    intGrille = uGrille / 48128;
    decGrille = (uGrille % 48128) * 10 / 48128;

    sprintf(tmp, "%lu:-%u.%u\r\n", uAnode, intGrille, decGrille);
#endif
    strcat(print, tmp);
  }

  serialDebug(print);
}
