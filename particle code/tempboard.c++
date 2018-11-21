#include <ThingSpeak.h>
#include <PietteTech_DHT.h>
#include <blynk.h>

#define DHTTYPE  DHT22        
#define DHTPIN   4         	 
#define DHT_SAMPLE_INTERVAL   60000 


const int LIGHT_PIN = A0; 

const int LED_PIN = 7;
const int LED2_PIN = 6;
const int LED3_PIN = 5;
const int servoPin = 0;



const float VCC = 4.98; 
const float R_DIV = 4660.0;
const float DARK_THRESHOLD1 = 10000.0;
const float DARK_THRESHOLD2 = 20000.0;
const float DARK_THRESHOLD3 = 35000.0;
const int servo_position = 0;


Servo myservo;
void dht_wrapper();

PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);

unsigned int DHTnextSampleTime;	  
bool bDHTstarted;		 
int n;                    

char resultstr[64]; 

char auth[] = "350a04eef49446af816981dbefae2afb"; 

char VERSION[64] = "0.04";

#define READ_INTERVAL 60000

TCPClient client;
unsigned int myChannelNumber = 602495;
//zane
//unsigned int myChannelNumber = 622517;
//callum


const char * myWriteAPIKey = "WP6RQ1C7TZMEXDDW"; 
//zane
//const char * myWriteAPIKey = "3FSNPVUT3LC2FOI8"; 
//callum


void setup()
{
 Serial.begin(9600);
  Blynk.begin(auth);
 ThingSpeak.begin(client);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
    


 DHTnextSampleTime = 0; 
 Particle.variable("result", resultstr, STRING);

 Particle.publish("DHT22 - firmware version", VERSION, 60, PRIVATE);
 
}



void dht_wrapper() {
    DHT.isrCallback();
}

void loop()
{

  Blynk.run();
 

  if (millis() > DHTnextSampleTime) {
      
	if (!bDHTstarted) {	
	    DHT.acquire();
	    bDHTstarted = true;
	}

 if (!DHT.acquiring()) {

  float temp = (float)DHT.getCelsius();
  int temp1 = (temp - (int)temp) * 100;

  char tempInChar[32];
  sprintf(tempInChar,"%0d.%d", (int)temp, temp1);
  Particle.publish("The temperature from the dht22 is:", tempInChar, 60, PRIVATE);
  ThingSpeak.setField(2,temp);
  Blynk.virtualWrite(V2, tempInChar);
  
  int servo_position = temp * 4.2;
  myservo.attach(servoPin);
  myservo.write(servo_position);
  Serial.println(servo_position);


  float humid = (float)DHT.getHumidity();
  int humid1 = (humid - (int)humid) * 100;

  char humidInChar[32];
  sprintf(humidInChar,"%0d.%d", (int)humid, humid1);
  Particle.publish("The humidity from the dht22 is:", humidInChar, 60, PRIVATE);
  ThingSpeak.setField(3,humid);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  Blynk.virtualWrite(V3, humidInChar);

  n++; 
  bDHTstarted = false;
  DHTnextSampleTime = millis() + DHT_SAMPLE_INTERVAL;
 }
 
}

  int lightADC = analogRead(LIGHT_PIN);
  if (lightADC > 0)
  {
    float lightV = lightADC * VCC / 1023.0;
    float lightR = R_DIV * (VCC / lightV - 1.0);
   // Serial.println("Voltage: " + String(lightV) + " V");
  //  Serial.println("Resistance: " + String(lightR) + " ohms");

    if (lightR >= DARK_THRESHOLD1)
    {
      digitalWrite(LED_PIN, HIGH);
    }
    else{
      digitalWrite(LED_PIN, LOW);
    }

      
    if (lightR >= DARK_THRESHOLD2){
      digitalWrite(LED2_PIN, HIGH);
    }
    else{
      digitalWrite(LED2_PIN, LOW);
    }
     
            
    if (lightR >= DARK_THRESHOLD3){
      digitalWrite(LED3_PIN, HIGH);
    }
    else{
      digitalWrite(LED3_PIN, LOW);
    }
      


    Serial.println();
    delay(500);
  }
  
}


