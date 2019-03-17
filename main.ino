// Utilizes public domain libraries from Grove and SEEED websites for DHT Temperature Humidity Sensor,
//Grove Light Sensor and Servo Motors


#include "DHT.h"
#include <math.h>
#include <Servo.h>

//setting up the pins for the Servo, Light and Temperature Sensors
const int lightSensor=A0; //Light Analog Pin #
int servoPin = 9;       //Sensor Digital Pin #
Servo servo;          //Servo Motor object
int triggerFallAngle = 150;   //Fall Angle, to be turned when the curtain is to be dropped
int defaultAngle = -50;       //Default angle of the Servo, to be returned when the program initializes
boolean hotState = false;     //Boolean describing whether the outside temperature is hot or not
int hotCounter = 0;           //Counter to counterbalance the natural errors of the sensor, so a sharp spike won't solely trigger the curtain fall


#define DHTPIN 2     // temp sensor pin
#define DHTTYPE DHT11   // DHT 11 temperature sensor type 
DHT dht(DHTPIN, DHTTYPE);   //Setting up sensor type on pin with DHT library
float temperatureThreshold = 26.0f;     //Temperature the sensor has to get before triggering fall event. Higher than estimated actual to account for extremely hot comp lab conditions
void setup() 
{
    Serial.begin(9600);       //Initialize baud rate on sensor reads
    servo.attach(servoPin);   //Set up initial servo object
    servo.write(defaultAngle);  //Default out motor on restart
    dht.begin();                //initialize temp/humidity sensor
}


void loop() 
{
    float t = dht.readTemperature();                //Store temperature reading
    //int lightValue=analogRead(lightSensor);       //For future versions that might require light pin input
    //Serial.println("light sensor data:");
    //Serial.println(lightValue);
    Serial.println(t);                              //Debug reasons. Print out whats what
  
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t)) 
    {
        Serial.println("Failed to read from DHT");      //Do nothing if the temperature is invalid
    } 
    else 
    {
        if(t>=temperatureThreshold){
            hotCounter++;
            if(!hotState && hotCounter>=5){      //Make this hasn't been done before, and that the temperature reading has consistent history of high temperature
              servo.write(triggerFallAngle);      //Drop the curtain
              hotState=true;                    //set flag so we only move the motor once
            }     
      }else{
          if(hotState){         //If curtain still has not fallen
          hotCounter--;         //Decrement counter
          if(hotCounter<=0){    //To a minimum of 0 if the temperature is too low
              hotCounter = 0;   
            }   
          }
      }
    }
    
    delay(1000);        //Only perform these scans once every second. No need to go crazy or anything
}
