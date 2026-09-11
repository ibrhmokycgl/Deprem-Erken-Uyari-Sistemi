# Bitirme Projesi Raporu
## ESP32 + LoRa + MPU6050 Tabanlı Deprem Erken Uyarı Sistemi

**Öğrenciler:**
- İbrahim OKUYUCUOĞLU - 210757077

**Bölüm:** Atatürk Üniversitesi - Bilgisayar Mühendisliği
**Ders:** Seçmeli Tasarım-I

---

## 1. GİRİŞ

Depremler, dünya genelinde ve özellikle ülkemizde büyük can ve mal kayıplarına yol açan en yıkıcı doğal afetlerden biridir. Bu proje, gelişen IoT teknolojileri kullanılarak düşük maliyetli ve yerel ölçekli bir deprem erken uyarı sistemi geliştirilmesini konu almaktadır.

## 2. SİSTEM MİMARİSİ

Sistem iki ana birimden oluşmaktadır:
- **3 Adet Sensör Düğümü:** ESP32 + MPU6050 + LoRa
- **1 Adet Ana Gateway:** ESP32 + LoRa + Wi-Fi Web Arayüzü

## 3. DONANIM BİLEŞENLERİ

- ESP32 Mikrodenetleyici x4
- MPU6050 6-Eksen İvmeölçer x3
- LoRa RA-01 (SX1278) 433MHz x4
- Buzzer ve LED (alarm)

## 4. ÇALIŞMA PRENSİBİ

1. MPU6050 sensörleri sürekli ivme verisi okur
2. Toplam ivme = √(Ax² + Ay² + Az²) formülü ile hesaplanır
3. Eşik değer (2.0 m/s²) aşılırsa LoRa ile alarm paketi gönderilir
4. 3 sensörün 15 saniye içinde teyit etmesi durumunda GERÇEK DEPREM alarmı verilir

## 5. SONUÇ

Geliştirilen sistem, GSM ve internet altyapısına bağlı kalmadan LoRa teknolojisi sayesinde afet anında da çalışabilen, düşük maliyetli bir erken uyarı çözümü sunmaktadır.
