

//Ghazal Sadeghian-MQTT

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "Arista1377";
const char* password = "gsvs1377";
const char* mqtt_server = "192.168.1.5";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String num = "";  

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String s="";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    s=s+(char)payload[i];
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if (s.equals("ON")) {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  num = random(20);
  Serial.println("YOUR NUMBER:" + num); 
  snprintf (msg, 50, num.c_str()); 
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("numGenerator", msg);
  delay(10000);

}
