# JWS_ESP01_328P_MARYOTO_DF_BARU
Ganti Library https://github.com/enjoyneering/DFPlayer

Sudah ditest pada DFPlayer Mini dengan IC GD3200

DI PC :
C:\Users\creative1\Documents\GitHub\JWS_ESP01_328P_MARYOTO\JWS_ESP01_328P_MARYOTO_DF_BARU\JWS_328P_ESP01_TAMPIL_MARYOTO_2

Berikut setting microsd yang saya gunakan :
- File System : FAT
- Nama Drive : MP3
- Nama Folder : MP3
- Nama file terdiri dari 4 digit diawali dengan 0. Contoh : 0001.mp3


Contoh Penggunaan :
```
void play(){
  if ((millis()-previousMillis_1) > interval_1) {
  previousMillis_1 = millis();
  mp3.wakeup(2);
  mp3.playMP3Folder(3); // memainkan file mp3 di folder MP3 track 3 
  }
}
```
