/*
Connecting to a broker and subscribe to the topic "test".
Then if a motion detected publish this event.

*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xEF }; 
//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; from examples web server
//ip of ethernet shield (you can see this from your router ) 
IPAddress ip(192, 168, 1, 77);
//IPAddress server(172, 16, 0, 2);
//iot.eclipse.org=198.41.30.241
//Broker's ip address
IPAddress server(192,168,1,68);
int ledPin = 13;
int pirPin = 9;
int val = 0;


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic","hello world");
      client.publish("/test","hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);
   pinMode (ledPin,OUTPUT);
   pinMode (pirPin, INPUT);

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
   val = digitalRead(pirPin);
      digitalWrite(ledPin,val);
      
      

   if (val == 1)
   {
      digitalWrite(ledPin,LOW);
      client.publish("test","motion Detected");
      delay(5000);
   }
  
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
