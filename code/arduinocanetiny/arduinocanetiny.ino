#include <SoftwareSerial.h>
//#include <ATtinySerialOut.h>
#include <TinyWireM.h>
#include <HCSR04.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BNO055.h>
//#include <utility/imumaths.h>

#include <TimedAction.h> //protothreading library

//setup the pins for the bluetooth  
#define RxD 1
#define TxD 0
SoftwareSerial MyBlue(RxD, TxD); // RX | TX

//reads inccoming data
int flag = 0;

//for sending data
//String out;
//setup led pin
const int LED = 10;

//setup pins for the ultrasonic sensor
const int trigPin = 7;
const int echoPin = 8;
UltraSonicDistanceSensor distanceSensor(trigPin, echoPin);
//used to calculate distance via how long it takes for sound to leave and return to the sensor
long duration = 0;
int distance = 0;


//Motion sensor stuff
#define BNO055_SAMPLERATE_DELAY_MS (88)

//Check I2C device address
//Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
//sensors_event_t event;

//
//imu::Vector<3> compass;

//float orientation_x;
//float orientation_y;
//float orientation_z;

void setupBlueToothConnection()
{
  MyBlue.begin(9600); //Set BluetoothBee BaudRate to default baud rate 38400
  MyBlue.print("\r\n+STWMOD=0\r\n"); //set the bluetooth work in slave mode
  MyBlue.print("\r\n+STNA=HC-05\r\n"); //set the bluetooth name as "HC-05"
  MyBlue.print("\r\n+STOAUT=1\r\n"); // Permit Paired device to connect me
  MyBlue.print("\r\n+STAUTO=1\r\n"); // Auto-connection should be forbidden here
  
  delay(2000); // This delay is required.
  //MyBlue.print("\r\n+INQ=1\r\n"); //make the slave bluetooth inquirable 
  MyBlue.print("bluetooth connected!\n");
   
  delay(2000); // This delay is required.
  MyBlue.flush();
}

void ultrasonic_stuff()
{
  digitalWrite(LED, HIGH);
  //clear trigPin
  digitalWrite(trigPin, LOW);
  delay(2);

  //set trigPin on HIGH for 10 ms
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  //read echoPin
  duration = pulseIn(echoPin, HIGH);

  //calculate distance
  distance = duration * 0.034 / 2;
  //delay(100);
  digitalWrite(LED, LOW);

}

void motion_sensor_stuff()
{
  
  /* Get a new sensor event */
//  bno.getEvent(&event);
//  compass = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
//  
//    orientation_x = (float)event.orientation.x;
//    orientation_y = (float)event.orientation.y;
//    orientation_z = (float)event.orientation.z;
//  

  /*Serial.print(360 - (float)event.orientation.x);
    Serial.print(F(", "));
    Serial.print((float)event.orientation.y);
    Serial.print(F(", "));
    Serial.print((float)event.orientation.z);
    Serial.println(F(""));  */

}

void bluetooth_stuff()
{
  //digitalWrite(LED, HIGH);
  //out = F("");
  //write to bluetooth and serial output the data collected from the ultrasonic and motion sensors
//  out = distance+"apple\n";
  //out += F(", ");
  
//    out += (float)orientation_x;
//    out += F(", ");
//    out += (float)orientation_y;
//    out += F(", ");
//    out += (float)orientation_z;
//    out += F("");

  distance = distanceSensor.measureDistanceCm();
  MyBlue.print(distance);
  //Serial.println(out);
  //  Serial.println((float)distance);
  //  Serial.println((float)orientation_x);
  //  Serial.println((float)orientation_y);
  //  Serial.println((float)orientation_z);
  //out = "";
  //delay(10);
  //digitalWrite(LED, LOW);
}

TimedAction ultrasonic_thread = TimedAction(50, ultrasonic_stuff);
TimedAction motion_sensor_thread = TimedAction(BNO055_SAMPLERATE_DELAY_MS, motion_sensor_stuff);
TimedAction bluetooth_thread = TimedAction(50, bluetooth_stuff);

void setup()
{
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection();
  //bluetooth setup, sets the baud rate and sets the pin's mode to output
  //Serial.begin(9600);
  //MyBlue.begin(300);
  //pinMode(LED, OUTPUT);
  //Serial.println("Ready to connect\nDefualt password is 1234 or 000");

  //out.reserve(30);

  //ultrasonic setup, trigpin outputs, echopin receives
//  pinMode(trigPin, OUTPUT);
//  pinMode(echoPin, INPUT);
  
  // Initialise the sensor
  //  if (!bno.begin())
  //  {
  //    // There was a problem detecting the BNO055
  //    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
  //    while (1);
  //  }
  //delay(1000);

  // Use external crystal for better accuracy
  //  bno.setExtCrystalUse(true);


}

void loop()
{
  //MyBlue.print(distance);
  //ultrasonic_thread.check();
  //motion_sensor_thread.check();
  //bluetooth_thread.check();
  //ultrasonic_stuff();
  //bluetooth_stuff();
  
  double d = distanceSensor.measureDistanceCm();
  MyBlue.print(d, 0);
  MyBlue.print("\n");
  delay(200);
  
  //if the bluetooth module has incoming data, read that data
//  if (MyBlue.available())
//  {
//    flag = MyBlue.read();
//    Serial.println(flag);
//  }

  //turn light on or off depending on what was read
//  if (flag == '1')
//  {
//    if (ledOn == 0)
//    {
//      ledOn = 1;
//      digitalWrite(LED, HIGH);
//    }
//    else
//    {
//      ledOn = 0;
//      digitalWrite(LED, LOW);
//    }
//  }

  //flag = 0;

  //sound stuff
//  if (flag == '2')
//  {
//    if (soundOn == 0)
//    {
//      soundOn = 1;
//      digitalWrite(buzzerPin, HIGH);
//    }
//    else
//    {
//      soundOn = 0;
//      digitalWrite(buzzerPin, LOW);
//    }
//  }
}
