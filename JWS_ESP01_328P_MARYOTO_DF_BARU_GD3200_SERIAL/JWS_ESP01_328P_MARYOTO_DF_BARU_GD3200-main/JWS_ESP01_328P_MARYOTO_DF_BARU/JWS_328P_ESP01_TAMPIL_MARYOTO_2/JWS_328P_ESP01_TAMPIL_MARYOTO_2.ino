/* ================================================
  0 Latitude : -6.21
  5 Longitude : 106.85
  10 Timezone : 7.00

  15 adj_subuh : 2    | 32 tpl_subuh : 1    | 49 iqm_subuh : 2
  17 adj_dzuhur : 2   | 34 tpl_dzuhur : 1   | 51 iqm_dzuhur : 2
  19 adj_ashar : 2    | 36 tpl_ashar : 1    | 53 iqm_ashar : 2
  21 adj_maghrib : 2  | 38 tpl_maghrib : 1  | 55 iqm_maghrib : 2
  23 adj_isya : 2     | 40 tpl_isya : 1     | 57 iqm_isya : 2
  25 adj_imsak : 0    | 42 tpl_imsak : 1    | 59 lama_adzan : 4
  27 adj_terbit : 0   | 44 tpl_terbit : 0   |
  29 adj_dhuha : 2    | 46 tpl_dhuha : 0    | 61 lama_imsak : 0

  63 sesuaikan_tgl_hijriyah : 1 | 67 sesuaikan_volume : 20
  65 sesuaikan_bright : 10      | 69 sesuaikan_volume_adzan : 15

  71 Nama Masjid  : Mushola Tetep Iman Almh. Hj. Sri Loeloet Binti Mangun Digdo Di Gunung Wiyu
  221 Setelah Iqomah : Matikan HP - Luruskan Shaf - Matikan HP - Luruskan Shaf
  371 Informasi 1 : Jadikan Sabar dan Sholat Sebagai Penolong mu
  521 Informasi 2 : Sudahkah anda sholat ??
  671 Hari Besar : Selamat Hari Raya Idul Adha 10 Dzuhijjah 1442 H

  785 beep_status : 0

  773 tpl_nama : 1
  775 tpl_info1 : 0
  777 tpl_info2 : 0
  779 tpl_stlh_iqomah : 0


  (27-04-2021 15:40) - Global variables use 869 bytes (42%) of dynamic memory, leaving 1179 bytes for local variables. Maximum is 2048 bytes.
  (27-04-2021 16:00) - Pesan setelah iqomah diambil dari eeprom
                   - Perubahan eeprom info 1 ke 371, b_beep ke 785
  (28-04-2021 12:00) - Global variables use 871 bytes (42%) of dynamic memory, leaving 1177 bytes for local variables. Maximum is 2048 bytes.
                   - Penambahan info 2
  (28-04-2021 16:00) - Global variables use 878 bytes (42%) of dynamic memory, leaving 1170 bytes for local variables. Maximum is 2048 bytes.
                   - Penambahan opsi tampilkan nama masjid info1 info2

  (21-01-2022)       - Ganti Library https://github.com/enjoyneering/DFPlayer
                   - mp3.playMP3Folder(1); //1=track, folder name must be "mp3" or "MP3" & files in folder must start with 4 decimal digits with leading zeros
                   - Flush Serial sebelum masuk TPL_HH_WKT_MSK();
                   - Hilangkan Serial di TPL_HH_WKT_MSK();

  (30-03-2023)     Global variables use 860 bytes (41%) of dynamic memory, leaving 1188 bytes for local variables. Maximum is 2048 bytes. (serial Nyala)
                   Global variables use 852 bytes (41%) of dynamic memory, leaving 1196 bytes for local variables. Maximum is 2048 bytes. (tanpa serial)
                   Global variables use 844 bytes (41%) of dynamic memory, leaving 1204 bytes for local variables. Maximum is 2048 bytes.




  ================================================ */

#include <DMD3asis.h>
#include <Wire.h>
#include "RTClib.h"
#include "SholahTimeCalulation.h"

#include <DFPlayer.h>
DFPlayer mp3;

#include <EEPROM.h>

DMD3 display(1, 1);

#include <SystemFont5x7.h>
#include <angka6x13.h>
//#include <Font5x7.h>
#include <font/Font4x6.h>

RTC_DS3231 rtc;

unsigned long p_e_0_names = 0, p_e_1_jam = 0, p_e_2_hjry = 0, p_e_4_jmsk = 0, p_e_3_wksol = 0, p_e_5_iqomah = 0, p_e_6_lurus = 0;
byte evt_0 = 0, evt_1 = 0, evt_2 = 1, evt_3 = 1, evt_4 = 1, evt_5 = 1, evt_6 = 1;
int iqm_ee, sis_wkt_iqo, du_iq, iqm_menit, iqm_menit_sisa, iqm_detik;

int b_a[8];
int b_t[8];
byte flag = 0, flag_mp3 = 0, flag_hijriyah = 0; //flag_mp3 untuk getar pada saat masuk jam //flag2 = 0;

int Tahun;
byte Bulan, Tanggal, Jam, Menit, Detik, hariIni;

int mode = 1;

int ash, bsh;

//String txhh, txmm, txss, tx_hh_mm, tx_hh_mm_ss, namaHarimasehi ;
String tx_hh_mm; //, tx_hh_mm_ss; //, namaHarimasehi ;
String waktu_tbt, waktu_dhu, waktu_sbh, waktu_dzr, waktu_ims, waktu_asr, waktu_mgr, waktu_isy; //, jss;
String wkt_iqomah, tpl_iqo_ss, namaMasukWkt, textIqohitmun;//, wkt_ims_sat;

int b_bright, b_volume, b_volume_adzan;
byte b_beep, b_tpl_nama, b_tpl_info1, b_tpl_info2, b_tpl_stlh_iqomah;

int dur_tpl_tx_sol;
String slt_sek;
String cck;
int  j_s_s = 0;
String tx_ser;
byte wkt_msk_skrg = 0;

String isi_tgl_hijrii;

const char namaBulanMasehi[12][12] PROGMEM = {"Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober", "November", "Desember"};
const char namaHariMasehi[7][7] PROGMEM = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
const char namaWaktuSholat[8][12] PROGMEM = {"Imsak ", " Subuh ", " Syuruq ", " Dhuha ", " Dzuhur ", " Ashar ", " Maghrib ", " Isya "};
//const char pesan_hariRaya[6][60] PROGMEM = {"Hari Maulid Nabi Muhammad SAW ", "Selamat Hari Raya Idul Fitri ", "Selamat Hari Raya Idul Adha ", "Jadikan Sabar dan Sholat Sebagai Penolongmu", "Selamat Tahun Baru Islam ", "Selamat Isra Miraj "};

void scan() {
  display.refresh();
}
void setup() {

  Serial.begin (9600);
  // mp3_set_serial (Serial);

  EEPROM.get(67, b_volume);
  EEPROM.get(69, b_volume_adzan);
  EEPROM.get(785, b_beep);


  //  mp3.setVolume(b_volume);

  //  if (b_beep == 1){
  //
  DDRD |= (1 << DDD2) ; //pin 2 is in output mode
  //
  //     for (int dd = 0; dd < 4; dd++){
  //        if (dd % 2 == 0){
  //          //digitalWrite(buzzer,HIGH);
  //          PORTD |=(1<< PORTD2);
  //        }
  //        else
  //        {
  //          //digitalWrite(buzzer,LOW);
  //          PORTD &= ~(1<<PORTD2);
  //        }
  //        delay(250);
  //     }
  //  }

  EEPROM.get(65, b_bright); // Baca EEPROM kecerahan

  Timer1.initialize(2000);
  Timer1.attachInterrupt(scan);
  Timer1.pwm(9, b_bright);
  display.setDoubleBuffer(1);
  // Wire.begin();
  rtc.begin();
  display.clear();

  // mp3Serial.begin(9600);
  mp3.begin(Serial, 100, DFPLAYER_HW_247A, false);
  //  Serial.swap();     //now hardware serial on GPIO15(TX) & GPIO13(RX)
  Serial.flush();    //clear serial library buffer, ALWAYS CLEAN BUFFER AFTER SERIAL SWAP!!!
  mp3.stop();        //if player was runing during ESP8266 reboot
  mp3.reset();       //reset all setting to default
  mp3.setSource(2);  //1=USB-Disk, 2=TF-Card, 3=Aux, 4=Sleep, 5=NOR Flash
  mp3.setEQ(0);      //0=Off, 1=Pop, 2=Rock, 3=Jazz, 4=Classic, 5=Bass
  mp3.setVolume(b_volume); //0..30, module persists volume on power failure

}
void loop() {
  switch (mode) {
    case 0:
      evt_0 = 1; evt_1 = 1; evt_2 = 0; evt_3 = 1; evt_4 = 1; evt_5 = 1; evt_6 = 1;
      EEPROM.get(773, b_tpl_nama);
      //Serial.println(b_tpl_nama);
      if (b_tpl_nama == 1) {
        TPL_HH_TGL(3);
      } else {
        mode = 10;
      }
      break;
    case 1:
      evt_0 = 1; evt_1 = 0; evt_2 = 1; evt_3 = 1; evt_4 = 1; evt_5 = 1; evt_6 = 1;
      TPL_HH();
      break;
    case 2:
      evt_0 = 1; evt_1 = 1; evt_2 = 0; evt_3 = 1; evt_4 = 1; evt_5 = 1; evt_6 = 1;
      TPL_HH_TGL(1);
      break;
    case 3:
      evt_0 = 1; evt_1 = 0; evt_2 = 1; evt_3 = 1; evt_4 = 1; evt_5 = 1; evt_6 = 1;
      TPL_HH();
      break;
    case 4:
      evt_0 = 1; evt_1 = 1; evt_2 = 0; evt_3 = 1; evt_4 = 1; evt_5 = 1; evt_6 = 1;
      TPL_HH_TGL(5);
      break;
    case 5:
      evt_0 = 1; evt_1 = 1; evt_2 = 1; evt_3 = 1; evt_4 = 0; evt_5 = 1; evt_6 = 1;
      Serial.flush();
      TPL_HH_WKT_MSK();
      break;
    case 6:
      evt_0 = 1; evt_1 = 1; evt_2 = 1; evt_3 = 1; evt_4 = 1; evt_5 = 0; evt_6 = 1;
      TPL_QD_IQM();
      break;
    case 7:
      evt_0 = 1; evt_1 = 1; evt_2 = 0; evt_3 = 1; evt_4 = 1; evt_5 = 1; evt_6 = 1;
      EEPROM.get(779, b_tpl_stlh_iqomah);
      if (b_tpl_stlh_iqomah == 1) {
        TPL_HH_TGL(4);
      } else {
        mode = 1;
      }
      break;
    case 8:
      evt_0 = 1; evt_1 = 0; evt_2 = 1; evt_3 = 1; evt_4 = 1; evt_5 = 1; evt_6 = 1;
      TPL_HH();
      break;
    case 9:
      evt_0 = 1; evt_1 = 1; evt_2 = 0; evt_3 = 1; evt_4 = 1; evt_5 = 1; evt_6 = 1;
      TPL_HH_TGL(2);
      break;
    case 10:
      evt_0 = 1; evt_1 = 1; evt_2 = 0; evt_3 = 1; evt_4 = 1; evt_5 = 1; evt_6 = 1;
      EEPROM.get(775, b_tpl_info1);
      if (b_tpl_info1 == 1) {
        TPL_HH_TGL(6);
      } else {
        mode = 11;
      }
      break;
    case 11:
      evt_0 = 1; evt_1 = 1; evt_2 = 0; evt_3 = 1; evt_4 = 1; evt_5 = 1; evt_6 = 1;
      EEPROM.get(777, b_tpl_info2);;
      if (b_tpl_info2 == 1) {
        TPL_HH_TGL(7);
      } else {
        mode = 3;
      }
      break;

  }

  while (Serial.available() > 0 ) {

    tx_ser  = Serial.readStringUntil('\r\n');
    //tx_ser  = Serial.readString();

  }

  if (tx_ser.length() > 0) {
    // Setting Jam === SJ=23-59-55-24-05-2020-01-15-20   SJ=23-59-20-08-11-2019-01-15-20 4 digit terakhir (10-15) = brightness-volume
    // SJ=23-59-55-24-05-2020-01-15-20-15     -15 = volume adzan
    // SJ=04-30-50-24-03-2021-01-12-20-00-00  -00 = beep status

    if (tx_ser.substring(0, 2) == "SJ") {
      Serial.println(tx_ser);

      String bnm_mesjid = readString(71);

      rtc.adjust(DateTime(tx_ser.substring(18, 22).toInt(), tx_ser.substring(15, 17).toInt(), tx_ser.substring(12, 14).toInt(), tx_ser.substring(3, 5).toInt(), tx_ser.substring(6, 8).toInt(), tx_ser.substring(9, 11).toInt()));
      EEPROM.put(63, tx_ser.substring(23, 25).toInt()); // Adj Hijriyah
      EEPROM.put(65, tx_ser.substring(26, 28).toInt()); // Adj Kecerahan
      EEPROM.put(67, tx_ser.substring(29, 31).toInt()); // Adj Volume
      EEPROM.put(69, tx_ser.substring(32, 34).toInt()); // Adj Volume Adzan

      writeString(71, bnm_mesjid);

      EEPROM.put(785, tx_ser.substring(35, 37).toInt()); // beep_status

      CERAH_VOLUME();

      BUZZ();
    }

    // Setting Jam === SA=23-59-55-24-05-2020   SJ=23-59-20-08-11-2019
    if (tx_ser.substring(0, 2) == "SA") {
      //Serial.println(tx_ser);

      rtc.adjust(DateTime(tx_ser.substring(18, 22).toInt(), tx_ser.substring(15, 17).toInt(), tx_ser.substring(12, 14).toInt(), tx_ser.substring(3, 5).toInt(), tx_ser.substring(6, 8).toInt(), tx_ser.substring(9, 11).toInt()));

      BUZZ();
    }

    // Setting lama Iqomah === IQ=02-02-02-02-02-04 === IQ=03-03-03-03-03-05
    else if (tx_ser.substring(0, 2) == "IQ") {
      //Serial.println(tx_ser);

      byte blama_imsak;

      EEPROM.get(61, blama_imsak);

      EEPROM.put(49, tx_ser.substring(3, 5).toInt());
      EEPROM.put(51, tx_ser.substring(6, 8).toInt());
      EEPROM.put(53, tx_ser.substring(9, 11).toInt());
      EEPROM.put(55, tx_ser.substring(12, 14).toInt());
      EEPROM.put(57, tx_ser.substring(15, 17).toInt());
      EEPROM.put(59, tx_ser.substring(18, 20).toInt());

      EEPROM.put(61, blama_imsak);

      BUZZ();

    }

    // Setting Koreksi waktu sholat === KR=02-02-02-02-02-02-00-00 == 02-00-02 imsak dan dhuha
    else if (tx_ser.substring(0, 2) == "KR") {
      //Serial.println(tx_ser);

      byte btpl_subuh;

      EEPROM.get(32, btpl_subuh);

      EEPROM.put(15, tx_ser.substring(3, 5).toInt());
      EEPROM.put(17, tx_ser.substring(6, 8).toInt());
      EEPROM.put(19, tx_ser.substring(9, 11).toInt());
      EEPROM.put(21, tx_ser.substring(12, 14).toInt());
      EEPROM.put(23, tx_ser.substring(15, 17).toInt());
      EEPROM.put(25, tx_ser.substring(18, 20).toInt());
      EEPROM.put(27, tx_ser.substring(21, 23).toInt());
      EEPROM.put(29, tx_ser.substring(24, 26).toInt());

      EEPROM.put(32, btpl_subuh);

      BUZZ();
    }

    // Setting Tampil waktu sholat === TP=01-01-01-01-01-01-01-01
    else if (tx_ser.substring(0, 2) == "TP") {

      //Serial.println(tx_ser);

      byte biqo_subuh;

      EEPROM.get(49, biqo_subuh);

      EEPROM.put(32, tx_ser.substring(6, 8).toInt());
      EEPROM.put(34, tx_ser.substring(15, 17).toInt());
      EEPROM.put(36, tx_ser.substring(18, 20).toInt());
      EEPROM.put(38, tx_ser.substring(21, 23).toInt());
      EEPROM.put(40, tx_ser.substring(24, 26).toInt());
      EEPROM.put(42, tx_ser.substring(3, 5).toInt());
      EEPROM.put(44, tx_ser.substring(9, 11).toInt());
      EEPROM.put(46, tx_ser.substring(12, 14).toInt());

      EEPROM.put(49, biqo_subuh);

      BUZZ();
    }

    // Setting Nama masjid === NM=Masjid Raya Akbar Maulana - Ampera Raya // NM=Musholah "Tetep Iman" Almh.Hj.Sri Loeloet

    else if (tx_ser.substring(0, 2) == "NM") {
      //Serial.println(tx_ser);

      String isi_namamasjid = tx_ser.substring(3, tx_ser.length());
      writeString(71, isi_namamasjid);

      BUZZ();
    }

    // Setting Info === IN=Sudahkan Anda Sholat ??
    else if (tx_ser.substring(0, 2) == "IN") {
      //Serial.println(tx_ser);

      String isi_info = tx_ser.substring(3, tx_ser.length());
      writeString(371, isi_info);

      BUZZ();
    }

    // Setting Info 2 === I2=Sudahkan Anda Sholat ??
    else if (tx_ser.substring(0, 2) == "I2") {
      //Serial.println(tx_ser);

      String isi_info2 = tx_ser.substring(3, tx_ser.length());
      writeString(521, isi_info2);

      BUZZ();
    }

    // Setting setelah iqomah === SI=Matikan HP - Luruskan Shaf - Matikan HP - Lurusk
    else if (tx_ser.substring(0, 2) == "SI") {
      //Serial.println(tx_ser);

      String isi_stlh_iqomah = tx_ser.substring(3, tx_ser.length());
      writeString(221, isi_stlh_iqomah);

      BUZZ();
    }

    // Setting tampil nama masjid, info1, info2 --- TI=01-01-01-01
    else if (tx_ser.substring(0, 2) == "TI") {
      //Serial.println(tx_ser);

      EEPROM.put(773, tx_ser.substring(3, 5).toInt());
      EEPROM.put(775, tx_ser.substring(6, 8).toInt());
      EEPROM.put(777, tx_ser.substring(9, 11).toInt());
      EEPROM.put(779, tx_ser.substring(12, 14).toInt());


      BUZZ();
    }

    // Setting Lokasi === LL=-6.287617|106.821189|7 === LL=-3.3186|114.5944|8
    else if (tx_ser.substring(0, 2) == "LL") {
      //Serial.println(tx_ser);

      for (int i = 0; i < tx_ser.length(); i++) {
        if (tx_ser.substring(i, i + 1) == "|") {

          EEPROM.put(0, (tx_ser.substring(3, i)).toDouble());
          EEPROM.put(5, (tx_ser.substring(i + 1, tx_ser.length() - 3)).toDouble());
          EEPROM.put(10, (tx_ser.substring(tx_ser.length() - 2, tx_ser.length())).toDouble());

          break;
        }
      }

      BUZZ();
    }
    
    // Ambil kalender hijriyah dari ESP-01
    else if (tx_ser.substring(0, 2) == "HR") {
      //Serial.println(tx_ser);
      isi_tgl_hijrii = tx_ser.substring(3, tx_ser.length());
      Serial.println(isi_tgl_hijrii);
    }
    
    // Ambil jadwal dari ESP-01
    else if (tx_ser.substring(0, 2) == "JR") {
      //Serial.println(tx_ser);
      isi_tgl_hijrii = tx_ser.substring(3, tx_ser.length());
      Serial.println(isi_tgl_hijrii);
    }
    
    // Ambil Tanggal dari ESP-01
    else if (tx_ser.substring(0, 2) == "TR") {
      //Serial.println(tx_ser);
      isi_tgl_hijrii = tx_ser.substring(3, tx_ser.length());
      Serial.println(isi_tgl_hijrii);
    }
    tx_ser = "";
  } // akhir serial lenght


}
void BUZZ() {
  PORTD |= (1 << PORTD2);
  delay(200);
  PORTD &= ~(1 << PORTD2);
}
String Konversi(int sInput) {
  if (sInput < 10)
  {
    return"0" + String(sInput);
  }
  else
  {
    return String(sInput);
  }
}
void WAKTU() {                        // AMBIL WAKTU
  DateTime now = rtc.now();
  Tahun = now.year();
  Bulan = now.month();
  Tanggal = now.day();
  Jam = now.hour();
  Menit = now.minute();
  Detik = now.second();
  hariIni = now.dayOfTheWeek();

  //txhh = Konversi(Jam);
  //txmm = Konversi(Menit);
  //txss = Konversi(Detik);

  if (Detik % 2 == 0) {
    tx_hh_mm = Konversi(Jam) + " " + Konversi(Menit);
  }
  else
  {
    tx_hh_mm = Konversi(Jam) + ":" + Konversi(Menit);
  }

  cck = Konversi(Jam) + ":" + Konversi(Menit);
}
void TPL_HH () {                      // MENAMPILKAN JAM BESAR

  WAKTU();
  j_s_s = 0;

  // Ambil tanggal hijriyah

  // txt_kirim = "RH=30-03-2023-4-1";

  int b_ses_hijriyah;
  EEPROM.get(63, b_ses_hijriyah);
  String txt_kirim = "RH=" + String(Konversi(Tanggal)) + "-" + String(Konversi(Bulan)) + "-" + String(Konversi(Tahun)) + "-" + String(hariIni) + "-" + String(b_ses_hijriyah) ;
  
  if (flag_hijriyah == 0) {
    Serial.println(txt_kirim);
    flag_hijriyah = 1;
  }
  //isi_tgl_hijrii =


  if ( !evt_1 && (millis() - p_e_1_jam) > 50) {
    p_e_1_jam = millis();

    if (ash < 240) { // if(ash < 240){
      ash++;

      display.clear();
      display.setFont(angka6x13);

      if (ash < 17) {
        display.drawText(1, ash - 16, Konversi(Jam));
        display.drawText(18, ash - 16, Konversi(Menit));
      }
      else if (ash > 16 && ash < 223)
      {
        display.drawText(1, 0, Konversi(Jam));
        display.drawText(18, 0, Konversi(Menit));

      }
      else if (ash > 222) //else if(ash>142)
      {
        display.drawText(1, (-1 * (-222 + ash)), Konversi(Jam));
        display.drawText(18, (-1 * (-222 + ash)), Konversi(Menit));
      }


      display.setFont(SystemFont5x7);


      if (Detik % 2 == 0)
      {
        // display.drawChar(14, 4, ':');
        display.drawFilledRect(15, 4, 16, 5, 1);
        display.drawFilledRect(15, 10, 16, 11, 1);
      }
      else
      {
        //display.drawChar(14, 4, ' ');
        display.drawFilledRect(15, 4, 16, 5, 0);
        display.drawFilledRect(15, 10, 16, 11, 0);
      }
      display.swapBuffers();
      AMBIL_WAKTU_SHOLAT();
//      if (ash % 20 == 0)
//      {
//        Serial.print(Konversi(Jam)); Serial.print(":"); Serial.print(Konversi(Menit)); Serial.print(":"); Serial.println(Konversi(Detik));
//      }
    }
    else
    {
      if (mode == 1)
      {
        mode = 0; // NAMA MASJID
      }
      else if (mode == 3)
      {
        mode = 2;  // TANGGAL HIJRIYAH
      }
      else if (mode == 8)
      {
        // Ambil jadwal dari ESP
        String txt_kirim_jadwal = "RJ=" + String(Konversi(Tanggal)) + "-" + String(Konversi(Bulan)) + "-" + String(Konversi(Tahun));
        Serial.println(txt_kirim_jadwal);
        mode = 4;  // WAKTU SHOLAT
      }
      ash = 0;
      flag_hijriyah = 0;
    }
  }
}
void TPL_HH_TGL(int pilih) {          // MENAMPILKAN TANGGAL HIJRIYAH
  j_s_s = 0;
  WAKTU();
  AMBIL_WAKTU_SHOLAT();

  String str_tanggal_hijriyah;
  char isi_hari_raya[60];


  if (pilih == 1) { // ============= TANGGAL HIJRIYAH
    str_tanggal_hijriyah = isi_tgl_hijrii;


    /* kode_hari_raya=(str_tanggal_hijriyah.substring(0, 1)).toInt();

      if(flag==0){ // Menulis EEPROM Sekali pada pukul 01:00
      if(Jam == 0 && (Menit == 0 || Menit == 1 || Menit == 2 || Menit == 3 || Menit == 4))
      {
         if(kode_hari_raya == 1){

          memset(isi_hari_raya, 0, 60);
          strcpy_P(isi_hari_raya, pesan_hariRaya[0]);

          writeString(121, isi_hari_raya + str_tanggal_hijriyah.substring(1, str_tanggal_hijriyah.length()));
          flag = 1; flag2 = 1;

          // Serial.println(F("tulis Maulid"));
        }
        else if(kode_hari_raya == 2){

          memset(isi_hari_raya, 0, 60);
          strcpy_P(isi_hari_raya, pesan_hariRaya[1]);

          writeString(121, isi_hari_raya + str_tanggal_hijriyah.substring(1, str_tanggal_hijriyah.length()));
          flag = 1; flag2 = 1;

          // Serial.println(F("tulis Idul Fitri"));
        }
        else if(kode_hari_raya == 3){

          memset(isi_hari_raya, 0, 60);
          strcpy_P(isi_hari_raya, pesan_hariRaya[2]);

          writeString(121, isi_hari_raya + str_tanggal_hijriyah.substring(1, str_tanggal_hijriyah.length()));
          flag = 1; flag2 = 1;

          // Serial.println(F("tulis Idul Adha"));
        }
        else if(kode_hari_raya == 5){

          memset(isi_hari_raya, 0, 60);
          strcpy_P(isi_hari_raya, pesan_hariRaya[4]);

          writeString(121, isi_hari_raya + str_tanggal_hijriyah.substring(1, str_tanggal_hijriyah.length()));
          flag = 1; flag2 = 1;

          // Serial.println(F("tulis Tahun Baru"));
        }
        else if(kode_hari_raya == 6){

          memset(isi_hari_raya, 0, 60);
          strcpy_P(isi_hari_raya, pesan_hariRaya[5]);

          writeString(121, isi_hari_raya + str_tanggal_hijriyah.substring(1, str_tanggal_hijriyah.length()));
          flag = 1; flag2 = 1;

          // Serial.println(F("tulis Isra Miraj"));
        }
        else
        {
          if (flag2 == 1){
            memset(isi_hari_raya, 0, 60);
            strcpy_P(isi_hari_raya, pesan_hariRaya[3]);

            writeString(121, isi_hari_raya);
            flag = 1;

            // Serial.println(F("tulis Hari Biasa"));
            flag2 = 0;
          }
        }
      }
      }

      str_tanggal_hijriyah = str_tanggal_hijriyah.substring(1, str_tanggal_hijriyah.length());
    */

  }
  else if (pilih == 2) { // ============= TANGGAL MASEHI
    str_tanggal_hijriyah = isi_tgl_hijrii;
    /*
    // Hari Masehi
    char DayName[7];
    memset(DayName, 0, 7);
    strcpy_P(DayName, namaHariMasehi[hariIni]);

    // Bulan Masehi
    char BulanName[12];
    memset(BulanName, 0, 12);
    strcpy_P(BulanName, namaBulanMasehi[Bulan - 1]);

    str_tanggal_hijriyah =  String(DayName) + ", " + String(Tanggal) + " " + String(BulanName) + " " + String(Tahun);
    */
  }
  else if (pilih == 3) { // ============= NAMA MASJID
    str_tanggal_hijriyah = readString(71);
  }
  else if (pilih == 4) { // ============= LURUSKAN
    str_tanggal_hijriyah = readString(221);
  }
  else if (pilih == 5) { // ============= JWS
    str_tanggal_hijriyah = isi_tgl_hijrii;
    
    /*

    // Imsak 04:32 Subuh 04:42 Terbit 06:05 Dhuha 06:27 Dzuhur 11:58 Ashar 15:20 Maghrib 17:51 Isya 19:06
    // Imsak 04:32 Terbit 06:03 Dhuha 06:25 Dzuhur 11:58 Ashar 15:20 Maghrib 17:51 Isya 19:06

    EEPROM.get(42, b_t[0]); //imsak
    EEPROM.get(32, b_t[1]); //subuh
    EEPROM.get(44, b_t[2]); //terbit
    EEPROM.get(46, b_t[3]); //dhuha
    EEPROM.get(34, b_t[4]); //dzuhur
    EEPROM.get(36, b_t[5]); //ashar
    EEPROM.get(38, b_t[6]); //maghrib
    EEPROM.get(40, b_t[7]); //isya

    str_tanggal_hijriyah = "";

    if (b_t[0] == 1) {
      char nama_waktu[8];
      memset(nama_waktu, 0, 8);
      strcpy_P(nama_waktu, namaWaktuSholat[0]);

      str_tanggal_hijriyah = str_tanggal_hijriyah + nama_waktu + waktu_ims;
    }
    else
    {
      str_tanggal_hijriyah = str_tanggal_hijriyah;
    }

    if (b_t[1] == 1) {
      char nama_waktu[8];
      memset(nama_waktu, 0, 8);
      strcpy_P(nama_waktu, namaWaktuSholat[1]);

      str_tanggal_hijriyah = str_tanggal_hijriyah + nama_waktu + waktu_sbh;
    }
    else
    {
      str_tanggal_hijriyah = str_tanggal_hijriyah;
    }

    if (b_t[2] == 1) {
      char nama_waktu[8];
      memset(nama_waktu, 0, 8);
      strcpy_P(nama_waktu, namaWaktuSholat[2]);

      str_tanggal_hijriyah = str_tanggal_hijriyah + nama_waktu + waktu_tbt;
    }
    else
    {
      str_tanggal_hijriyah = str_tanggal_hijriyah;
    }

    if (b_t[3] == 1) {
      char nama_waktu[8];
      memset(nama_waktu, 0, 8);
      strcpy_P(nama_waktu, namaWaktuSholat[3]);

      str_tanggal_hijriyah = str_tanggal_hijriyah + nama_waktu + waktu_dhu;
    }
    else
    {
      str_tanggal_hijriyah = str_tanggal_hijriyah;
    }

    if (b_t[4] == 1) {
      char nama_waktu[8];
      memset(nama_waktu, 0, 8);
      strcpy_P(nama_waktu, namaWaktuSholat[4]);

      str_tanggal_hijriyah = str_tanggal_hijriyah + nama_waktu + waktu_dzr;
    }
    else
    {
      str_tanggal_hijriyah = str_tanggal_hijriyah;
    }

    if (b_t[5] == 1) {
      char nama_waktu[8];
      memset(nama_waktu, 0, 8);
      strcpy_P(nama_waktu, namaWaktuSholat[5]);

      str_tanggal_hijriyah = str_tanggal_hijriyah + nama_waktu + waktu_asr;
    } else {
      str_tanggal_hijriyah = str_tanggal_hijriyah;
    }

    if (b_t[6] == 1) {
      char nama_waktu[8];
      memset(nama_waktu, 0, 8);
      strcpy_P(nama_waktu, namaWaktuSholat[6]);

      str_tanggal_hijriyah = str_tanggal_hijriyah + nama_waktu + waktu_mgr;
    } else {
      str_tanggal_hijriyah = str_tanggal_hijriyah;
    }

    if (b_t[7] == 1) {
      char nama_waktu[8];
      memset(nama_waktu, 0, 8);
      strcpy_P(nama_waktu, namaWaktuSholat[7]);

      str_tanggal_hijriyah = str_tanggal_hijriyah + nama_waktu + waktu_isy;
    } else {
      str_tanggal_hijriyah = str_tanggal_hijriyah;
    }*/
  }
  else if (pilih == 6) { // ============= INFO 1
    str_tanggal_hijriyah = readString(371);
  }
  else if (pilih == 7) { // ============= INFO 2
    str_tanggal_hijriyah = readString(521);
  }


  if ( !evt_2 && (millis() - p_e_2_hjry) > 75) {

    p_e_2_hjry = millis();
    display.setFont(System5x7);
    if (bsh > (-1 * (display.width() + display.textWidth(str_tanggal_hijriyah)))) {
      display.clear();

      // MASUK JAM DARI ATAS //
      if (bsh > -8) { // -1,-2,-3...-7
        display.drawText((display.width() - display.textWidth(tx_hh_mm)) / 2, (-8 + (-1 * bsh)), tx_hh_mm);
      }
      else if (bsh < (-1 * (display.width() + display.textWidth(str_tanggal_hijriyah))) + 8) {
        display.drawText((display.width() - display.textWidth(tx_hh_mm)) / 2, ((-1 * ((-1 * (display.width() + display.textWidth(str_tanggal_hijriyah))) + 8)) + bsh ), tx_hh_mm);
      }
      else if ((bsh < -7) && (bsh > (-1 * (display.width() + display.textWidth(str_tanggal_hijriyah))) + 7))
      {
        display.drawText((display.width() - display.textWidth(tx_hh_mm)) / 2, 0, tx_hh_mm);
      }

      display.drawText((bsh + 32), 8, str_tanggal_hijriyah);
      display.swapBuffers();
      bsh--;
      // Serial Print
//      if (bsh % 24 == 0)
//      {
//        //Serial.print(bsh); Serial.print(" "); Serial.println(str_tanggal_hijriyah);
//        Serial.println(str_tanggal_hijriyah);
//      }
      // AMBIL_WAKTU_SHOLAT();

    }
    else
    {
      if (pilih == 1) {
        String txt_kirim_tanggal = "RT=" + String(Konversi(Tanggal)) + "-" + String(Konversi(Bulan)) + "-" + String(Konversi(Tahun)) + "-" + String(hariIni);
        Serial.println(txt_kirim_tanggal);
        mode = 9; // TANGGAL HIJRIYAH >> TANGGAL MASEHI
      }
      else if (pilih == 2) {
        mode = 8; // TANGGAL MASEHI >> JAM BESAR
      }
      else if (pilih == 3) {
        mode = 10; // NAMA MASJID >> INFO
      }
      else if (pilih == 4) {
        mode = 1; // INFO >> JAM BESAR
      }
      else if (pilih == 5) {
        mode = 1; // JWS >> JAM BESAR
      }
      else if (pilih == 6) {
        mode = 11; // INFO 1 >> INFO 2
      }
      else if (pilih == 7) {
        mode = 3; // INFO 2 >> JAM BESAR
      }
      bsh = 0;
      flag = 0;

    }
  }
}
void TPL_HH_WKT_MSK() {               // MENAMPILKAN WAKTU MASUK ADZAN

  WAKTU();
  //    int dur_tpl_tx_sol;
  EEPROM.get(785, b_beep);
  int b_lama_adzan;
  EEPROM.get(59, b_lama_adzan);
  int b_lama_imsak;
  EEPROM.get(61, b_lama_imsak); // harus 0
  int b_lama;

  // Membedakan waktu masuk Imsak dan Jadwal sholat

  if (slt_sek == "IMSAK") {
    b_lama = b_lama_imsak + 1;
  }
  else
  {
    b_lama = b_lama_adzan;
  }


  if (!evt_4 && (millis() - p_e_4_jmsk) > 1000) {
    p_e_4_jmsk = millis();

    if (dur_tpl_tx_sol <= (60 * b_lama)) {

      if (dur_tpl_tx_sol < 10) { // imsak
        if (wkt_msk_skrg == 1) {
          if (b_beep == 1) {
            PORTD &= ~(1 << PORTD2);
            delay(500);
            PORTD |= (1 << PORTD2);
            delay(500);
            PORTD &= ~(1 << PORTD2);
          }
        }
        if (wkt_msk_skrg == 2) {
          if (b_beep == 1) {
            PORTD &= ~(1 << PORTD2);
            delay(500);
            PORTD |= (1 << PORTD2);
            delay(500);
            PORTD &= ~(1 << PORTD2);
          }
        }
        if (wkt_msk_skrg == 3 || wkt_msk_skrg == 4 || wkt_msk_skrg == 5 || wkt_msk_skrg == 6) {
          if (b_beep == 1) {
            PORTD &= ~(1 << PORTD2);
            delay(500);
            PORTD |= (1 << PORTD2);
            delay(500);
            PORTD &= ~(1 << PORTD2);
          }
        }
      }
      else if (dur_tpl_tx_sol == 11) {
        if (wkt_msk_skrg == 2) {
          EEPROM.get(69, b_volume_adzan);
          mp3.setVolume(b_volume_adzan);
          mp3.wakeup(2);
          mp3.playMP3Folder(2);
          //mp3.playTrack(2);

        }
        if (wkt_msk_skrg == 3 || wkt_msk_skrg == 4 || wkt_msk_skrg == 5 || wkt_msk_skrg == 6) {
          mp3.setVolume(b_volume_adzan);
          mp3.wakeup(2);
          mp3.playMP3Folder(1);
          //mp3.playTrack(1);
        }
      }

      display.clear();

      if (dur_tpl_tx_sol % 2 == 0) {
        display.setFont(System5x7);
        display.drawText((display.width() - display.textWidth(tx_hh_mm)) / 2, 0, tx_hh_mm); // baris 1 : JAM
        if (slt_sek == "IMSAK") {
          display.setFont(Font4x6);
          namaMasukWkt = slt_sek;
          //namaMasukWkt = "WAKTU " + slt_sek;
          display.drawText((display.width() - display.textWidth(namaMasukWkt)) / 2, 8, namaMasukWkt); // baris  : IMSAK
          // Serial.println("WAKTU");
        }
        else
        {
          display.setFont(Font4x6);
          namaMasukWkt = slt_sek;
          display.drawText((display.width() - display.textWidth(namaMasukWkt)) / 2, 8, namaMasukWkt); // baris  : ADZAN
          // Serial.println("ADZAN");
        }
        display.swapBuffers();
      }
      else
      {
        display.setFont(System5x7);
        display.drawText((display.width() - display.textWidth(tx_hh_mm)) / 2, 0, tx_hh_mm); // baris 1 :  JAM
        display.drawText((display.width() - display.textWidth(slt_sek)) / 2, 8, "  "); // baris  : JADWAL
        display.swapBuffers();
      }

      //Serial.print("WAKTU ");Serial.println(slt_sek);

      dur_tpl_tx_sol++;
    }
    else
    {
      if ((slt_sek == "IMSAK") || (slt_sek == "JUMAT")) {
        j_s_s = 0;
        dur_tpl_tx_sol = 0;
        wkt_msk_skrg = 0;
        mode = 1; //  memanggil JAM

      }
      else
      {
        j_s_s = 1;
        dur_tpl_tx_sol = 0;
        wkt_msk_skrg = 0;
        mode = 6; //  memanggil IQOMAH
      }
    }
  }
}
void TPL_QD_IQM() {                   // MENAMPILKAN IQOMAH
  WAKTU();
  EEPROM.get(785, b_beep);
  j_s_s = 0;

  int b_iqm_sbh, b_iqm_dzr, b_iqm_asr, b_iqm_mgr, b_iqm_isy;

  // Baca EEPROM tampil lama iqomah
  EEPROM.get(49, b_iqm_sbh);
  EEPROM.get(51, b_iqm_dzr);
  EEPROM.get(53, b_iqm_asr);
  EEPROM.get(55, b_iqm_mgr);
  EEPROM.get(57, b_iqm_isy);

  if (slt_sek == "SUBUH")
  {
    iqm_ee = (b_iqm_sbh * 60);
  }
  if (slt_sek == "DZUHUR" || slt_sek == "JUMAT")
  {
    iqm_ee = (b_iqm_dzr * 60);
  }
  if (slt_sek == "ASHAR")
  {
    iqm_ee = (b_iqm_asr * 60);
  }
  if (slt_sek == "MAGRIB")
  {
    iqm_ee = (b_iqm_mgr * 60);
  }
  if (slt_sek == "ISYA")
  {
    iqm_ee = (b_iqm_isy * 60);
  }

  if (!evt_5 && (millis() - p_e_5_iqomah) > 1000) {
    p_e_5_iqomah = millis();


    display.clear();
    if (du_iq <= iqm_ee) {

      // Buzzer pengingat akhir iqomah


      sis_wkt_iqo = iqm_ee - du_iq;

      iqm_detik = sis_wkt_iqo % 60;
      iqm_menit_sisa = (sis_wkt_iqo - iqm_detik) / 60;
      iqm_menit = iqm_menit_sisa % 60;
      if (iqm_detik < 10) {
        tpl_iqo_ss = "0" +  String(iqm_detik);
      }
      else
      {
        tpl_iqo_ss = String(iqm_detik);
      }

      if (iqm_detik % 2 == 0) {
        wkt_iqomah = "-" + String(iqm_menit) + ":" + tpl_iqo_ss;
      }
      else
      {
        wkt_iqomah = " " + String(iqm_menit) + ":" + tpl_iqo_ss;
      }

      // ==== TEMPAT BUAT BUZZER 10 DETIK TERAKHIR IQOMAH ==== //
      display.setFont(Font4x6);
      display.drawText((display.width() - display.textWidth("IQOMAH")) / 2, 0, "IQOMAH"); // JAM
      display.setFont(System5x7);
      textIqohitmun = wkt_iqomah;
      display.drawText((display.width() - display.textWidth(textIqohitmun)) / 2, 8, textIqohitmun); // baris  : ADZAN
      display.swapBuffers();

      if (b_beep == 1) {

        if (du_iq > (iqm_ee - 10) && du_iq < iqm_ee) {
          PORTD &= ~(1 << PORTD2);
          delay(500);
          PORTD |= (1 << PORTD2);
          delay(500);
          PORTD &= ~(1 << PORTD2);
        }
        // Buzzer pengingat akhir iqomah detik terakhir
        if (du_iq == iqm_ee) {
          PORTD &= ~(1 << PORTD2);
          delay(500);
          PORTD |= (1 << PORTD2);
          delay(2000);
          PORTD &= ~(1 << PORTD2);
        }
      }

//      Serial.print("IQMAH "); Serial.println(wkt_iqomah);

      du_iq++;

    }
    else
    {
      wkt_msk_skrg = 0;
      mode = 7;
      du_iq = 0;
    }

  }

}
void AMBIL_WAKTU_SHOLAT() {           // AMBIL WAKTU SHOLAT

  DateTime now = rtc.now();
  int sekarang = now.dayOfTheWeek();
  double b_lati, b_longi, b_timezone;

  EEPROM.get(0, b_lati);
  EEPROM.get(5, b_longi);
  EEPROM.get(10, b_timezone);

  EEPROM.get(25, b_a[0]); //imsak
  EEPROM.get(15, b_a[1]); //subuh
  EEPROM.get(27, b_a[2]); //syuruq
  EEPROM.get(29, b_a[3]); //dhuha
  EEPROM.get(17, b_a[4]); //dzuhur
  EEPROM.get(19, b_a[5]); //ashar
  EEPROM.get(21, b_a[6]); //maghrib
  EEPROM.get(23, b_a[7]); //isya

  SholahTimeCalulation sholat;
  sholat.coordinatSet(b_lati, b_longi, b_timezone);
  sholat.dateSet(now.year(), now.month(), now.day());
  sholat.ihtiyatiSet(0);
  sholat.calculate();

  int hh_sol, mm_sol;
  int mm_ims, hh_ims;
  int hh_sol_res, mm_sol_res;

  for (int ws = 0; ws < SholatCount; ws++) {

    sholat.getTimePart(ws, hh_sol, mm_sol);

    if (ws == 3) { // Dhuha
      // mm_sol = mm_sol + b_a[ws]-2;
      mm_sol = mm_sol + b_a[ws]; // Normal Dulu ya...
    }

    else if (ws == 2) { // Terbit
      // mm_sol = mm_sol + b_a[ws]-2;
      mm_sol = mm_sol + b_a[ws]; // Normal Dulu ya...
    }
    else
    {
      mm_sol = mm_sol + b_a[ws];
    }

    if (mm_sol >= 60) {
      mm_sol = mm_sol - 60;
      hh_sol ++;
    }

    String wkt_sol_sat =  String(hh_sol / 10) + String(hh_sol % 10) + ":" + String(mm_sol / 10) + String(mm_sol % 10);


    if (ws == 0) {
      waktu_ims = wkt_sol_sat;
    }
    if (ws == 1) {
      waktu_sbh = wkt_sol_sat;
    }
    if (ws == 2) {
      waktu_tbt = wkt_sol_sat;
    }
    if (ws == 3) {
      waktu_dhu = wkt_sol_sat;
    }
    if (ws == 4) {
      waktu_dzr = wkt_sol_sat;
    }
    if (ws == 5) {
      waktu_asr = wkt_sol_sat;
    }
    if (ws == 6) {
      waktu_mgr = wkt_sol_sat;
    }
    if (ws == 7) {
      waktu_isy = wkt_sol_sat;
    }

  }


  // CEK APAKAH SUDAH MASUK WAKTU SHOLAT //
  if (cck == waktu_ims && j_s_s == 0) {
    wkt_msk_skrg = 1;
    slt_sek = "IMSAK";
    bsh = 0; ash = 0;
    mode = 5;
  }

  if (cck == waktu_sbh && j_s_s == 0) {
    wkt_msk_skrg = 2;
    slt_sek = "SUBUH";
    bsh = 0; ash = 0;
    mode = 5;
  }
  if (cck == waktu_dzr && j_s_s == 0) {
    wkt_msk_skrg = 3;
    if (sekarang == 5) {
      slt_sek = "JUMAT";
      bsh = 0; ash = 0;
      mode = 5;
    }
    else
    {
      slt_sek = "DZUHUR";
      bsh = 0; ash = 0;
      mode = 5;
    }
  }

  if (cck == waktu_asr && j_s_s == 0) {
    wkt_msk_skrg = 4;
    slt_sek = "ASHAR";
    bsh = 0; ash = 0;
    mode = 5;
  }
  if (cck == waktu_mgr && j_s_s == 0) {
    wkt_msk_skrg = 5;
    slt_sek = "MAGRIB";
    bsh = 0; ash = 0;
    mode = 5;
  }
  if (cck == waktu_isy && j_s_s == 0) {
    wkt_msk_skrg = 6;
    slt_sek = "ISYA";
    bsh = 0; ash = 0;
    mode = 5;
  }

  if (Menit == 0 && Detik == 0 && wkt_msk_skrg == 0) {
    if (flag_mp3 == 0) {
      EEPROM.get(67, b_volume);
      mp3.setVolume(b_volume);
      mp3.wakeup(2);
      if (Jam == 1 || Jam == 13)      {
        mp3.playMP3Folder(5);
      }
      else if (Jam == 2 || Jam == 14) {
        mp3.playMP3Folder(6);
      }
      else if (Jam == 3 || Jam == 15) {
        mp3.playMP3Folder(7);
      }
      else if (Jam == 4 || Jam == 16) {
        mp3.playMP3Folder(8);
      }
      else if (Jam == 5 || Jam == 17) {
        mp3.playMP3Folder(9);
      }
      else if (Jam == 6 || Jam == 18) {
        mp3.playMP3Folder(10);
      }
      else if (Jam == 7 || Jam == 19) {
        mp3.playMP3Folder(11);
      }
      else if (Jam == 8 || Jam == 20) {
        mp3.playMP3Folder(12);
      }
      else if (Jam == 9 || Jam == 21) {
        mp3.playMP3Folder(13);
      }
      else if (Jam == 10 || Jam == 22) {
        mp3.playMP3Folder(14);
      }
      else if (Jam == 11 || Jam == 23) {
        mp3.playMP3Folder(15);
      }
      else if (Jam == 12 || Jam == 0) {
        mp3.playMP3Folder(16);
      }
      flag_mp3 = 1;
    }
  }

  if (Menit == 30 && Detik == 0 && wkt_msk_skrg == 0) {
    if (flag_mp3 == 0) {
      mp3.setVolume(b_volume);
      mp3.wakeup(2);
      mp3.playMP3Folder(18);
      flag_mp3 = 1;
    }
  }

  if (Detik == 3) {
    flag_mp3 = 0;
  }
}
void CERAH_VOLUME() {
  EEPROM.get(65, b_bright); // Baca EEPROM kecerahan
  Timer1.pwm(9, b_bright);
  EEPROM.get(67, b_volume); // Baca EEPROM volume
  EEPROM.get(69, b_volume_adzan); // Baca EEPROM volume adzan
  mp3.setVolume(b_volume);
}
void writeString(int address, String data) {
  int stringSize = data.length();

  for (int i = 0; i < stringSize; i++)
  {
    EEPROM.write(address + i, data[i]);
  }
  EEPROM.write(address + stringSize, '\0');  //Add termination null character
}
String readString(int address) {
  char data[100]; //Max 100 Bytes
  int len = 0;
  unsigned char k;
  k = EEPROM.read(address);
  while (k != '\0' && len < 100)  //Read until null character
  {
    k = EEPROM.read(address + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  return String(data);
}
