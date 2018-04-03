
#include "HeepDeviceDefinitions.h"
#include "DHT.h"

#include <math.h>

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT Sensor
const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

unsigned long lastReadTime = 0;  
unsigned long currentMillis;
int sensorFrequency = 3000;

int ReadHumidity(){
  float currentHumidity = dht.readHumidity();
  int currentSetting = round(currentHumidity);
  SetControlValueByName("Humidity",currentSetting);
  return currentSetting;
}

int ReadTemperature(){
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float currentTemperature = dht.readTemperature(true);
  int currentSetting = round(currentTemperature);
  SetControlValueByName("Temperature",currentSetting);
  return currentSetting;
}

int SendTempTrigger(int currentTemperature, int temperatureTresh){
  bool check = currentTemperature > temperatureTresh;
  SetControlValueByName("Temp Trigger", check);
  return check;
}

int SendHumidityTrigger(int currentHumidity, int humidityTresh){
  bool check = currentHumidity > humidityTresh;
  SetControlValueByName("Humidity Trigger", check);
  return check;
}

int CheckTempThresh(){
  int currentSetting = GetControlValueByName("Temp Thresh");
  return currentSetting;
}

int CheckHumidityTresh(){
  int currentSetting = GetControlValueByName("Humidity Tresh");
  return currentSetting;
}

void setup()
{

  Serial.begin(115200);
  
  AddRangeControl("Humidity",HEEP_OUTPUT,100,0,0);
  AddRangeControl("Temperature",HEEP_OUTPUT,100,0,0);
  AddRangeControl("Temp Thresh",HEEP_INPUT,100,0,0);
  AddRangeControl("Humidity Tresh",HEEP_INPUT,100,0,0);
  AddOnOffControl("Temp Trigger",HEEP_OUTPUT,0);
  AddOnOffControl("Humidity Trigger",HEEP_OUTPUT,0);
  StartHeep("HumidityTemp", 5);

  dht.begin();


}

int checkSensorValues() 
{
  int currentHumidity = ReadHumidity();
  int currentTemperature = ReadTemperature();
  int tempThresh = CheckTempThresh();
  int humidityThresh = CheckHumidityTresh();
  
  SendTempTrigger(currentTemperature, tempThresh);
  SendHumidityTrigger(currentHumidity, humidityThresh);

  Serial.print("\n\nTemperature: "); Serial.println(currentTemperature);
  Serial.print("Temp Tresh: "); Serial.println(tempThresh);
  Serial.print("\nHumidity: "); Serial.println(currentHumidity);
  Serial.print("Hum Tresh: "); Serial.println(humidityThresh);

  lastReadTime = millis();
  
  return 1;
}

void loop()
{
  PerformHeepTasks();
  
  currentMillis = millis();
  if (currentMillis - lastReadTime > sensorFrequency) {
    checkSensorValues();
  }


}
