#include <SoftwareSerial.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//setup the pins for the bluetooth
SoftwareSerial MyBlue(10, 11); // RX | TX

//reads inccoming data
int flag = 0;

//for sending data
String out = "";
//setup led pin
int LED = 8;

//setup pins for the ultrasonic sensor
const int trigPin = 3;
const int echoPin = 2;

//expiremental sound stuff
int buzzerPin = 12;

//used to calculate distance via how long it takes for sound to leave and return to the sensor
long duration;
int distance;


//just a bool to check the state of elements
int ledOn = 0;
int soundOn = 0;

//Motion sensor stuff
#define BNO055_SAMPLERATE_DELAY_MS (88)

//Check I2C device address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
sensors_event_t event;

//Motion sensor orientation
float orientation_x;
float orientation_y;
float orientation_z;

//messy stuff
unsigned long time1;
unsigned long time2;



unsigned long StartTime1;
unsigned long CurrentTime1;
unsigned long ElapsedTime1;


unsigned long StartTime2;
unsigned long CurrentTime2;
unsigned long ElapsedTime2;

void ultrasonic_stuff()
{
  //clear trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  //set trigPin on HIGH for 10 ms
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //read echoPin
  duration = pulseIn(echoPin, HIGH);

  //calculate distance
  distance = duration * 0.034 / 2;

  time2 = millis();

}

void motion_sensor_stuff()
{
  /* Get a new sensor event */
  bno.getEvent(&event);

  orientation_x = (float)event.orientation.x;
  orientation_y = (float)event.orientation.y;
  orientation_z = (float)event.orientation.z;
  
  /*Serial.print(360 - (float)event.orientation.x);
  Serial.print(F(", "));
  Serial.print((float)event.orientation.y);
  Serial.print(F(", "));
  Serial.print((float)event.orientation.z);
  Serial.println(F(""));  */

  delay(BNO055_SAMPLERATE_DELAY_MS);
}

void bluetooth_stuff()
{
  //write to bluetooth and serial output the data collected from the ultrasonic and motion sensors
  if (time2 - time1 >= 1000)
  {
    out+= distance;
    out += F(", ");
    
    out += (float)orientation_x;
    out += F(", ");
    out += (float)orientation_y;
    out += F(", ");
    out += (float)orientation_z;
    out += F("");

    
    MyBlue.println(out);
    //Serial.println(out);
    Serial.println((float)distance);
    Serial.println((float)orientation_x);
    Serial.println((float)orientation_y);
    Serial.println((float)orientation_z);
    time1 = millis();
  }
  out = "";

  //if the bluetooth module has incoming data, read that data
  if (MyBlue.available())
  {
    flag = MyBlue.read();
    Serial.println(flag);
  }
  
  //turn light on or off depending on what was read
  if (flag == '1')
  {
    if (ledOn == 0)
    {
      ledOn = 1;
      digitalWrite(LED, HIGH);
    }
    else
    {
      ledOn = 0;
      digitalWrite(LED, LOW);
    }
  }
  
  flag = 0;

}

void setup()
{
  //bluetooth setup, sets the baud rate and sets the pin's mode to output
  Serial.begin(9600);
  MyBlue.begin(9600);
  pinMode(LED, OUTPUT);
  Serial.println("Ready to connect\nDefualt password is 1234 or 000");

  //ultrasonic setup, trigpin outputs, trigpin receives
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialise the sensor
  if (!bno.begin())
  {
    // There was a problem detecting the BNO055
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  delay(1000);
  
  // Use external crystal for better accuracy
  bno.setExtCrystalUse(true);
   
  time1 = millis();
  time2;

  //Sound Setup
  pinMode(buzzerPin, OUTPUT);

  StartTime1 = millis();
  StartTime2 = millis();
}

void loop()
{

  ultrasonic_stuff();
  motion_sensor_stuff();
  bluetooth_stuff();
  //sound stuff
  if (flag == '2')
  {
    if (soundOn == 0)
    {
      soundOn = 1;
      digitalWrite(buzzerPin, HIGH);
    }
    else
    {
      soundOn = 0;
      digitalWrite(buzzerPin, LOW);
    }
  }
}
