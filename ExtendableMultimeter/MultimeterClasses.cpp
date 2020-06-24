#include "Arduino.h"
#include "MultimeterClasses.h"

using namespace Multimeter;

#pragma region Final
Result::Result(bool success, float value = 0)
{
	Success = success;
	Value = value;
}
#pragma endregion Final

#pragma region BaseClass
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
#pragma endregion BaseClass

#pragma region ResistanceForks
#pragma region Resistance
Result Resistance::OnMeasure()
{
	int raw = analogRead(ResistancePin);
	if(raw)
	{
		float buffer = raw * Vin;
		float Vout = (buffer)/1024.0;
		buffer = (Vin/Vout) -1;
		return Measurement::GetResult(R1 * buffer);
	}
	return Result(false);
}
#pragma endregion Resistance

#pragma region Current
Result Current::OnMeasure()
{
	int raw = analogRead(ResistancePin);
	if(raw)
	{
		float buffer = raw * Vin;
		float Vout = (buffer)/1024.0;
		buffer = (Vin/Vout) -1;
		float R2 = R1*buffer;
		return Measurement::GetResult(Vout/R2);
	}
	return Result(false);
}
#pragma endregion Current

#pragma region DiodeVoltage
Result DiodeVoltage::OnMeasure()
{
	int raw = analogRead(ResistancePin);
	if(raw)
	{
		float buffer = raw * Vin;
		return Measurement::GetResult((buffer)/1024.0);
	}
	return Result(false);
}
#pragma endregion DiodeVoltage

#pragma region Conductance
Result Conductance::OnMeasure()
{
	int raw = analogRead(ResistancePin);
	if(raw)
	{
		float buffer = raw * Vin;
		float Vout = (buffer)/1024.0;
		buffer = (Vin/Vout) -1;
		return Measurement::GetResult(1/(R1 * buffer));
	}
	return Result(false);
}
#pragma endregion Conductance

#pragma region hFe
Result hFe::OnMeasure()
{
	int raw = analogRead(ResistancePin);
	float Ib = 0;
	if(raw)
	{
		float buffer = raw * Vin;
		float Vout = (buffer)/1024.0;
		buffer = (Vin/Vout) -1;
		Ib = Vout/(R1*buffer);
	}
	else
	{
		return Result(false);
	}
	raw = analogRead(CollectorPin);
	if(raw)
	{
		float buffer = raw * Vin;
		float Vout = (buffer)/1024.0;
		buffer = (Vin/Vout) -1;
		return Result((Vout/(R1*buffer))/Ib);
	}
	return Result(false);
}
#pragma endregion hFe
#pragma endregion ResistanceForks

#pragma region Voltage
Result Voltage::OnMeasure()
{
	return Measurement::GetResult(analogRead(VoltagePin)*vpp);
}
#pragma endregion Voltage

#pragma region Capacitance
void Capacitance::OnStart()
{
	pinMode(ChargePin, OUTPUT);
	digitalWrite(ChargePin, LOW);
}

Result Capacitance::OnMeasure()
{
	digitalWrite(ChargePin, HIGH);
	unsigned long StartTime = millis();
	while(analogRead(CapacitancePin) < 648 ){}
	unsigned long ElapsedTime = millis()-StartTime;
	MicroFarads = ((float)ElapsedTime / ResistorValue) * 1000;
	Result toReturn = Measurement::GetResult(MicroFarads > 1 ? (long)MicroFarads : (long)(MicroFarads*1000.0));
	digitalWrite(ChargePin, LOW);
	pinMode(DischargePin, OUTPUT);
	digitalWrite(DischargePin, LOW);
	while(analogRead(CapacitancePin) > 0){}
	pinMode(DischargePin, INPUT);
	return toReturn;
}

String Capacitance::Unit()
{
	return MicroFarads > 1 ? String(" microfarads") : String(" nanofarads");
}
#pragma endregion Capacitance
