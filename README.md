# IR Jammer for Flipper Zero

[English](#english) | [Türkçe](#türkçe)

---

<a name="english"></a>
## 🇬🇧 English

### Description
This is a simple **IR Jammer** application designed for the **Flipper Zero**. It allows you to jam infrared signals by emitting continuous bursts of IR light at a configurable frequency.

**Disclaimer:** This application is for **educational and testing purposes only**. Do not use it to disrupt legitimate communications or services. The author is not responsible for any misuse.

### Features
- **Adjustable Frequency:** Change the jamming frequency (default: 38kHz) using Left/Right buttons.
- **Toggle Jamming:** Start and stop jamming with the OK button.
- **Visual Feedback:** Screen indicates when jamming is active.

### How to Build
1. Clone this repository into your Flipper Zero firmware's `applications_user` directory.
2. Build the firmware or just this application using `fbt`.
   ```bash
   ./fbt fap_ir_jammer
   ```

### Controls
- **OK Button:** Start / Stop Jamming
- **Left / Right:** Decrease / Increase Frequency (Steps of 1kHz)
- **Back:** Exit Application

---

<a name="türkçe"></a>
## 🇹🇷 Türkçe

### Açıklama
Bu, **Flipper Zero** için tasarlanmış basit bir **Kızılötesi (IR) Karıştırıcı (Jammer)** uygulamasıdır. Ayarlanabilir bir frekansta sürekli IR ışığı yayarak kızılötesi sinyalleri karıştırmanızı sağlar.

**Yasal Uyarı:** Bu uygulama yalnızca **eğitim ve test amaçlıdır**. Meşru iletişimleri veya hizmetleri aksatmak için kullanmayınız. Yazar, herhangi bir kötüye kullanımdan sorumlu değildir.

### Özellikler
- **Ayarlanabilir Frekans:** Sol/Sağ tuşlarını kullanarak karıştırma frekansını değiştirebilirsiniz (Varsayılan: 38kHz).
- **Karıştırmayı Aç/Kapat:** OK tuşu ile karıştırmayı başlatıp durdurabilirsiniz.
- **Görsel Geri Bildirim:** Karıştırma aktif olduğunda ekranda belirtilir.

### Nasıl Derlenir (Build)
1. Bu depoyu Flipper Zero yazılımınızın `applications_user` dizinine klonlayın.
2. Yazılımı veya sadece bu uygulamayı `fbt` kullanarak derleyin.
   ```bash
   ./fbt fap_ir_jammer
   ```

### Kontroller
- **OK Tuşu:** Karıştırmayı Başlat / Durdur
- **Sol / Sağ:** Frekansı Azalt / Artır (1kHz'lik adımlarla)
- **Geri:** Uygulamadan Çık
