#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <WebServer.h>

#define BENIM_DUGUM_ID 1
#define I2C_SDA 27
#define I2C_SCL 26
#define LORA_SCK  25
#define LORA_MISO 33
#define LORA_MOSI 32
#define LORA_NSS  14
#define LORA_RST  13
#define LORA_DIO0 35
#define ALARM_LED_PIN 2

Adafruit_MPU6050 mpu;
unsigned long sonSallanmaZamani[4] = {0, 0, 0, 0};
float SARSINTI_ESIGI = 2.0;

const char* ssid = "DEPREM_AGI";
WebServer server(80);

String webDurum = "GUVENLI (SAKIN)";
String webRenk = "#2ecc71";
String webSallananlar = "Yok";

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<meta http-equiv=\"refresh\" content=\"1\">";
  html += "<title>Akilli Deprem Sistemi</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; background-color: #e0e5ec; padding: 20px; }";
  html += "h1 { color: #2c3e50; font-size: 24px;}";
  html += ".card { background: white; padding: 30px; border-radius: 20px; box-shadow: 9px 9px 16px rgb(163,177,198,0.6); display: inline-block; margin-top: 20px; width: 90%; max-width: 400px; }";
  html += ".status { font-size: 24px; font-weight: bold; color: white; padding: 20px; border-radius: 15px; margin: 20px 0; background-color: " + webRenk + "; }";
  html += ".info { font-size: 16px; color: #7f8c8d; }";
  html += ".nodes { font-size: 22px; font-weight: bold; color: #34495e; margin-top: 10px; }";
  html += "</style></head><body>";
  html += "<h1>SENSOR FUZYONU</h1>";
  html += "<div class=\"card\">";
  html += "<h3>Sistem Durumu:</h3>";
  html += "<div class=\"status\">" + webDurum + "</div>";
  html += "<p class=\"info\">Anlik Sallanan Cihazlar:</p>";
  html += "<p class=\"nodes\">" + webSallananlar + "</p>";
  html += "</div>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(ALARM_LED_PIN, OUTPUT);
  digitalWrite(ALARM_LED_PIN, LOW);

  WiFi.softAP(ssid);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP: http://");
  Serial.println(IP);

  server.on("/", handleRoot);
  server.begin();

  Wire.begin(I2C_SDA, I2C_SCL);
  if (!mpu.begin()) {
    Serial.println("HATA! MPU6050 bulunamadi.");
    while (1) { delay(10); }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);
  LoRa.setPins(LORA_NSS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("HATA! LoRa modulu bulunamadi.");
    while (1) { delay(10); }
  }

  unsigned long baslangicGecikmesi = 30000;
  sonSallanmaZamani[1] = millis() - baslangicGecikmesi;
  sonSallanmaZamani[2] = millis() - baslangicGecikmesi;
  sonSallanmaZamani[3] = millis() - baslangicGecikmesi;

  Serial.println("Sistem Hazir!");
}

void loop() {
  server.handleClient();
  unsigned long suAn = millis();

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float x = a.acceleration.x;
  float y = a.acceleration.y;
  float z = a.acceleration.z;
  float toplamIvme = sqrt(x*x + y*y + z*z);

  if (abs(toplamIvme - 9.8) > SARSINTI_ESIGI) {
    sonSallanmaZamani[BENIM_DUGUM_ID] = suAn;
    static unsigned long sonMesajAtma = 0;
    if (suAn - sonMesajAtma > 500) {
      LoRa.beginPacket();
      LoRa.print("DEPREM:");
      LoRa.print(BENIM_DUGUM_ID);
      LoRa.endPacket();
      sonMesajAtma = suAn;
    }
  }

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String gelenMesaj = "";
    while (LoRa.available()) {
      gelenMesaj += (char)LoRa.read();
    }
    if (gelenMesaj.startsWith("DEPREM:")) {
      int gelenID = gelenMesaj.substring(7).toInt();
      if (gelenID >= 1 && gelenID <= 3 && gelenID != BENIM_DUGUM_ID) {
        sonSallanmaZamani[gelenID] = suAn;
      }
    }
  }

  bool d1 = (suAn - sonSallanmaZamani[1] < 15000);
  bool d2 = (suAn - sonSallanmaZamani[2] < 15000);
  bool d3 = (suAn - sonSallanmaZamani[3] < 15000);
  bool hepsi = (d1 && d2 && d3);
  bool biri = (d1 || d2 || d3);

  String sallananlar = "";
  if (d1) sallananlar += "[1] ";
  if (d2) sallananlar += "[2] ";
  if (d3) sallananlar += "[3] ";
  if (sallananlar == "") sallananlar = "Yok";

  if (hepsi) {
    digitalWrite(ALARM_LED_PIN, HIGH);
    delay(200);
    digitalWrite(ALARM_LED_PIN, LOW);
    delay(200);
    webDurum = "GERCEK DEPREM!";
    webRenk = "#e74c3c";
    webSallananlar = sallananlar;
  } else if (biri) {
    digitalWrite(ALARM_LED_PIN, LOW);
    webDurum = "TEYIT BEKLENIYOR...";
    webRenk = "#f39c12";
    webSallananlar = sallananlar;
  } else {
    digitalWrite(ALARM_LED_PIN, LOW);
    webDurum = "GUVENLI (SAKIN)";
    webRenk = "#2ecc71";
    webSallananlar = "Yok";
  }
}
