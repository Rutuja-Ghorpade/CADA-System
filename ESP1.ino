#define BLYNK_TEMPLATE_ID "TMPL3mzzdyIjd"
#define BLYNK_TEMPLATE_NAME "Child Abuse Detection System and Alert System"
#define BLYNK_AUTH_TOKEN "lE8aYnc_mYjBzLAc7AoTKeiE-2YdydEl"

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <TinyGPS++.h>

#define GSR 36
#define FORCE_SENSOR_PIN 35
#define RXD2 16  // GPS Module TX
#define TXD2 17  // GPS Module RX
#define Buzzer_PIN 5
#define SOS_PIN 18
#define HIGH_PIN 13
#define LOW_PIN 12
#define NORMAL_PIN 14

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "ABC";  
char pass[] = "123456789";

// GPS & MAX30100 Setup
HardwareSerial neogps(1);
TinyGPSPlus gps;


void setup() {
    Serial.begin(115200);
    neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Blynk.begin(auth, ssid, pass);
    pinMode(SOS_PIN, INPUT_PULLUP);
    pinMode(Buzzer_PIN, OUTPUT);
    pinMode(13,INPUT);
    pinMode(12,INPUT);
    pinMode(14,INPUT);

    delay(1000);
}

void loop() {
    Blynk.run();
    int sensorValue = analogRead(GSR);
    int forceReading = analogRead(FORCE_SENSOR_PIN);
    int sosValue = digitalRead(SOS_PIN);
    int bpm1 = digitalRead(HIGH_PIN);
    int bpm2 = digitalRead(LOW_PIN);
    int bpm3 = digitalRead(NORMAL_PIN);
    Serial.println(bpm1);
    Serial.println(bpm2);

    if((sensorValue >2000 && (bpm1 ==1 || bpm2 ==1) && forceReading>2500) || sosValue == LOW ) 
    {  
      Serial.println("🚨 SOS ALERT! Sending Location...");
      Blynk.virtualWrite(V5, "EMERGENCY, NEED HELP!");
      digitalWrite(Buzzer_PIN, HIGH);
      sendGPSData();
    }
    else {
        Serial.println("✅ ALL NORMAL");
        Blynk.virtualWrite(V5, "ALL NORMAL");
        digitalWrite(Buzzer_PIN, LOW);
    }
        Serial.print("📟 GSR Sensor: "); Serial.println(sensorValue);
        Blynk.virtualWrite(V1, sensorValue);
        Serial.print("🖐 Force Sensor: "); Serial.println(forceReading);
        Blynk.virtualWrite(V2, forceReading);    

}

void sendGPSData() {
    boolean newData = false;
    
    // Try fetching GPS data for 2 seconds
    for (unsigned long start = millis(); millis() - start < 2000;) {
        while (neogps.available()) {
            if (gps.encode(neogps.read())) {
                newData = true;
            }
        }
    }

    if (newData && gps.location.isValid()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        Serial.println("📍 GPS Location Sent:");
        Serial.print("Latitude: "); Serial.println(latitude, 6);
        Serial.print("Longitude: "); Serial.println(longitude, 6);

        Blynk.virtualWrite(V3, latitude);
        Blynk.virtualWrite(V4, longitude);
    } else {
        Serial.println("⚠ No GPS Data Available!");
    }
}