#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define BENIM_DUGUM_ID 2
#define I2C_SDA 27
#define I2C_SCL 26
#define LORA_SCK  25
#define LORA_MISO 33
#define LORA_MOSI 32
#define LORA_NSS  14
#define LORA_RST  13
#define LORA_DIO0 35

Adafruit_MPU6050 mpu;
float SARSINTI_ESIGI = 2.0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n--- Deprem Sensor Dugumu ---");

  Wire.begin(I2C_SDA, I2C_SCL);
  if (!mpu.begin()) {
    Serial.println("HATA! MPU6050 bulunamadi.");
    while (1) { delay(10); }
  }
  Serial.println("MPU6050 BASARILI!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);
  LoRa.setPins(LORA_NSS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("HATA! LoRa modulu bulunamadi.");
    while (1) { delay(10); }
  }
  Serial.println("LoRa BASARILI!");
  Serial.println("Sistem Hazir! Izleniyor...");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float x = a.acceleration.x;
  float y = a.acceleration.y;
  float z = a.acceleration.z;
  float toplamIvme = sqrt(x*x + y*y + z*z);

  Serial.print("Ivme -> X: ");
  Serial.print(x);
  Serial.print(" | Y: ");
  Serial.print(y);
  Serial.print(" | Z: ");
  Serial.println(z);

  if (abs(toplamIvme - 9.8) > SARSINTI_ESIGI) {
    Serial.println("=> SARSINTI ALGILANDI! Haber gonderiliyor...");
    static unsigned long sonMesaj = 0;
    unsigned long suAn = millis();
    if (suAn - sonMesaj > 500) {
      LoRa.beginPacket();
      LoRa.print("DEPREM:");
      LoRa.print(BENIM_DUGUM_ID);
      LoRa.endPacket();
      sonMesaj = suAn;
    }
  }

  delay(100);
}
