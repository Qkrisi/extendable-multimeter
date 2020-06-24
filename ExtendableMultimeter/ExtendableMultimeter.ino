#include "MultimeterClasses.cpp"

// Rotary Encoder Inputs
#define CLK 2
#define DT 3
#define SW 4
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

Measurement* measurements[] = {new Resistance(), new Current(), new DiodeVoltage(), new Conductance(), new hFe(), new Voltage(), new Capacitance()};
int measureIndex = 0;

void setup(){
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);

  Serial.begin(9600);

  for(auto m : measurements)
  {
    m->OnStart();
  }
}

void loop(){
  HandleRotation();
  HandleMeasurement();
  delay(1);
}

void HandleRotation(){
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      currentDir ="CCW";
    } else {
      counter ++;
      currentDir ="CW";
    }
    measureIndex = (counter % sizeof(measurements)) -1;
    auto currentMeasure = measurements[measureIndex];
    Serial.print("Now measuring: ");
    Serial.println(currentMeasure->Name());
    currentMeasure->Reset();
  }
  lastStateCLK = currentStateCLK;
}

void HandleMeasurement()
{
  Measurement* currentMeasure = measurements[measureIndex];
  auto result = currentMeasure->OnMeasure();
  if(result.Success)
  {
    Serial.print(currentMeasure->Name());
    Serial.print(": ");
    Serial.print(result.Value, 5);
    Serial.println(currentMeasure->Unit());
    return;
  }
}
