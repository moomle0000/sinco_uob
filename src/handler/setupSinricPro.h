#include "SinricPro.h"
#include "SinricProSwitch.h"
#include "SinricProTemperaturesensor.h"
#include "handler/Temperaturerang.h"
#define APP_KEY "0c94c439-5c73-45d2-bcdd-0cb3f9650e9d"
#define APP_SECRET "c7c2b17f-b3a8-4364-89a2-fd88dbe5c871-b58304a1-eab8-4d9a-b365-0d07d8bd1e1d"
#define TEMP_SENSOR_ID "656259c2a3c6b579a1afe4cc"

#define windowmode "65625fc2031135be133ebc28"
#define windowmoderamg "65635fbf031135be133f393b"
#define BAUD_RATE 115200      // Change baudrate to your need (used for serial monitor)
#define EVENT_WAIT_TIME 60000 // send event every 60 seconds

#define DHT_SENSOR_PIN 23     // ESP32 pin GPIO23 connected to DHT11
#define DHT_SENSOR_TYPE DHT11 // DHT11 sensor type

bool isSwitchOn = false;
// Create an instance of the DHT sensor
Temperaturerang &temperaturerang = SinricPro[windowmoderamg];
DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

#define TEMP_SENSOR_ID "656259c2a3c6b579a1afe4cc"
#define BAUD_RATE 115200      // Change baudrate to your need (used for serial monitor)
#define EVENT_WAIT_TIME 20000 // send event every 60 seconds

#define DHT_PIN 23

bool deviceIsOn;                              // Temeprature sensor on/off state
float temperature;                            // actual temperature
float humidity;                               // actual humidity
float lastTemperature;                        // last known temperature (for compare)
float lastHumidity;                           // last known humidity (for compare)
unsigned long lastEvent = (-EVENT_WAIT_TIME); // last time event has been sent                          // Temeprature sensor on/off state

int temperatureset;

int controaltime = 0;

bool globalPowerState;
std::map<String, int> globalRangeValues;
bool PowerState(String deviceId, bool &state)
{
  if (deviceId == windowmode)
  {
    Serial.println('Window mode turned ' + state ? "on" : "off");
    if (isSwitchOn)
    {
      Serial.printf("Switch OFF\n");
      isSwitchOn = false;
      // Your code to perform an action when the switch is turned OFF
      controaltime = 0;
    }
    else
    {
      Serial.printf("Switch ON\n");
      isSwitchOn = true;
      controaltime = 0;

      // Your code to perform an action when the switch is turned ON
    }

    // temperature
  }
  if (deviceId == TEMP_SENSOR_ID)
  {
    Serial.printf("Temperaturesensor turned %s (via SinricPro) \r\n", state ? "on" : "off");
    deviceIsOn = state; // turn on / off temperature sensor
  }

  if (deviceId == windowmoderamg)
  {
    Serial.printf("[Device: %s]: Powerstate changed to %s\r\n", deviceId.c_str(), state ? "on" : "off");
    globalPowerState = state;
    isSwitchOn = state;
    return true; // request handled properly
  }
  return true;
}

// PowerStateController
void updatePowerState(bool state)
{
  temperaturerang.sendPowerStateEvent(state);
}

// RangeController
void updateRangeValue(String instance, int value)
{
  temperaturerang.sendRangeValueEvent(instance, value);
}
// RangeController
bool onRangeValue(const String &deviceId, const String &instance, int &rangeValue)
{
  Serial.printf("[Device: %s]: Value for \"%s\" changed to %d\r\n", deviceId.c_str(), instance.c_str(), rangeValue);
  globalRangeValues[instance] = rangeValue;
  temperatureset = rangeValue;
  return true;
}

bool onAdjustRangeValue(const String &deviceId, const String &instance, int &valueDelta)
{
  globalRangeValues[instance] += valueDelta;
  Serial.printf("[Device: %s]: Value for \"%s\" changed about %d to %d\r\n", deviceId.c_str(), instance.c_str(), valueDelta, globalRangeValues[instance]);
  globalRangeValues[instance] = valueDelta;
  return true;
}

void setupSinricPro()
{

  // PowerStateController
  temperaturerang.onPowerState(PowerState);

  // RangeController
  temperaturerang.onRangeValue("temperaturerange", onRangeValue);
  temperaturerang.onAdjustRangeValue("temperaturerange", onAdjustRangeValue);
  // add device to SinricPro
  SinricProTemperaturesensor &mySensor = SinricPro[TEMP_SENSOR_ID];
  mySensor.onPowerState(PowerState);

  SinricProSwitch &mySwitch = SinricPro[windowmode];
  mySwitch.onPowerState(PowerState);

  // setup SinricPro
  SinricPro.onConnected([]()
                        { Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([]()
                           { Serial.printf("Disconnected from SinricPro\r\n"); });

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void handleTemperaturesensor()
{
  if (deviceIsOn == false)
    return; // device is off...do nothing

  unsigned long actualMillis = millis();
  if (actualMillis - lastEvent < EVENT_WAIT_TIME)
    return; // only check every EVENT_WAIT_TIME milliseconds

  temperature = dht.readTemperature(); // get actual temperature in °C
                                       //  temperature = dht.getTemperature() * 1.8f + 32;  // get actual temperature in °F
  humidity = dht.readHumidity();       // get actual humidity

  if (isnan(temperature) || isnan(humidity))
  {                                           // reading failed...
    Serial.printf("DHT reading failed!\r\n"); // print error message
    return;                                   // try again next time
  }

  Serial.println("Temperature: " + String(temperature) + "°C Humidity: " + String(humidity) + "%");

  if (temperature == lastTemperature || humidity == lastHumidity)
    return; // if no values changed do nothing...

  SinricProTemperaturesensor &mySensor = SinricPro[TEMP_SENSOR_ID];    // get temperaturesensor device
  bool success = mySensor.sendTemperatureEvent(temperature, humidity); // send event
  if (success)
  { // if event was sent successfuly, print temperature and humidity to serial
    Serial.printf("Temperature: %2.1f Celsius\tHumidity: %2.1f%%\r\n", temperature, humidity);
  }
  else
  { // if sending event failed, print error message
    Serial.printf("Something went wrong...could not send Event to server!\r\n");
  }

  lastTemperature = temperature; // save actual temperature for next compare
  lastHumidity = humidity;       // save actual humidity for next compare
  lastEvent = actualMillis;      // save actual time for next compare
}
void openwindow(int time)
{
  // Serial.println("Window opened");
  controaltime = 1;
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(time);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void closewindow(int time)
{
  // Serial.println("Window closed");
  controaltime = 1;
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(time);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

}

bool windowmoded = false;
void controalWindow()
{
  if (isSwitchOn)
  {
    windowmoded = controaltime == 0 ? true : false;

    //// open
    int temp = (int)temperature;

    // unsigned long actualMillis = millis();
    // if (actualMillis - lastEvent < EVENT_WAIT_TIME)
    //   return;


      
      

    if (dht.readTemperature() < temperatureset)
    {
      closewindow(12000);
    }
    if (dht.readTemperature() > temperatureset)
    {
      openwindow(12000);
    }
  }
  else
  {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
}
