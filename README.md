# Isletim_Sistemleri_Donem_Projesi

## 📌 Proje Hakkında

Bu proje, **çok katlı bir apartmanın inşaat sürecini modelleyerek** işletim sistemlerinde sıkça karşılaşılan **process (süreç), thread (iş parçacığı)** ve **semaphore (senkronizasyon)** kavramlarını öğretici bir yaklaşımla simüle etmektedir.

Her **kat bir process**, her **daire ise bir thread** olarak modellenmiştir. Ortak kullanılan **vinç** kaynağı ise `semaphore` ile senkronize edilmektedir.

## 👨‍💻 Geliştiriciler

- Ahmet Furkan Öcel — `23360859729`
- Lütfü Bedel — `21360859030`

## 🧠 Temel Kavramlar

- **Process (fork):** Her kat, ayrı bir process olarak başlatılır.
- **Thread (pthread):** Her kattaki daireler eşzamanlı olarak thread'lerle temsil edilir.
- **Semaphore:** Tek bir vinç kaynağı, semaphore ile tüm thread’ler arasında senkronize şekilde paylaşılır.

## 🔧 Derleme ve Çalıştırma

### Gereksinimler:
- POSIX uyumlu bir işletim sistemi (Linux/macOS)
- C derleyicisi (gcc önerilir)

## Youtube Videosu
