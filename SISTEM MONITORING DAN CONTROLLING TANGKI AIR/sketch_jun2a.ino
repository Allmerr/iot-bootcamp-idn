#include <NewPing.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "TMPL6cMF4P82S"
#define BLYNK_TEMPLATE_NAME "SMW"
#define BLYNK_AUTH_TOKEN "eOIEd9fLdGFcCFmShMhWGLsVPvWro0ay"

#define TRIGGER_PIN D0
#define ECHO_PIN D1
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

char ssid[] = "ya misikin";
char pass[] = "dark angel";

BlynkTimer timer;
void sendSensor(){
  delay(50);
  Serial.print("Jarak : " + String(sonar.ping_cm()) + " cm");
  Blynk.virtualWrite(V0, sonar.ping_cm());
}

int state;
BLYNK_WRITE(V1){
  state = param.asInt();

  if(state == 1){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Pompa HIDUP");
    Blynk.virtualWrite(V1, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Pompa Mati");
    Blynk.virtualWrite(V1, LOW);
  }
}



void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, sendSensor);
}

void loop() {
  int jarak = sonar.ping_cm();

  Serial.print("Jarak " + String(jarak) + " cm\n");

  Blynk.run();
  timer.run();
}
