#define BLYNK_TEMPLATE_ID "TMPL3mzzdyIjd"
#define BLYNK_TEMPLATE_NAME "Child Abuse Detection System and Alert System"
#define BLYNK_AUTH_TOKEN "lE8aYnc_mYjBzLAc7AoTKeiE-2YdydEl"


// #define BLYNK_TEMPLATE_ID "TMPL3ELq_DNup"
// #define BLYNK_TEMPLATE_NAME "Child Abuse Detection System"
// #define BLYNK_AUTH_TOKEN "EAxMQJKVUReWwuJtSVkxBLNrVNYAxpEo"
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000
#define HIGH_PIN 13
#define LOW_PIN 12
#define NORMAL_PIN 14

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "ABC";  
char pass[] = "123456789";

PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup()
{
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
    pinMode(13,OUTPUT);
    pinMode(12,OUTPUT);
    pinMode(14,OUTPUT);

    Serial.print("Initializing pulse oximeter..");

    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
   pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
    Blynk.run();
    pox.update();
    float bpm = pox.getHeartRate();
    float spo2 = pox.getSpO2();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Blynk.virtualWrite(V0,pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        if(bpm>0 && bpm<60){
         digitalWrite(12,HIGH);
         digitalWrite(13,LOW);
         digitalWrite(14,LOW);
         Serial.print("LOW BPM");
        }else if(bpm>60 &&bpm<80){
         digitalWrite(12,LOW);
         digitalWrite(13,LOW);
         digitalWrite(14,HIGH);
         Serial.print("NORMAL BPM");
        }else if(bpm>81)
        {
         digitalWrite(12,LOW);
         digitalWrite(13,HIGH);
         digitalWrite(14,LOW);
         Serial.print("HIGH BPM");
        }else
        {
         digitalWrite(12,LOW);
         digitalWrite(13,LOW);
         digitalWrite(14,LOW);
         Serial.print("NOT Getting BPM");
        }
 tsLastReport = millis();
}
}