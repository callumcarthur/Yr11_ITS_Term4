#include <ThingSpeak.h>
#include <PietteTech_DHT.h>
#include <blynk.h>

#define DHTTYPE  DHT22        
#define DHTPIN   4         	 
#define DHT_SAMPLE_INTERVAL   60000 

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
const char * myWriteAPIKey = "WP6RQ1C7TZMEXDDW"; 


void setup()
{
 Serial.begin(9600);
  Blynk.begin(auth);
 ThingSpeak.begin(client);

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
  ThingSpeak.setField(2,tempInChar);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  Blynk.virtualWrite(V2, tempInChar);

sprintf(resultstr, "{\"t\":%s}", tempInChar);

  float humid = (float)DHT.getHumidity();
  int humid1 = (humid - (int)humid) * 100;

  char humidInChar[32];
  sprintf(humidInChar,"%0d.%d", (int)humid, humid1);
  Particle.publish("The humidity from the dht22 is:", humidInChar, 60, PRIVATE);
  ThingSpeak.setField(3,humidInChar);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  Blynk.virtualWrite(V3, humidInChar);

  n++; 
  bDHTstarted = false;
  DHTnextSampleTime = millis() + DHT_SAMPLE_INTERVAL;
 }
 
}

}
