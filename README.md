# 🌍 Deprem Erken Uyarı Sistemi
### ESP32 + LoRa + MPU6050 Tabanlı Dağıtık Sismik Algılama Ağı

## 📖 Proje Hakkında
Bu proje, Atatürk Üniversitesi Bilgisayar Mühendisliği Bölümü Seçmeli Tasarım dersi kapsamında geliştirilmiş bir bitirme projesidir. Sistem, depremin yıkıcı S-dalgaları gelmeden önce P-dalgalarını tespit ederek saniyeler öncesinden uyarı verir.

## 🎯 Temel Özellikler
- 3 Sensörlü Füzyon: 3 farklı noktadan eş zamanlı sarsıntı doğrulaması
- LoRa Haberleşme: GSM/Internet olmadan kilometrelerce menzil
- Web Arayüzü: Telefondan anlık izleme

## 🔧 Donanım
- ESP32 x4
- MPU6050 (GY-521) x3
- LoRa RA-01 (SX1278) 433MHz x4
- Buzzer + LED

## 📂 Proje Yapısı
- firmware/gateway/ → Ana gateway kodu
- firmware/sensor_node/ → Sensör düğüm kodu
- docs/ → Bitirme raporu

## 👥 Geliştiriciler
- İbrahim OKUYUCUOĞLU 

Atatürk Üniversitesi - Bilgisayar Mühendisliği
