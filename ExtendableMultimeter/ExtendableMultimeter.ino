#include "MultimeterClasses.cpp"
#include "TSGraphics.cpp"
#include <ArxContainer.h>
#include "Adafruit_GFX.h"     
#include "Adafruit_ILI9341.h" 
#include "URTouch.h"

#define TFT_DC 6              
#define TFT_CS 7            
#define TFT_RST 5
#define TFT_MISO 12         
#define TFT_MOSI 11           
#define TFT_CLK 13            

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#define t_SCK 0              
#define t_CS 1                
#define t_MOSI 2              
#define t_MISO 3             
#define t_IRQ 4             

URTouch ts(t_SCK, t_CS, t_MOSI, t_MISO, t_IRQ);

#define Base_X 120
#define Base_Y 200

using namespace Multimeter;
using namespace arx;

/*// Rotary Encoder Inputs
#define CLK 2
#define DT 3
#define SW 4
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;*/

ValuePair lastResult("","");

Measurement* measurements[] =
{
    new Resistance(),
    new Current(),
    /*new DiodeVoltage(),
    new Conductance(),
    new hFe(),
    new Voltage(),
    new Capacitance(),
    new Temperature()*/
};

vector<Button> Buttons {};

Measurement* currentMeasure = measurements[1];

void setup(){
  /*pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);*/
  tft.begin();                     
  tft.setRotation(3);           
 
  //ts.InitTouch();                   
  ts.setPrecision(PREC_EXTREME);
  tft.fillScreen(ILI9341_BLACK);
  
  int x = 0;
  int y = 25;
  int counter = 0;
  for(auto m : measurements)
  {
    m->OnStart();
    Buttons.push_back(m->getSymbol().Draw(x, getY(y), &tft, m));
    counter+=1;
    x += 25;
    if(counter==4)
    {
      counter = 0;
      x = 0;
      y = 0;
    }
  }
}

void loop(){
  HandleState();
  HandleMeasurement();
  delay(10);
}

void HandleState(){
  if(ts.dataAvailable())
  {
    ts.read();
    auto x = ts.getX();
    auto y = getY(ts.getY());
    for(auto b : Buttons)
    {
      if(b.Check(x, y))
      {
        currentMeasure = b.Handler;
        break;
      }
    }
  }
}

void HandleMeasurement()
{
  auto result = currentMeasure->OnMeasure();
  if(result.Success)
  {
    auto Unit = currentMeasure->Unit();
    tft.setCursor(Base_X, getY(Base_Y));
    tft.setTextColor(ILI9341_BLACK);
    tft.print(lastResult.Value);
    tft.print(" ");
    tft.print(lastResult.Unit);
    tft.setCursor(Base_X, getY(Base_Y));
    tft.setTextColor(ILI9341_WHITE);
    tft.print(String(result.Value));
    tft.print(" ");
    tft.print(Unit);
    lastResult = ValuePair(String(result.Value), Unit);
    return;
  }
}
