#include <ThingSpeak.h>
#include <blynk.h>



//declaration
int val = 0;//variable to store soil value
int soil = A2;//Declare a variable for the soil moisture sensor 
int soilPower = D6;//Variable for Soil moisture Power
int servoPin = D0;
int servo_position = 0;
// Lib instantiate
Servo myservo;


//DANGER - DO NOT SHARE!!!!
char auth[] = "350a04eef49446af816981dbefae2afb"; // Put your blynk token here
//DANGER - DO NOT SHARE!!!!

char VERSION[64] = "0.04";



TCPClient client;
unsigned int myChannelNumber = 602495; // replace with your ChannelID
const char * myWriteAPIKey = "WP6RQ1C7TZMEXDDW"; // replace with your WriteAPIKey


void setup()
{
Serial.begin(9600);   // open serial over USB
Blynk.begin(auth);
ThingSpeak.begin(client);


pinMode(soilPower, OUTPUT);//Set D6 as an OUTPUT
digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
 
}


int readSoil()
{
    digitalWrite(soilPower, HIGH);//turn D6 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soil);
    digitalWrite(soilPower, LOW);//turn D6 "Off"
    return val;
}



void loop()
{

  Blynk.run(); // all the Blynk magic happens here
 
    float moisture = readSoil();

   
    int servo_position = moisture / 26;
    myservo.attach(servoPin);
    myservo.write(servo_position);
    Serial.println(servo_position);
 
 
    ThingSpeak.setField(1,moisture);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    Blynk.virtualWrite(V1, moisture);
    delay(500);
}