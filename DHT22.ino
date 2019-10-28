/*
 *  Simple HTTP get webclient test
 */
//THIS IS DIRECTLY FOR THE ESP8266 MCU
#include <ESP8266WiFi.h>
#include <dht.h>
#define DHT22_PIN 2//#define DHTPIN 2     // what pin we're connected to gpio02
//#define DHTTYPE DHT22   // DHT 22  (AM2302) was used
dht DHT;//(DHTPIN,DHTTYPE); //was DHT dht

const char* ssid     = "thanos";
const char* password = "9F5195BBD679AAE6";
const char* host = "santiapps.com";
unsigned long previousMillis = 0;        // will store last time was updated
const long interval = 180000; //3,600,000 = 3,600 seconds or 1 hr (180,000 = 180 secs or 
char tempString[20];
char humString[20];

//void setup() {
//  Serial.begin(115200);
//  delay(100);
// 
//  // We start by connecting to a WiFi network
// 
//  Serial.println();
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//  
//  WiFi.begin(ssid, password);
//  
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
// 
//  Serial.println("");
//  Serial.println("WiFi connected");  
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
//}
 
int value = 0;

void start_test () {
  int chk = DHT.read22(DHT22_PIN);
  float h = DHT.humidity;//dht.readHumidity();
  float t = DHT.temperature;//readTemperature();
  dtostrf(t, 4, 2, tempString);  //convert flat to char  
  dtostrf(h, 4, 2, humString);  //convert flat to char
}

void loop2() {
  ///////////INSERTED millis-sample code
    unsigned long currentMillis = millis();
  if (currentMillis - previousMillis <= interval) { //when time elapsed is LESS than interval of 30 mins    
  } else { //if deltaTimeElapsed is GREATER than target interval, sample and post...
    previousMillis = currentMillis; //sets previous-0 to current-3600
    start_test(); //sample

    //now post
    delay(5000);
  ++value;
 
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request santiapps.com/emoncms/input/post.json?apikey=bde919c2b47c5f12&node=fortnite&json={t:66,h:77}
  String url = String("/emoncms/input/post.json?apikey=thingspeakApiKey&node=fortnite&json={\"t\":") + tempString + ",\"h\":" + humString + "}";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  }
  
}
