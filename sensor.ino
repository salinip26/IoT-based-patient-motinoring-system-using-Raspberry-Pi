#define USE_ARDUINO_INTERRUPTS true 
#include <PulseSensorPlayground.h>     	 
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

DeviceAddress insideThermometer;

// Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.


PulseSensorPlayground pulseSensor;       


void setup() 
{

     Serial.begin(9600);          // For Serial Monitor

     // Configure the PulseSensor object, by assigning our variables to it.
     pulseSensor.analogInput(PulseWire); //pointing the A0 variable from the arduino
      pulseSensor.blinkOnPulse(LED13); //auto-magically blink Arduino's LED with heartbeat.
      pulseSensor.setThreshold(Threshold); //assignening the 550 to thr threshold

     // Double-check the "pulseSensor" object was created and "began" seeing a signal.
     if (pulseSensor.begin()) 
{
Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }

     sensors.begin(); //temperature sensor
     Serial.print(sensors.getDeviceCount(), DEC); //it checkshow many temperature sensor connect
     if (sensors.isParasitePowerMode()) Serial.println("ON"); //to check if its working ,library defined function if the temperature sensor is connected
     printAddress(insideThermometer); //function name
     sensors.setResolution(insideThermometer, 9); //internal function
}

void printTemperature(DeviceAddress deviceAddress) 
{
 	 // method 2 - faster
 	 float tempC = sensors.getTempC(deviceAddress);
  if (tempC == DEVICE_DISCONNECTED_C)
 	 {
   	 	Serial.println("Error: Could not read temperature data");
    		return;
 	 }
    Serial.print("Temp C: ");
    Serial.print(tempC);
    Serial.print(" Temp F: "); // temperature in celcius
    Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
}


void loop() 
{
int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
 	 // "myBPM" hold this BPM value now.

  if (pulseSensor.sawStartOfBeat())
 {            // Constantly test to see if "a beat happened".
Serial.println(" A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
   		 Serial.print("BPM: ");                      
  		  Serial.println(myBPM);                       
  }

     delay(20);                    // Delaying time for20 ms 
     sensors.requestTemperatures(); //to re quest next temperature value
     printTemperature(insideThermometer);
}

//identify the temperature sensor
void printAddress(DeviceAddress deviceAddress)
{
  	for (uint8_t i = 0; i < 8; i++)
  	{
    		if (deviceAddress[i] < 16) Serial.print("0");
  		 Serial.print(deviceAddress[i], HEX);
 	 }
}
