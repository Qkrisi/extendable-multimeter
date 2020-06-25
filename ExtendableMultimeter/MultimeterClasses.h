#ifndef MULTIMETER_CLASSES
#define MULTIMETER_CLASSES

#include <DHT.h>

namespace Multimeter
{
	class Result
	{
		public:
			bool Success;
			float Value;
			inline Result(bool success, float value);
	};
	
	class Measurement
	{
		public:
			virtual String Name() {return String("Nothing");}
			virtual String Unit() {return String("");}
			inline virtual Result OnMeasure();
			inline virtual void OnStart();
			inline virtual void Reset();
		protected:
			inline Result GetResult(float Value);
		private:
			float lastValue;
	};
	
	class ResistanceFork : public Measurement
	{
		protected:
			const int ResistancePin = 4;
			const int Vin = 5;
			const float R1 = 10000;
	};
	
	class Resistance : public ResistanceFork
	{
		public:
			String Name() override {return String("Resistance");}
			String Unit() override {return String("Ω");}
			inline Result OnMeasure() override;
	};
	
	class Current : public ResistanceFork
	{
		public:
			String Name() override {return String("Current");}
			String Unit() override {return String("A");}
			inline Result OnMeasure() override;
	};
	
	class DiodeVoltage : public ResistanceFork
	{
		public:
			String Name() override {return String("Diode Voltage");}
			String Unit() override {return String("V");}
			inline Result OnMeasure() override;
	};
	
	class Conductance : public ResistanceFork
	{
		public:
			String Name() override {return String("Conductance");}
			String Unit() override {return String("S");}
			inline Result OnMeasure() override;
	};
	
	class hFe : public ResistanceFork
	{
		public:
			String Name() override {return String("Tranzistor gain");}
			inline Result OnMeasure() override;
		private:
			int CollectorPin = 3;
	};
	
	class Voltage : public Measurement
	{
		public:
			String Name() override {return String("Voltage");}
			String Unit() override {return String("V");}
			inline Result OnMeasure() override;
		private:
			int VoltagePin = 1;
			const float vpp = 0.01173020527859237537;
	};
	
	class Capacitance : public Measurement
	{
		public:
			String Name() override {return String("Capacitance");}
			inline String Unit() override;
			inline Result OnMeasure() override;
			inline void OnStart() override;
		private:
			int CapacitancePin = 2;
			int ChargePin = 9;
			int DischargePin = 8;
			float ResistorValue = 10000.0F;
			float MicroFarads;
	};
	
	class Temperature : public Measurement
	{
		public:
			String Name() override {return String("Temperature");}
			String Unit() override {return String("°C");}
			inline Result OnMeasure() override;
			inline void OnStart() override;
		private:
			DHT dht = DHT(10, 11);			//DHT(TemperaturePin, DHTType)
	};
}

#endif
