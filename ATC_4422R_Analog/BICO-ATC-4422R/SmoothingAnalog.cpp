#include <Arduino.h>
#include "SmoothingAnalog.h"

SmoothingAnalog::SmoothingAnalog(uint8_t analogPin, uint8_t numOfArray)
{
	_analogPin = analogPin;
	_numOfAnalog = numOfArray;
	_analogIndex = 0;
}

int16_t SmoothingAnalog::getAnalog()
{
	_analogIndex = (_analogIndex < _numOfAnalog)? _analogIndex:0;
	_analogArray[_analogIndex++] = analogRead(_analogPin);
	long totalAnalog = 0;
	for(uint8_t i = 0; i <_numOfAnalog; i++)
	{
		totalAnalog += _analogArray[i];
	}

	return totalAnalog/_numOfAnalog;
}