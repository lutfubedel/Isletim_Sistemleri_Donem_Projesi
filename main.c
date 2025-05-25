#include <stdio.h>      // Girdi cikti islemleri icin
#include <stdlib.h>     // Genel amacli fonksiyonlar icin (malloc, exit, vb.)
#include <unistd.h>     // POSIX fonksiyonlari icin (fork, sleep, vb.)
#include <pthread.h>    // Thread olusturma ve yonetimi icin
#include <semaphore.h>  // Semaphore islemleri icin
#include <fcntl.h>      // Semaphore flag'lari icin (O_CREAT, O_EXCL, vb.)
#include <sys/wait.h>   // Process bekleme islemleri icin
#include <string.h>     // String islemleri icin (gerekirse)

#define KAT_SAYISI 10       // Toplam kat sayisi
#define DAIRE_SAYISI 4      // Her katta 4 daire var
#define SEM_VINC "/vinc"    // Ortak kaynak: vinc icin semaphore adi

// -------------------- DAIRE THREAD FONKSIYONU --------------------
void* daire_islemi(void* arg) {
    int daire_no = *((int*)arg);         // Gelen argumani int olarak al
    free(arg);                           // Bellek sizintisi olmamasi icin heap'ten alinan bellegi bosalt

    // Vinc semaforunu ac
    sem_t* vinc = sem_open(SEM_VINC, 0);
    if (vinc == SEM_FAILED) {            // Acilamazsa hata ver
        perror("[HATA] Vinc semaforu acilamadi");
        pthread_exit(NULL);              // Thread cikisi
    }

    printf("[DAIRE %02d] Siva ve tesisat icin vinc bekleniyor...\n", daire_no);

    sem_wait(vinc);                      // Vinc kaynaginin kilidini al

    // Vinc kullanimi simulasyonu
    printf("[DAIRE %02d] Vinci kullaniyor...\n", daire_no);
    sleep(1);                            // 1 saniye bekleyerek islem simulasyonu yap

    printf("[DAIRE %02d] Vinci kullanmayi bitirdi.\n", daire_no);
    sem_post(vinc);                      // Vinc kaynagini serbest birak

    sem_close(vinc);                     // Semaphore baglantisini kapat

    pthread_exit(NULL);                  // Thread basariyla bitti
}

// -------------------- KAT PROCESS FONKSIYONU --------------------
void kat_insa_et(int kat_no) {
    pthread_t threads[DAIRE_SAYISI];     // Her daire icin thread dizisi

    printf("\n[KAT %d] Insaat basladi.\n", kat_no);

    // Her daire icin bir thread olustur
    for (int i = 0; i < DAIRE_SAYISI; i++) {
        int* daire_no = malloc(sizeof(int)); // Her daire icin dinamik arguman olustur
        if (daire_no == NULL) {
            perror("[HATA] malloc basarisiz");
            exit(1);
        }

        *daire_no = kat_no * 10 + (i + 1);   // Daire numarasi ornegi: 21, 22, 23, 24

        if (pthread_create(&threads[i], NULL, daire_islemi, daire_no) != 0) {
            perror("[HATA] Thread olusturulamadi");
            free(daire_no); // malloc basarisiz degil ama thread olusturulamadiysa da serbest birak
            exit(1);
        }
    }

    // Tum thread'lerin bitmesini bekle
    for (int i = 0; i < DAIRE_SAYISI; i++) {
        pthread_join(threads[i], NULL);     // Thread'leri bekle
    }

    printf("[KAT %d] Tum daireler tamamlandi.\n", kat_no);

    exit(0);                                // Process basariyla bitti
}

// -------------------- ANA PROGRAM --------------------
int main() {
    sem_unlink(SEM_VINC);                  // Onceki semaphore varsa sil

    // Vinc semaforu 1 kaynakla olustur
    sem_t* vinc = sem_open(SEM_VINC, O_CREAT | O_EXCL, 0644, 1);
    if (vinc == SEM_FAILED) {
        perror("[HATA] Vinc semaforu olusturulamadi");
        exit(1);
    }

    printf("Apartman insaati basliyor...\n");

    // Her kat icin process olustur
    for (int kat = 1; kat <= KAT_SAYISI; kat++) {
        pid_t pid = fork();                // Yeni process olustur

        if (pid < 0) {
            perror("[HATA] fork hatasi");  // Fork hatasi kontrolu
            exit(1);
        }

        if (pid == 0) {
            // Cocuk process: kendi katini insa eder
            kat_insa_et(kat);
        } else {
            // Ana process: cocuk bitene kadar bekler
            wait(NULL);
        }
    }

    sem_close(vinc);                       // Vinc semaforu kapat
    sem_unlink(SEM_VINC);                  // Vinc semaforu sistemden sil

    printf("\nApartman insaati basariyla tamamlandi.\n");
    return 0;                              // Program sonlandi
}
