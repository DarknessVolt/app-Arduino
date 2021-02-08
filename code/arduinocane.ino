#include <SoftwareSerial.h> 


//setup the pins for the bluetooth
SoftwareSerial MyBlue(10, 11); // RX | TX 

//reads inccoming data
int flag = 0; 

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


//messy stuff
unsigned long time1;
unsigned long time2;



unsigned long StartTime1;
unsigned long CurrentTime1;
unsigned long ElapsedTime1;


unsigned long StartTime2;
unsigned long CurrentTime2;
unsigned long ElapsedTime2;


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

 time1 = millis();
 time2;

 //Sound Setup
 pinMode(buzzerPin, OUTPUT);

  StartTime1 = millis();
  StartTime2 = millis();
} 

void loop() 
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
 distance = duration*0.034/2;

 time2 = millis();

 if(time2 - time1 >= 1000)
 {
  MyBlue.println(distance);
  Serial.println(distance);
  time1=millis();
 }
 
 //bluetooth stuff
 //if the bluetooth module has incoming data, read that data
 if (MyBlue.available()) 
 {
   flag = MyBlue.read();
   Serial.println(flag); 
 }
 //turn light on or off depending on what was read
 if (flag == '1') 
 {
  if(ledOn == 0)
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

 //sound stuff
 if(flag == '2')
 {
  if(soundOn == 0)
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