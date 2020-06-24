#include "Arduino.h"
#include "MultimeterClasses.h"

using namespace Multimeter;

Result::Result(bool success, float value = 0)
{
	Success = success;
	Value = value;
}
#pragma region Defaults
Result Measurement::OnMeasure()
{
	return Result(false);
}

void Measurement::OnStart()
{
	return;
}

void Measurement::Reset()
{
  lastValue = 0;
}

#pragma endregion Defaults

Result Measurement::GetResult(float value)
{
	if(isnan(value) || lastValue == value)
	{
		return Result(false);
	}
	lastValue = value;
	return Result(true, value);
}

Result Resistance::OnMeasure()
{
	raw = analogRead(ResistancePin);
	if(raw)
	{
		buffer = raw * Vin;
		Vout = (buffer)/1024.0;
		buffer = (Vin/Vout) -1;
		return Measurement::GetResult(R1 * buffer);
	}
	return Result(false);
}
