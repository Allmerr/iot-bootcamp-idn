// include some library
#include "AntaresESP8266HTTP.h" // antares library
#include <NewPing.h> // library is used for working with ultrasonic distance sensors
#include <DHT.h> // library is used for working with DHT series temperature and humidity sensors
#include <MQUnifiedsensor.h> // library is used for working with gas sensors, specifically MQ series gas sensors

#define ACCESSKEY "3a7d3048b3892150:0b48c729bd743996" // set key profile antaris
#define WIFISSID "ya misikin"  // set constant wifi ssid
#define PASSWORD "dark angel"  // set constant wifi pass

#define projectName "SmartFarm_Kevin"  // set project name antaris
#define deviceName "NodeSensor" // set divice node sensor

AntaresESP8266HTTP antares(ACCESSKEY); // intilizate object

#define TRIGGER_PIN D5    // Pin Trigger HC-SR04 pada NodeMCU
#define ECHO_PIN D6       //Pin Echo HC-SR04 pada NodeMCU
#define MAX_DISTANCE 250  //Maksimum Pembacaan Jarak (cm)
#define DHTPIN D4         //Define Pin DHT
#define DHTTYPE DHT11     //Define Jenis DHT
#define Pompa D0          //Define Pin Relay 1
#define Minum D1          //Define Pin Relay 2
#define LED2 D2           //Define Pin LED

char placa[] = "ESP8266";       //Define jenis board yang digunakan
#define Voltage_Resolution 5    //Tegangan yang digunakan
#define pin A0                  //Pin yang digunakan untuk MQ-135
#define type "MQ-135"           //Jenis MQ yang digunakan
#define ADC_Bit_Resolution 10   //Resolusi Bit ADC
#define RatioMQ135CleanAir 3.6  //Nilai Udara dianggap bersih

char LED[] = "led";
char RELAY1[] = "relay1";
char RELAY2[] = "relay2";
char SERVO[] = "servo";
char AMONIA[] = "amonia";
char KELEMBABAN[] = "kelembaban";
char SUHU[] = "suhu";
char KETINGGIAN[] = "ketinggian";
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
DHT dht(DHTPIN, DHTTYPE);

unsigned long durasiKirim = 0;
unsigned long jedaKirim = 2000;

void setup() {
  Serial.begin(115200);
  antares.setDebug(true);
  antares.wifiConnection(WIFISSID, PASSWORD);
  pinMode(LED2, OUTPUT);
  pinMode(Pompa, OUTPUT);
  pinMode(Minum, OUTPUT);
  digitalWrite(LED2, LOW);
  digitalWrite(Pompa, HIGH);
  digitalWrite(Minum, HIGH);

  dht.begin();
  
  MQ135.setRegressionMethod(1);
  MQ135.init();
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ135.update();
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0 / 10);
}

void loop() {
  unsigned long time = millis();
  MQ135.update();
  MQ135.setA(102.2);
  MQ135.setB(-2.473);
  float NH4 = MQ135.readSensor();
  int jarak = sonar.ping_cm();
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  antares.add(KETINGGIAN, jarak);
  antares.add(SUHU, t);
  antares.add(KELEMBABAN, h);
  antares.add(AMONIA, NH4);
  antares.send(projectName, deviceName);
  delay(10000);
}
