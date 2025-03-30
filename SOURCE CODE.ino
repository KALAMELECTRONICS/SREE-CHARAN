#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>


char auth[] = "DDtHJqXTWe3yLOPrzm4gh-ku-mz30V13";  

const char* ssid = "Kalam_004";
const char* password = "kalam@1234";

#define BOTtoken "8143450004:AAGXstTPh_Xuxr-ubp-eLeAx8V3VDraLPxs"
#define CHAT_ID "6204515538"


X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


#define FLAME_SENSOR_1 D1  // Flame Sensor 1 (Area 1)
#define FLAME_SENSOR_2 D2  // Flame Sensor 2 (Area 2)
#define FLAME_SENSOR_3 D4  // Flame Sensor 3 (Area 3)
#define FLAME_SENSOR_4 D3 // Flame Sensor 4 (Area 4)
#define MQ2_SENSOR A0       // MQ2 Gas Sensor
#define DHTPIN D5           // DHT Sensor
#define DHTTYPE DHT11       // DHT Type
#define BUZZER D6           // Buzzer


DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);

  pinMode(FLAME_SENSOR_1, INPUT);
  pinMode(FLAME_SENSOR_2, INPUT);
  pinMode(FLAME_SENSOR_3, INPUT);
  pinMode(FLAME_SENSOR_4, INPUT);
  pinMode(MQ2_SENSOR, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(ALERT_LED, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Connected!");

  Blynk.begin(auth, ssid, password);
  dht.begin();

  bot.sendMessage(CHAT_ID, "🔥 Fire Alert System is Online!", "");
}

void loop() {
  Blynk.run();

  int flame1 = digitalRead(FLAME_SENSOR_1);
  int flame2 = digitalRead(FLAME_SENSOR_2);
  int flame3 = digitalRead(FLAME_SENSOR_3);
  int flame4 = digitalRead(FLAME_SENSOR_4);
  int smokeLevel = analogRead(MQ2_SENSOR);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Flame Sensor 1: "); Serial.println(flame1);
  Serial.print("Flame Sensor 2: "); Serial.println(flame2);
  Serial.print("Flame Sensor 3: "); Serial.println(flame3);
  Serial.print("Flame Sensor 4: "); Serial.println(flame4);
  Serial.print("Smoke Level: "); Serial.println(smokeLevel);
  Serial.print("Temperature: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);

  // Send data to Blynk
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, smokeLevel);

  bool fireDetected = false;
  String fireMessage = "🔥 FIRE DETECTED IN: ";

  if (flame1 == LOW) { fireDetected = true; fireMessage += "Area 1, "; }
  if (flame2 == LOW) { fireDetected = true; fireMessage += "Area 2, "; }
  if (flame3 == LOW) { fireDetected = true; fireMessage += "Area 3, "; }
  if (flame4 == LOW) { fireDetected = true; fireMessage += "Area 4, "; }

  if (fireDetected) {
    digitalWrite(BUZZER, HIGH);
    digitalWrite(ALERT_LED, HIGH);
    Blynk.virtualWrite(V4, 1); // 


    fireMessage.remove(fireMessage.length() - 2);

    bot.sendMessage(CHAT_ID, fireMessage + "! 🚨 Take Immediate Action!", "");
    delay(5000);  
  } else {
    digitalWrite(BUZZER, LOW);
    digitalWrite(ALERT_LED, LOW);
    Blynk.virtualWrite(V4, 0); 
  }

  delay(2000); 
}

// CODE WAS WRITTEN BY KALAM ELECTRONICS(30-03-2025)
//@COPY RIGHTS KALAM ELECTRONICS PROJECT CENTRE&ACADEMY

