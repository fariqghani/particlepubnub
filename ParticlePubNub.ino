// This #include statement was automatically added by the Particle IDE.
#include "application.h"
#include "DHT.h"
// This #include statement was automatically added by the Particle IDE.
#include <PubNub.h>


#define DEVICE_NAME "ParticleOfis"
#define DHTPIN D3
#define DHTTYPE DHT22
#define LEDPIN D7
#define MIN_TEMPERATURE -30
#define MAX_TEMPERATURE 120
#define MIN_HUMIDITY 0
#define MAX_HUMIDITY 100
#define SEND_INTERVAL 10

char pubkey[] = "pub-c-d296a56e-3228-489a-b468-305f41131489";
char subkey[] = "sub-c-f7c6f0d6-a4ff-11e7-b003-222aeb593f38";
char channel[] = "ParticlePN";

DHT dht(DHTPIN, DHTTYPE);
int failed = 0;
int lastUpdate = 0;


void setup()
{
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, HIGH);
    
    dht.begin();
    PubNub.begin(pubkey, subkey);
}

void loop()
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    
    int now = Time.now();
    if (now - lastUpdate < SEND_INTERVAL)
        {
            return;
        }
        
    digitalWrite(LEDPIN, HIGH);
    lastUpdate = now;
    
    failed = 0;
    
    Particle.publish("climate_reading", (String)temperature + ":" + (String)humidity);
    delay(5000);
    
    String payload = "{\"Temperature\":\"" + String(temperature) + "\", \"Humidity\":\"" + String(humidity) + "\"}";
    
    TCPClient *client;
    client = PubNub.publish(channel, payload);
    client->stop();
    delay(200);
    
    digitalWrite(LEDPIN, LOW);
}
