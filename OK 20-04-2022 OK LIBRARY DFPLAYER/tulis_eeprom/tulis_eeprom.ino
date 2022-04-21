#include <EEPROM.h>

void setup() {

  Serial.begin(9600);

  //  LATITUDE DAN LONGITUDE
  //  ======================

  double latitude = -6.27533;
  EEPROM.put(0, latitude);
  double longitude = 106.70116;
  EEPROM.put(5, longitude);
  double tz = 7;
  EEPROM.put(10, tz);


  //  ADJUST WAKTU SHOLAT
  //  ===================

  int adj_subuh = 2; EEPROM.put(15, adj_subuh);
  int adj_dzuhur = 2; EEPROM.put(17, adj_dzuhur);
  int adj_ashar = 2; EEPROM.put(19, adj_ashar);
  int adj_maghrib = 2; EEPROM.put(21, adj_maghrib);
  int adj_isya = 2; EEPROM.put(23, adj_isya);
  int adj_imsak = 0; EEPROM.put(25, adj_imsak);
  int adj_terbit = 0; EEPROM.put(27, adj_terbit);
  int adj_dhuha = 2; EEPROM.put(29, adj_dhuha);
  

  //  TAMPILKAN WAKTU SHOLAT
  //  ======================

  int tpl_subuh = 1; EEPROM.put(32, tpl_subuh);
  int tpl_dzuhur = 1; EEPROM.put(34, tpl_dzuhur);
  int tpl_ashar = 1; EEPROM.put(36, tpl_ashar);
  int tpl_maghrib = 1; EEPROM.put(38, tpl_maghrib);
  int tpl_isya = 1; EEPROM.put(40, tpl_isya);
  int tpl_imsak = 1; EEPROM.put(42, tpl_imsak);
  int tpl_terbit = 0; EEPROM.put(44, tpl_terbit);
  int tpl_dhuha = 0; EEPROM.put(46, tpl_dhuha);

  //  LAMA IQOMAH
  //  ===========

  int iqm_subuh = 2; EEPROM.put(49, iqm_subuh);
  int iqm_dzuhur = 2; EEPROM.put(51, iqm_dzuhur);
  int iqm_ashar = 2; EEPROM.put(53, iqm_ashar);
  int iqm_maghrib = 2; EEPROM.put(55, iqm_maghrib);
  int iqm_isya = 2; EEPROM.put(57, iqm_isya);
  int lama_adzan = 4; EEPROM.put(59, lama_adzan);



  //  LAMA IMSAK
  //  ================

  int lama_imsak = 0; EEPROM.put(61, lama_imsak);

   
  //  KOREKSI HIJRIYAH
  //  ================

  int sesuaikan_tgl_hijriyah = 1; EEPROM.put(63, sesuaikan_tgl_hijriyah); 
  int sesuaikan_bright = 10; EEPROM.put(65, sesuaikan_bright);
  int sesuaikan_volume = 20; EEPROM.put(67, sesuaikan_volume);
  int sesuaikan_volume_adzan = 0; EEPROM.put(69, sesuaikan_volume_adzan);

  writeString(71, "Mushola Tetep Iman Almh. Hj. Sri Loeloet Binti Mangun Digdo Di Gunung Wiyu"); // NAMA MASJID
  writeString(221, "Matikan HP - Luruskan Shaf - Matikan HP - Luruskan Shaf");                   // SETELAH IQOMAH
  writeString(371, "Jadikan Sabar dan Sholat Sebagai Penolong mu");                              // INFO 1
  writeString(521, "Sudahkah anda sholat ??");                                                   // INFO 2
  writeString(671, "Selamat Hari Raya Idul Adha 10 Dzuhijjah 1442 H");                           // HARI BESAR
  
 
  int tpl_nama = 1; EEPROM.put(773, tpl_nama);
  int tpl_info1 = 0; EEPROM.put(775, tpl_info1);
  int tpl_info2 = 0; EEPROM.put(777, tpl_info2);
  int tpl_stlh_iqomah = 0; EEPROM.put(779, tpl_stlh_iqomah);

  int beep_status = 1; EEPROM.put(785, beep_status);

}

void loop() {
  

}
void writeString(int address, String data){
  int stringSize = data.length();
  
  for(int i=0;i<stringSize;i++)
  {
    EEPROM.write(address+i, data[i]);
  }
  EEPROM.write(address + stringSize,'\0');   //Add termination null character
}
String readString(int address){
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k = EEPROM.read(address);
  while(k != '\0' && len < 100)   //Read until null character
  {
    k = EEPROM.read(address + len);
    data[len] = k;
    len++;
  }
  data[len]='\0';
  return String(data);
}
