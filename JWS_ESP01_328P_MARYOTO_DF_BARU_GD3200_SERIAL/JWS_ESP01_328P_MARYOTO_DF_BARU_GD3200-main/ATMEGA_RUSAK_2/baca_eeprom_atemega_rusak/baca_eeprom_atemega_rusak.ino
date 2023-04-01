#include <EEPROM.h>
double  blatitude, blongitude, btz;
int badj_subuh, badj_dzuhur, badj_ashar, badj_maghrib, badj_isya, badj_imsak, badj_terbit, badj_dhuha;
int btpl_subuh, btpl_dzuhur, btpl_ashar, btpl_maghrib, btpl_isya, btpl_imsak, btpl_terbit, btpl_dhuha;
int biqm_subuh, biqm_dzuhur, biqm_ashar, biqm_maghrib, biqm_isya, blam_zan, blama_imsak;
int bsesuaikan_tgl_hijriyah, bsesuaikan_bright, bsesuaikan_volume, bsesuaikan_volume_adzan, bbeep_status;
int b_tpl_nama, b_tpl_info1, b_tpl_info2, b_tpl_stlh_iqomah;

String nama, setelah_iqomah, hari_besar;
String info1, info2;


void setup() {

  Serial.begin(9600);

  //  LATITUDE DAN LONGITUDE
  //  ======================

  EEPROM.get(787, blatitude);
  Serial.print("787 Latitude : "); Serial.println(blatitude);
  EEPROM.get(792, blongitude);
  Serial.print("792 Longitude : "); Serial.println(blongitude);
  EEPROM.get(802, btz);
  Serial.print("802 Timezone : "); Serial.println(btz); Serial.println();
 
  //  ADJUST WAKTU SHOLAT
  //  ===================
  
  EEPROM.get(807, badj_subuh); Serial.print("807 adj_subuh : "); Serial.println(badj_subuh);
  EEPROM.get(809, badj_dzuhur); Serial.print("809 adj_dzuhur : "); Serial.println(badj_dzuhur);
  EEPROM.get(811, badj_ashar); Serial.print("811 adj_ashar : "); Serial.println(badj_ashar);
  EEPROM.get(813, badj_maghrib); Serial.print("813 adj_maghrib : "); Serial.println(badj_maghrib); 
  EEPROM.get(815, badj_isya); Serial.print("815 adj_isya : "); Serial.println(badj_isya);
  EEPROM.get(817, badj_imsak); Serial.print("817 adj_imsak : "); Serial.println(badj_imsak);
  EEPROM.get(819, badj_terbit); Serial.print("819 adj_terbit : "); Serial.println(badj_terbit);
  EEPROM.get(821, badj_dhuha); Serial.print("821 adj_dhuha : "); Serial.println(badj_dhuha);
  Serial.println();

  //  TAMPILKAN WAKTU SHOLAT
  //  ======================

  EEPROM.get(32, btpl_subuh); Serial.print("32 tpl_subuh : "); Serial.println(btpl_subuh);
  EEPROM.get(34, btpl_dzuhur); Serial.print("34 tpl_dzuhur : "); Serial.println(btpl_dzuhur);
  EEPROM.get(36, btpl_ashar); Serial.print("36 tpl_ashar : "); Serial.println(btpl_ashar);
  EEPROM.get(38, btpl_maghrib); Serial.print("38 tpl_maghrib : "); Serial.println(btpl_maghrib); 
  EEPROM.get(40, btpl_isya); Serial.print("40 tpl_isya : "); Serial.println(btpl_isya);
  EEPROM.get(42, btpl_imsak); Serial.print("42 tpl_imsak : "); Serial.println(btpl_imsak);
  EEPROM.get(44, btpl_terbit); Serial.print("44 tpl_terbit : "); Serial.println(btpl_terbit);
  EEPROM.get(46, btpl_dhuha); Serial.print("46 tpl_dhuha : "); Serial.println(btpl_dhuha);
  Serial.println();

  //  LAMA IQOMAH
  //  ===========

  EEPROM.get(49, biqm_subuh); Serial.print("49 iqm_subuh : "); Serial.println(biqm_subuh); 
  EEPROM.get(51, biqm_dzuhur); Serial.print("51 iqm_dzuhur : "); Serial.println(biqm_dzuhur);
  EEPROM.get(53, biqm_ashar); Serial.print("53 iqm_ashar : "); Serial.println(biqm_ashar);
  EEPROM.get(55, biqm_maghrib); Serial.print("55 iqm_maghrib : "); Serial.println(biqm_maghrib);
  EEPROM.get(57, biqm_isya); Serial.print("57 iqm_isya : "); Serial.println(biqm_isya);
  EEPROM.get(59, blam_zan); Serial.print("59 lama_adzan : "); Serial.println(blam_zan);
  Serial.println();

  //  LAMA IMSAK
  //  ================

  EEPROM.get(61, blama_imsak); Serial.print("61 lama_imsak : "); Serial.println(blama_imsak);
  Serial.println(); 
   
  //  KOREKSI HIJRIYAH
  //  ================

  EEPROM.get(63, bsesuaikan_tgl_hijriyah); Serial.print("63 sesuaikan_tgl_hijriyah : "); Serial.println(bsesuaikan_tgl_hijriyah);
  EEPROM.get(65, bsesuaikan_bright); Serial.print("65 sesuaikan_bright : "); Serial.println(bsesuaikan_bright);
  EEPROM.get(67, bsesuaikan_volume); Serial.print("67 sesuaikan_volume : "); Serial.println(bsesuaikan_volume);
  EEPROM.get(69, bsesuaikan_volume_adzan); Serial.print("69 sesuaikan_volume_adzan : "); Serial.println(bsesuaikan_volume_adzan);
  Serial.println();

  nama = readString(71);
  Serial.print("71 Nama Masjid  : "); Serial.println(nama);
  setelah_iqomah = readString(221);
  Serial.print("221 Setelah Iqomah : "); Serial.println(setelah_iqomah);
  info1 = readString(371);
  Serial.print("371 Informasi 1 : "); Serial.println(info1);
  info2 = readString(521);
  Serial.print("521 Informasi 2 : "); Serial.println(info2);
  hari_besar = readString(671);
  Serial.print("671 Hari Besar : "); Serial.println(hari_besar);
  

  
  
  Serial.println();

  EEPROM.get(785, bbeep_status); Serial.print("785 beep_status : "); Serial.println(bbeep_status);
  Serial.println();

  b_tpl_nama, b_tpl_info1, b_tpl_info2;


  EEPROM.get(773, b_tpl_nama); Serial.print("773 tpl_nama : "); Serial.println(b_tpl_nama);
  EEPROM.get(775, b_tpl_info1); Serial.print("775 tpl_info1 : "); Serial.println(b_tpl_info1);
  EEPROM.get(777, b_tpl_info2); Serial.print("777 tpl_info2 : "); Serial.println(b_tpl_info2);
  EEPROM.get(779, b_tpl_stlh_iqomah); Serial.print("779 tpl_stlh_iqomah : "); Serial.println(b_tpl_stlh_iqomah);
  Serial.println();


  
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
