#ifndef MULTIMETER_CLASSES
#define MULTIMETER_CLASSES

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
	
	class Resistance : public Measurement
	{
		public:
			String Name() override {return String("Resistance");}
			String Unit() override {return String("Ω");}
			inline Result OnMeasure() override;
		private:
			int ResistancePin = 4;
			int raw = 0;
			int Vin = 5;
			float Vout = 0;
			float R1 = 10000;
			float buffer = 0;
	};
}

#endif
