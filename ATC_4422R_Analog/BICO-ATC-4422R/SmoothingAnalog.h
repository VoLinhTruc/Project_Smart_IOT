#ifndef SmoothingAnalog_h
#define SmoothingAnalog_h

#include <Arduino.h>

class SmoothingAnalog
	{
	public:
		SmoothingAnalog(uint8_t analogPin, uint8_t numOfArray);

		int16_t getAnalog();

	private:
		uint8_t _analogPin;
		uint8_t _numOfAnalog;
		uint8_t _analogIndex;
		int16_t _analogArray[50];
	};

#endif