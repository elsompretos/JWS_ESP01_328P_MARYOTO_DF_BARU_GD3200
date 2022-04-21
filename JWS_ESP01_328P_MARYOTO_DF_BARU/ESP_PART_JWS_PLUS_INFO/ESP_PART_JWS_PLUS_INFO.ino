/*
SJ=12:59-00-24-06-2020-01-10-15
LL=-6.21462|106.84513|7
NM=Masjid Akbar Maulana
IN=Sudahkah Anda Sholat
IQ=01-01-01-01-01-04
KR=02-02-02-02-02-02-02-02
 */

#include <SPI.h>
#include "FS.h" 
#include <ArduinoJson.h>

#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#include "WebPage4.h"

// ==== WIFI ====
const char* ssid = "JWSKU";
const char* password = "00000000";

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress netmask(255, 255, 255, 0);

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

String kirim;
String bulan, tanggal, tahun, jam, menit, hijri, cerah, volume, volume_adzan, adzan, iqosubuh, iqodzuhur, iqoashar, iqomaghrib, iqoisya, beep_status;
String korimsak, korsubuh, korterbit, kordhuha, kordzuhur, korashar, kormaghrib, korisya;
String tpl_imsak, tpl_subuh, tpl_terbit, tpl_dhuha, tpl_dzuhur, tpl_ashar, tpl_maghrib, tpl_isya;
String tpl_namamasjid, tpl_info1, tpl_info2, tpl_stlh_iqomah;

//==== JSON ====
const char *fileadjhijri = "/adjhijrii.json";
int adjhijr, adjcerah, adjvolume, adjvolumeadzan, beepstatus;

const char *fileiqomah = "/iqomah.json";
int lamaadzan, lamaimsak, iqomahsubuh, iqomahdzuhur, iqomahashar,iqomahmaghrib, iqomahisya;

const char *filemasjid = "/masjidinfo.json";
char namamasjid[145], info1[145], info2[145], stlhiqomah[145]; 

const char *filelatilong = "/latilong.json";
float latitude, longitude; int zonawaktu;

const char *filekoreksi = "/koreksi.json";
int b_a[8];

const char *filetampil = "/tampil.json";
int b_t[8];

const char *filetplinfo = "/tampilinfo.json";
int tplnamamasjid, tplinfo1, tplinfo2, tplstlhiqomah;


//==== XML ====
String XML;
String XML_IQOMAH;
String XML_MASJID;
String XML_KOORDINAT;
String XML_KOREKSI;
String XML_TAMPIL;
String XML_TAMPILINFO;

void setup() {
  Serial.begin(9600);

   // =====================
  // ==== FILE SYSTEM ====
  // =====================
  //  LittleFS.begin();
  SPIFFS.begin();
  loadHijriConfig(fileadjhijri);
  loadLatiLongConfig(filelatilong);
  loadMasjidConfig(filemasjid);
  loadIqomahConfig(fileiqomah); 
  loadKoreksi(filekoreksi);
  loadTampil(filetampil);
  loadTampilinfo(filetplinfo);

  
  // ==============
  // ==== WIFI ====
  // ==============
  
  WiFi.softAPdisconnect(true);
  WiFi.disconnect();
  delay(1000);

  Serial.println("Wifi ACCESS POINT");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, netmask);
  WiFi.softAP(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  server.on("/", []() {
     server.send_P(200, "text/html", setwaktu);

    if (server.hasArg("date")) {
       // date=2020-06-24&time=10%3A20&hijr=1&cerah=10&volume=15
       // date=2021-03-10&time=01%3A01&adjhijr=1&adjcerah=10&adjvolumeadzan=15&adjvolume=20 == New : 10/03/2021

       tanggal = (server.arg(0)).substring(8,10);
       bulan = (server.arg(0)).substring(5,7);
       tahun = (server.arg(0)).substring(0,4);
       
       if ((server.arg(2)).toInt() < 10){ hijri = "0" + server.arg(2);  } else { hijri = server.arg(2);  }
       if ((server.arg(3)).toInt() < 10){ cerah = "0" + server.arg(3);  } else { cerah = server.arg(3);  }
       if ((server.arg(4)).toInt() < 10){ volume_adzan = "0" + server.arg(4); } else { volume_adzan = server.arg(4); }
       if ((server.arg(5)).toInt() < 10){ volume = "0" + server.arg(5); } else { volume = server.arg(5); }
       if ((server.arg(6)).toInt() < 10){ beep_status = "0" + server.arg(6); } else { beep_status = server.arg(6); }
   
       kirim = "SJ=" + server.arg(1) + "-00-" + tanggal + "-" + bulan + "-" + tahun + "-" + hijri + "-" + cerah + "-" + volume + "-" + volume_adzan + "-" + beep_status;
  
       Serial.println(kirim);

    } 
     
    if (server.hasArg("tgl_auto")) {
       // tgl_auto=2020-07-03&wkt_auto=09%3A06

       tanggal = (server.arg(0)).substring(8,10);
       bulan = (server.arg(0)).substring(5,7);
       tahun = (server.arg(0)).substring(0,4);
       
       if ((server.arg(2)).toInt() < 10){ hijri = "0" + server.arg(2);  } else { hijri = server.arg(2);  }
       if ((server.arg(3)).toInt() < 10){ cerah = "0" + server.arg(3);  } else { cerah = server.arg(3);  }
       if ((server.arg(4)).toInt() < 10){ volume_adzan = "0" + server.arg(4); } else { volume_adzan = server.arg(4); }
       if ((server.arg(5)).toInt() < 10){ volume = "0" + server.arg(5); } else { volume = server.arg(5); }
   
       
   
       kirim = "SA=" + server.arg(1) + "-00-" + tanggal + "-" + bulan + "-" + tahun;
       Serial.println(kirim);

    }  

    if (server.hasArg("namamasjid")) {
      // namamasjid=Masjid+Akbar+Maulana
      kirim = "NM=" + server.arg(0);
      Serial.println(kirim); 
    }

    if (server.hasArg("info1")) {
      kirim = "IN=" + server.arg(0);
      Serial.println(kirim);
    }

    if (server.hasArg("info2")) {
      kirim = "I2=" + server.arg(0);
      Serial.println(kirim);
    }

    if (server.hasArg("stlhiqomah")) {
      kirim = "SI=" + server.arg(0);
      Serial.println(kirim);
    }

    if (server.hasArg("latitude")) {
      // latitude=-6.21462&longitude=106.84513&timezone=7 
      kirim = "LL=" + server.arg(0) + "|" + server.arg(1) + "|" + server.arg(2);
      Serial.println(kirim);
    }

    if (server.hasArg("lamaadzan")) {
      // lamaadzan=4&iqomahsubuh=1&iqomahdzuhur=1&iqomahashar=1&iqomahmaghrib=1&iqomahisya=1
      
      if ((server.arg(0)).toInt() < 10){ adzan = "0" + server.arg(0);  } else { adzan = server.arg(0);  }
      if ((server.arg(1)).toInt() < 10){ iqosubuh = "0" + server.arg(1);  } else { iqosubuh = server.arg(1);  }
      if ((server.arg(2)).toInt() < 10){ iqodzuhur = "0" + server.arg(2); } else { iqodzuhur = server.arg(2); }
      if ((server.arg(3)).toInt() < 10){ iqoashar = "0" + server.arg(3);  } else { iqoashar = server.arg(3);  }
      if ((server.arg(4)).toInt() < 10){ iqomaghrib = "0" + server.arg(4);  } else { iqomaghrib = server.arg(4);  }
      if ((server.arg(5)).toInt() < 10){ iqoisya = "0" + server.arg(5); } else { iqoisya = server.arg(5); }
      
      kirim = "IQ=" + iqosubuh + "-" + iqodzuhur + "-" + iqoashar + "-" + iqomaghrib  + "-" + iqoisya + "-" + adzan;
      Serial.println(kirim);
    }

    if (server.hasArg("korimsak")) {
      // korimsak=2&korsubuh=2&korterbit=2&kordhuha=2&kordzuhur=2&korashar=2&kormaghrib=2&korisya=2
      // KR=02-02-02-02-02-02-00-02 == 02-00-02 imsak dan dhuha
      
      if ((server.arg(0)).toInt() < 10){ korimsak = "0" + server.arg(0);  } else { korimsak = server.arg(0);  }
      if ((server.arg(1)).toInt() < 10){ korsubuh = "0" + server.arg(1);  } else { korsubuh = server.arg(1);  }
      if ((server.arg(2)).toInt() < 10){ korterbit = "0" + server.arg(2); } else { korterbit = server.arg(2); }
      if ((server.arg(3)).toInt() < 10){ kordhuha = "0" + server.arg(3);  } else { kordhuha = server.arg(3);  }
      if ((server.arg(4)).toInt() < 10){ kordzuhur = "0" + server.arg(4);  } else { kordzuhur = server.arg(4);  }
      if ((server.arg(5)).toInt() < 10){ korashar = "0" + server.arg(5); } else { korashar = server.arg(5); }
      if ((server.arg(6)).toInt() < 10){ kormaghrib = "0" + server.arg(6);  } else { kormaghrib = server.arg(6);  }
      if ((server.arg(7)).toInt() < 10){ korisya = "0" + server.arg(7); } else { korisya = server.arg(7); }
      
      kirim = "KR=" + korsubuh + "-" + kordzuhur + "-" + korashar + "-" + kormaghrib  + "-" + korisya + "-" + korimsak + "-" + korterbit  + "-" + kordhuha;
      Serial.println(kirim);
    }

    if (server.hasArg("tpl_imsak")) {
      
      if ((server.arg(0)).toInt() < 10){ tpl_imsak = "0" + server.arg(0);  } else { tpl_imsak = server.arg(0);  }
      if ((server.arg(1)).toInt() < 10){ tpl_subuh = "0" + server.arg(1);  } else { tpl_subuh = server.arg(1);  }
      if ((server.arg(2)).toInt() < 10){ tpl_dzuhur = "0" + server.arg(2);  } else { tpl_dzuhur = server.arg(2);  }
      if ((server.arg(3)).toInt() < 10){ tpl_ashar = "0" + server.arg(3); } else { tpl_ashar = server.arg(3); }
      if ((server.arg(4)).toInt() < 10){ tpl_maghrib = "0" + server.arg(4);  } else { tpl_maghrib = server.arg(4);  }
      if ((server.arg(5)).toInt() < 10){ tpl_isya = "0" + server.arg(5); } else { tpl_isya = server.arg(5); }
      
      //kirim = "TP=" + tpl_imsak + "-" + tpl_subuh + "-" + tpl_terbit + "-" + tpl_dhuha  + "-" + tpl_dzuhur + "-" + tpl_ashar + "-" + tpl_maghrib  + "-" + tpl_isya;
      kirim = "TP=" + tpl_imsak + "-" + tpl_subuh + "-00-00" + "-" + tpl_dzuhur + "-" + tpl_ashar + "-" + tpl_maghrib  + "-" + tpl_isya;
      Serial.println(kirim);
    }
    
    if (server.hasArg("tpl_namamasjid")) {

      // file:///C:/Users/creative1/Desktop/index.html?tpl_namamasjid=1&tpl_info1=1&tpl_stlh_iqomah=1&tpl_info2=1
      
      if ((server.arg(0)).toInt() < 10){ tpl_namamasjid = "0" + server.arg(0);  } else { tpl_namamasjid = server.arg(0);  }
      if ((server.arg(1)).toInt() < 10){ tpl_info1 = "0" + server.arg(1);  } else { tpl_info1 = server.arg(1);  }
      if ((server.arg(2)).toInt() < 10){ tpl_stlh_iqomah = "0" + server.arg(2);  } else { tpl_stlh_iqomah = server.arg(2);  }
      if ((server.arg(3)).toInt() < 10){ tpl_info2 = "0" + server.arg(3);  } else { tpl_info2 = server.arg(3);  }
      
      kirim = "TI=" + tpl_namamasjid + "-" + tpl_info1 + "-" + tpl_info2 + "-" + tpl_stlh_iqomah;
      Serial.println(kirim);
    }
    
  });
  
  server.on("/xmlwaktu", handleXMLWaktu); // http://192.168.4.1/xmlwaktu
  server.on("/xmlkoordinat", handleXMLKoordinat); // http://192.168.4.1/xmlkoordinat
  server.on("/xmlinfomasjid", handleXMLInfoMasjid); // http://192.168.4.1/xmlinfomasjid
  server.on("/xmliqomah", handleXMLIqomah); // http://192.168.4.1/xmliqomah
  server.on("/xmlkoreksi", handleXMLKoreksi); // http://192.168.4.1/xmlkoreksi
  server.on("/xmltampil", handleXMLTampil); // http://192.168.4.1/xmltampil
  server.on("/xmltampilinfo", handleXMLTampilinfo); // http://192.168.4.1/xmltampilinfo


  server.on("/simpanWaktu", HTTP_POST, handleSimpanWaktu);
  server.on("/simpanKoordinat", HTTP_POST, handleSimpanKoordinat);
  server.on("/simpanNamamasjid", HTTP_POST, handleSimpaNamamasjid);
  server.on("/simpanIqomah", HTTP_POST, handleSimpanIqomah);
  server.on("/simpanKoreksi", HTTP_POST, handleSimpanKoreksi);
  server.on("/simpanTampil", HTTP_POST, handleSimpanTampil);
  server.on("/simpanTampilinfo", HTTP_POST, handleSimpanTampilinfo);
  
  // ================
  // ==== SERVER ====
  // ================

  httpUpdater.setup(&server);
  server.begin();
  Serial.println("HTTP server started"); 

}

void loop() {
  server.handleClient();
}

void loadHijriConfig(const char *fileadjhijri){     // Load Penyesuaian Tanggal Hijriyah //
  File configFilehijr = SPIFFS.open(fileadjhijri, "r");
  
  if (!configFilehijr) {
    Serial.println("Failed to open config file");
    makeHijriConfig();
    Serial.println("Sistem restart...");
    ESP.restart();
  }

  size_t size = configFilehijr.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFilehijr.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());

  if (error) {
    Serial.println("Gagal parse sethijri");
    return;
  }
  
  adjhijr = doc["adjhijr"];
  adjcerah = doc["adjcerah"];
  adjvolumeadzan = doc["adjvolumeadzan"];
  adjvolume = doc["adjvolume"];
  beepstatus = doc["beepstatus"];
  
  Serial.println("=========================================");
  Serial.print("Adj Hijriyah : "); Serial.println(adjhijr);
  Serial.print("Adj Kecerahan : "); Serial.println(adjcerah);
  Serial.print("Adj Volume Adzan : "); Serial.println(adjvolumeadzan);
  Serial.print("Adj Volume : "); Serial.println(adjvolume);
  Serial.print("Beep Status : "); Serial.println(beepstatus);

  

  configFilehijr.close();
  
}
void loadLatiLongConfig(const char *filelatilong){  //         Load Koordinat            //
  File configLatiLong = SPIFFS.open(filelatilong, "r");
  
  if (!configLatiLong) {
    Serial.println("Gagal membuka pengaturan Koordinat");
    makeLatiLongConfig();
    Serial.println("Sistem restart...");
    ESP.restart();
  }

  size_t size = configLatiLong.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configLatiLong.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());

  if (error) {
    Serial.println("Gagal parse pengaturan Koordinat");
    return;
  }

  latitude = doc["latitude"];
  longitude = doc["longitude"];
  zonawaktu = doc["zonawaktu"];   
  

  Serial.println("=========================================");
  Serial.print("latitude : "); Serial.println(latitude);
  Serial.print("longitude : "); Serial.println(longitude);
  Serial.print("zonawaktu : "); Serial.println(zonawaktu);
  
  configLatiLong.close();
}
void loadMasjidConfig(const char *filemasjid){      //     Load Nama Masjid dan Info     //
  File configMasjid = SPIFFS.open(filemasjid, "r");
  
  if (!configMasjid) {
    Serial.println("Gagal membuka pengaturan Masjid dan Info");
    makeMasjidConfig();
    Serial.println("Sistem restart...");
    ESP.restart();
  }

  size_t size = configMasjid.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configMasjid.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());

  if (error) {
    Serial.println("Gagal parse pengaturan Masjid dan Info");
    return;
  }

  strlcpy(namamasjid, doc["namamasjid"] | "", sizeof(namamasjid));  
  strlcpy(info1, doc["info1"] | "", sizeof(info1));  
  strlcpy(info2, doc["info2"] | "", sizeof(info2)); 
  strlcpy(stlhiqomah, doc["stlhiqomah"] | "", sizeof(stlhiqomah));    

  Serial.println("=========================================");
  Serial.print("nama masjid : "); Serial.println(namamasjid);
  Serial.print("info1 : "); Serial.println(info1);
  Serial.print("info2 : "); Serial.println(info2);
  Serial.print("stlh iqomah : "); Serial.println(stlhiqomah);



  configMasjid.close();
}
void loadIqomahConfig(const char *fileiqomah){      //     Load Lama Adzan dan Iqomah    //
  File configFileIqomah = SPIFFS.open(fileiqomah, "r");
  
  if (!configFileIqomah) {
    Serial.println("Failed to open config file");
    makeIqomahConfig();
    Serial.println("Sistem restart...");
    ESP.restart();
  }

  size_t size = configFileIqomah.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFileIqomah.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());

  if (error) {
    Serial.println("Gagal parse fileiqomah");
    return;
  }

  lamaadzan = doc["lamaadzan"];
  lamaimsak = doc["lamaimsak"];
  iqomahsubuh = doc["iqomahsubuh"];
  iqomahdzuhur = doc["iqomahdzuhur"];
  iqomahashar = doc["iqomahashar"];
  iqomahmaghrib = doc["iqomahmaghrib"];
  iqomahisya = doc["iqomahisya"];

  Serial.println("=========================================");
  Serial.print("lama adzan : "); Serial.println(lamaadzan);
  Serial.print("lama imsak : "); Serial.println(lamaimsak);
  Serial.print("iqomah subuh : "); Serial.println(iqomahsubuh);
  Serial.print("iqomah dzuhur : "); Serial.println(iqomahdzuhur);
  Serial.print("iqomah ashar : "); Serial.println(iqomahashar);
  Serial.print("iqomah maghrib : "); Serial.println(iqomahmaghrib);
  Serial.print("iqomah isya : "); Serial.println(iqomahisya);

  configFileIqomah.close();
  
}
void loadKoreksi(const char *filekoreksi){          //          Load Koreksi             //
  File configKoreksi = SPIFFS.open(filekoreksi, "r");
  
  if (!configKoreksi) {
    Serial.println("Gagal membuka pengaturan Koreksi Waktu");
    makeKoreksi();
    Serial.println("Sistem restart...");
    ESP.restart();
  }

  size_t size = configKoreksi.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configKoreksi.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());

  if (error) {
    Serial.println("Gagal parse pengaturan Koreksi Waktu");
    return;
  }

  b_a[0] = doc["korimsak"];
  b_a[1] = doc["korsubuh"];
  b_a[2] = doc["korterbit"]; 
  b_a[3] = doc["kordhuha"];
  b_a[4] = doc["kordzuhur"];
  b_a[5] = doc["korashar"];
  b_a[6] = doc["kormaghrib"];
  b_a[7] = doc["korisya"];  

     // TEST AJA //
     // b_a[2] = b_a[2] - 2;
     // b_a[3] = b_a[3] - 1;
     
  Serial.println("=========================================");
  Serial.print("kor imsak : "); Serial.println(b_a[0]);
  Serial.print("kor subuh : "); Serial.println(b_a[1]);
  Serial.print("kor terbit : "); Serial.println(b_a[2]);
  Serial.print("kor dhuha : "); Serial.println(b_a[3]);
  Serial.print("kor dzuhur : "); Serial.println(b_a[4]);
  Serial.print("kor ashar : "); Serial.println(b_a[5]);
  Serial.print("kor maghrib : "); Serial.println(b_a[6]);
  Serial.print("kor isya : "); Serial.println(b_a[7]);
  
  configKoreksi.close();
}
void loadTampil(const char *filetampil){            //          Load Tampil              //
  File configTampil = SPIFFS.open(filetampil, "r");
  
  if (!configTampil) {
    Serial.println("Gagal membuka pengaturan Koreksi Waktu");
    makeTampil();
    Serial.println("Sistem restart...");
    ESP.restart();
  }

  size_t size = configTampil.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configTampil.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());

  if (error) {
    Serial.println("Gagal parse pengaturan Koreksi Tampilan");
    return;
  }

  b_t[0] = doc["tpl_imsak"];
  b_t[1] = doc["tpl_subuh"];
//  b_t[2] = doc["tpl_terbit"]; 
//  b_t[3] = doc["tpl_dhuha"];
  b_t[4] = doc["tpl_dzuhur"];
  b_t[5] = doc["tpl_ashar"];
  b_t[6] = doc["tpl_maghrib"];
  b_t[7] = doc["tpl_isya"];  

     
  Serial.println("=========================================");
  Serial.print("tpl imsak : "); Serial.println(b_t[0]);
  Serial.print("tpl subuh : "); Serial.println(b_t[1]);
//  Serial.print("tpl terbit : "); Serial.println(b_t[2]);
//  Serial.print("tpl dhuha : "); Serial.println(b_t[3]);
  Serial.print("tpl dzuhur : "); Serial.println(b_t[4]);
  Serial.print("tpl ashar : "); Serial.println(b_t[5]);
  Serial.print("tpl maghrib : "); Serial.println(b_t[6]);
  Serial.print("tpl isya : "); Serial.println(b_t[7]);
  
  configTampil.close();
}
void loadTampilinfo(const char *filetplinfo){       //          Load Tampil              //
  File configTampilinfo = SPIFFS.open(filetplinfo, "r");
  
  if (!configTampilinfo) {
    Serial.println("Gagal membuka pengaturan Tampilan Info");
    makeTampilinfo();
    Serial.println("Sistem restart...");
    ESP.restart();
  }

  size_t size = configTampilinfo.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configTampilinfo.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());

  if (error) {
    Serial.println("Gagal parse pengaturan Tampilan Info");
    return;
  }


  tplnamamasjid = doc["tplnamamasjid"];
  tplinfo1 = doc["tplinfo1"];
  tplinfo2 = doc["tplinfo2"];
  tplstlhiqomah = doc["tplstlhiqomah"];


  Serial.println("=========================================");
  Serial.print("tampil nama masjid : "); Serial.println(tplnamamasjid);
  Serial.print("tampil info1 : "); Serial.println(tplinfo1);
  Serial.print("tampil info2 : "); Serial.println(tplinfo2);
  Serial.print("setelah iqomah : "); Serial.println(tplstlhiqomah);


  configTampilinfo.close();
}

void makeHijriConfig(){                             //        Kalau Gak Ada Buat...      //
  String dataawal = "{\"adjhijr\":1,\"adjcerah\":10,\"adjvolumeadzan\":15,\"adjvolume\":20,\"beepstatus\":1}";

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataawal);

  File configFileAdjHijri = SPIFFS.open(fileadjhijri, "w");
  if (!configFileAdjHijri) {
    Serial.println("Gagal membuat file adjhijrii.json untuk ditulis mungkin partisi belum dibuat");
    return;
  }
  
  serializeJson(doc, configFileAdjHijri);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    configFileAdjHijri.close();
    Serial.println("Berhasil membuat file configjws.json");
  
  }   
}
void makeLatiLongConfig(){ 
   
  String dataawalkoordinat = "{\"latitude\": \"-6.21462\",\"longitude\": \"106.84513\",\"zonawaktu\": \"7\"}";

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataawalkoordinat);

  File makeFileKoordinat = SPIFFS.open(filelatilong, "w");
  if (!makeFileKoordinat) {
    Serial.println("Gagal membuat file latilong.json untuk ditulis mungkin partisi belum dibuat");
    return;
  }
  
  serializeJson(doc, makeFileKoordinat);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    makeFileKoordinat.close();
    Serial.println("Berhasil membuat file iqomah.json");
  
  }   
}
void makeMasjidConfig(){  
    
  //String dataawalmasjid = "{\"namamasjid\": \"Masjid Raya Akbar Maulana\",\"info1\": \"Info1\",\"info2\": \"Info2\",\"info3\": \"Info3\"}";
  String dataawalmasjid = "{\"namamasjid\": \"Masjid Raya Akbar Maulana\",\"info1\": \"Info1\",\"info2\": \"Info2\", \"stlhiqomah\": \"stlhiqomah\"}";

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataawalmasjid);

  File makeFileMasjid = SPIFFS.open(filemasjid, "w");
  if (!makeFileMasjid) {
    Serial.println("Gagal membuat file masjidinfo.json untuk ditulis mungkin partisi belum dibuat");
    return;
  }
  
  serializeJson(doc, makeFileMasjid);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    makeFileMasjid.close();
    Serial.println("Berhasil membuat file iqomah.json");
  
  }   
}
void makeIqomahConfig(){
      
  String dataawaliqomah = "{\"lamaadzan\": \"1\",\"lamaimsak\": \"1\",\"iqomahsubuh\": \"1\",\"iqomahdzuhur\": \"1\",\"iqomahashar\": \"1\",\"iqomahmaghrib\": \"1\",\"iqomahisya\": \"1\"}";

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataawaliqomah);

  File makeFileIqomah = SPIFFS.open(fileiqomah, "w");
  if (!makeFileIqomah) {
    Serial.println("Gagal membuat file iqomah.json untuk ditulis mungkin partisi belum dibuat");
    return;
  }
  
  serializeJson(doc, makeFileIqomah);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    makeFileIqomah.close();
    Serial.println("Berhasil membuat file iqomah.json");
  
  }   
}
void makeKoreksi(){ 
   
  String dataawalkoreksi = "{\"korimsak\": \"2\",\"korsubuh\": \"2\",\"korterbit\": \"0\",\"kordhuha\": \"0\",\"kordzuhur\": \"2\",\"korashar\": \"2\",\"kormaghrib\": \"2\",\"korisya\": \"2\"}";

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataawalkoreksi);

  File makeFileKoreksi = SPIFFS.open(filekoreksi, "w");
  if (!makeFileKoreksi) {
    Serial.println("Gagal membuat file koreksi.json untuk ditulis mungkin partisi belum dibuat");
    return;
  }
  
  serializeJson(doc, makeFileKoreksi);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    makeFileKoreksi.close();
    Serial.println("Berhasil membuat file koreksi.json");
  
  }   
}
void makeTampil(){
  String dataawaltampil = "{\"tpl_imsak\": \"1\",\"tpl_subuh\": \"1\",\"tpl_dzuhur\": \"1\",\"tpl_ashar\": \"1\",\"tpl_maghrib\": \"1\",\"tpl_isya\": \"1\"}";

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataawaltampil);

  File makeFileTampil = SPIFFS.open(filetampil, "w");
  if (!makeFileTampil) {
    Serial.println("Gagal membuat file tampil.json untuk ditulis mungkin partisi belum dibuat");
    return;
  }
  
  serializeJson(doc, makeFileTampil);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    makeFileTampil.close();
    Serial.println("Berhasil membuat file tapil.json");
  
  }   
}
void makeTampilinfo(){  
    
  String dataawaltampilinfo = "{\"tplnamamasjid\": \"1\",\"tplinfo1\": \"0\",\"tplinfo2\": \"0\",\"tplstlhiqomah\": \"0\"}";

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataawaltampilinfo);

  File makeFiletampilinfo = SPIFFS.open(filetplinfo, "w");
  if (!makeFiletampilinfo) {
    Serial.println("Gagal membuat file tampilinfo.json untuk ditulis mungkin partisi belum dibuat");
    return;
  }
  
  serializeJson(doc, makeFiletampilinfo);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    makeFiletampilinfo.close();
    Serial.println("Berhasil membuat file tampilinfo.json");
  
  }   
}


void XMLWaktu(){
  XML="<?xml version='1.0'?>";
  XML+="<t>";
    XML+="<Hijriyah>";
    XML+= adjhijr;
    XML+="</Hijriyah>"; 

    XML+="<Kecerahan>";
    XML+= adjcerah;
    XML+="</Kecerahan>"; 

    XML+="<VolumeAdzan>";
    XML+= adjvolumeadzan;
    XML+="</VolumeAdzan>"; 

    XML+="<Volume>";
    XML+= adjvolume;
    XML+="</Volume>"; 

    XML+="<BeepStatus>";
    XML+= beepstatus;
    XML+="</BeepStatus>"; 

    
  XML+="</t>"; 
}
void handleXMLWaktu(){
  XMLWaktu();
  server.send(200,"text/xml",XML);
}
void XMLKoordinat(){

  XML_KOORDINAT="<?xml version='1.0'?>";
  XML_KOORDINAT+="<t>";
  
    XML_KOORDINAT+="<Latitude>";
    XML_KOORDINAT+= latitude;
    XML_KOORDINAT+="</Latitude>"; 

    XML_KOORDINAT+="<Longitude>";
    XML_KOORDINAT+= longitude;
    XML_KOORDINAT+="</Longitude>"; 

    XML_KOORDINAT+="<Timezone>";
    XML_KOORDINAT+= zonawaktu;
    XML_KOORDINAT+="</Timezone>"; 
    
  XML_KOORDINAT+="</t>"; 
}
void handleXMLKoordinat(){
  XMLKoordinat();
  server.send(200,"text/xml",XML_KOORDINAT);
}
void XMLInfoMasjid(){

  XML_MASJID="<?xml version='1.0'?>";
  XML_MASJID+="<t>";
  
    XML_MASJID+="<NamaMasjid>";
    XML_MASJID+= namamasjid;
    XML_MASJID+="</NamaMasjid>"; 

    XML_MASJID+="<InfoSatu>";
    XML_MASJID+= info1;
    XML_MASJID+="</InfoSatu>"; 

    XML_MASJID+="<InfoDua>";
    XML_MASJID+= info2;
    XML_MASJID+="</InfoDua>";

    XML_MASJID+="<StlhIqomah>";
    XML_MASJID+= stlhiqomah;
    XML_MASJID+="</StlhIqomah>";

    
  XML_MASJID+="</t>"; 
}
void handleXMLInfoMasjid(){
  XMLInfoMasjid();
  server.send(200,"text/xml", XML_MASJID);
}
void XMLIqomah(){

  XML_IQOMAH="<?xml version='1.0'?>";
  XML_IQOMAH+="<t>";
  
    XML_IQOMAH+="<LamZan>";
    XML_IQOMAH+= lamaadzan;
    XML_IQOMAH+="</LamZan>"; 

    XML_IQOMAH+="<LamSak>";
    XML_IQOMAH+= lamaimsak;
    XML_IQOMAH+="</LamSak>"; 

    XML_IQOMAH+="<ISub>";
    XML_IQOMAH+= iqomahsubuh;
    XML_IQOMAH+="</ISub>"; 

    XML_IQOMAH+="<IDzu>";
    XML_IQOMAH+= iqomahdzuhur;
    XML_IQOMAH+="</IDzu>";

    XML_IQOMAH+="<IAshr>";
    XML_IQOMAH+= iqomahashar;
    XML_IQOMAH+="</IAshr>"; 

    XML_IQOMAH+="<IMgr>";
    XML_IQOMAH+= iqomahmaghrib ;
    XML_IQOMAH+="</IMgr>"; 

    XML_IQOMAH+="<IIsy>";
    XML_IQOMAH+= iqomahisya;
    XML_IQOMAH+="</IIsy>"; 
    
  XML_IQOMAH+="</t>"; 
}
void handleXMLIqomah(){
  XMLIqomah();
  server.send(200,"text/xml", XML_IQOMAH);
}
void XMLKoreksi(){

  XML_KOREKSI="<?xml version='1.0'?>";
  XML_KOREKSI+="<t>";
  
    XML_KOREKSI+="<KorImsak>";
    XML_KOREKSI+= b_a[0];
    XML_KOREKSI+="</KorImsak>"; 

    XML_KOREKSI+="<KorSubuh>";
    XML_KOREKSI+= b_a[1];
    XML_KOREKSI+="</KorSubuh>"; 

    XML_KOREKSI+="<KorTerbit>";
    XML_KOREKSI+= b_a[2];
    XML_KOREKSI+="</KorTerbit>"; 

    XML_KOREKSI+="<KorDhuha>";
    XML_KOREKSI+= b_a[3];
    XML_KOREKSI+="</KorDhuha>";

    XML_KOREKSI+="<KorDzuhur>";
    XML_KOREKSI+= b_a[4];
    XML_KOREKSI+="</KorDzuhur>"; 

    XML_KOREKSI+="<KorAshar>";
    XML_KOREKSI+= b_a[5];
    XML_KOREKSI+="</KorAshar>"; 

    XML_KOREKSI+="<KorMaghrib>";
    XML_KOREKSI+= b_a[6];
    XML_KOREKSI+="</KorMaghrib>"; 

    XML_KOREKSI+="<KorIsya>";
    XML_KOREKSI+= b_a[7];
    XML_KOREKSI+="</KorIsya>"; 
    
  XML_KOREKSI+="</t>"; 
}
void handleXMLKoreksi(){
  XMLKoreksi();
  server.send(200,"text/xml", XML_KOREKSI);
}
void XMLTampil(){

  XML_TAMPIL="<?xml version='1.0'?>";
  XML_TAMPIL+="<t>";
  
    XML_TAMPIL+="<TplImsak>";
    XML_TAMPIL+= b_t[0];
    XML_TAMPIL+="</TplImsak>"; 

    XML_TAMPIL+="<TplSubuh>";
    XML_TAMPIL+= b_t[1];
    XML_TAMPIL+="</TplSubuh>"; 

    XML_TAMPIL+="<TplDzuhur>";
    XML_TAMPIL+= b_t[4];
    XML_TAMPIL+="</TplDzuhur>"; 

    XML_TAMPIL+="<TplAshar>";
    XML_TAMPIL+= b_t[5];
    XML_TAMPIL+="</TplAshar>"; 

    XML_TAMPIL+="<TplMaghrib>";
    XML_TAMPIL+= b_t[6];
    XML_TAMPIL+="</TplMaghrib>"; 

    XML_TAMPIL+="<TplIsya>";
    XML_TAMPIL+= b_t[7];
    XML_TAMPIL+="</TplIsya>"; 
    
  XML_TAMPIL+="</t>"; 
}
void handleXMLTampil(){
  XMLTampil();
  server.send(200,"text/xml", XML_TAMPIL);
}
void XMLTampilinfo(){

  XML_TAMPILINFO="<?xml version='1.0'?>";
  XML_TAMPILINFO+="<t>";
  
    XML_TAMPILINFO+="<TplNama>";
    XML_TAMPILINFO+= tplnamamasjid;
    XML_TAMPILINFO+="</TplNama>"; 

    XML_TAMPILINFO+="<TplInfo1>";
    XML_TAMPILINFO+= tplinfo1;
    XML_TAMPILINFO+="</TplInfo1>"; 

    XML_TAMPILINFO+="<TplInfo2>";
    XML_TAMPILINFO+= tplinfo2;
    XML_TAMPILINFO+="</TplInfo2>"; 
    
    XML_TAMPILINFO+="<TplStlhiqomah>";
    XML_TAMPILINFO+= tplstlhiqomah;
    XML_TAMPILINFO+="</TplStlhiqomah>"; 
    
  XML_TAMPILINFO+="</t>"; 
}
void handleXMLTampilinfo(){
  XMLTampilinfo();
  server.send(200,"text/xml", XML_TAMPILINFO);
}


void handleSimpanWaktu() {

  String datainfoWaktu = server.arg("plain");
  
  Serial.println(datainfoWaktu);
  
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, datainfoWaktu);

  File configFileWaktu = SPIFFS.open(fileadjhijri, "w");

  if (!configFileWaktu) {
    Serial.println("Gagal membuka File info untuk ditulis");
    return;
  }

  serializeJson(doc, configFileWaktu);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    configFileWaktu.close();
    Serial.println("Berhasil mengubah configFileJws");

    loadHijriConfig(fileadjhijri);
  }   
}
void handleSimpanKoordinat() {

  String datainfoKoordinat = server.arg("plain");
  
  Serial.println(datainfoKoordinat);
  
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, datainfoKoordinat);

  File configFileKoordinat = SPIFFS.open(filelatilong, "w");

  if (!configFileKoordinat) {
    Serial.println("Gagal membuka File info untuk ditulis");
    return;
  }

  serializeJson(doc, configFileKoordinat);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    configFileKoordinat.close();
    Serial.println("Berhasil mengubah configFileJws");

    loadLatiLongConfig(filelatilong);
  }   
}
void handleSimpaNamamasjid(){
   String datainfoNamamasjid = server.arg("plain");
  
  Serial.println(datainfoNamamasjid);
  
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, datainfoNamamasjid);

  File configFileNamamasjid = SPIFFS.open(filemasjid, "w");

  if (!configFileNamamasjid) {
    Serial.println("Gagal membuka File info untuk ditulis");
    return;
  }

  serializeJson(doc, configFileNamamasjid);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    configFileNamamasjid.close();
    Serial.println("Berhasil mengubah configFileJws");

    loadMasjidConfig(filemasjid);
  }   
}
void handleSimpanIqomah(){
String datainfoIqomah = server.arg("plain");
  
  Serial.println(datainfoIqomah);
  
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, datainfoIqomah);

  File configFileIqomah = SPIFFS.open(fileiqomah, "w");

  if (!configFileIqomah) {
    Serial.println("Gagal membuka File info untuk ditulis");
    return;
  }

  serializeJson(doc, configFileIqomah);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    configFileIqomah.close();
    Serial.println("Berhasil mengubah configFileJws");

    loadIqomahConfig(fileiqomah); 
  }   
}
void handleSimpanKoreksi(){
  String datainfoKoreksi = server.arg("plain");
  
  Serial.println(datainfoKoreksi);
  
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, datainfoKoreksi);

  File configFileKoreksi = SPIFFS.open(filekoreksi, "w");

  if (!configFileKoreksi) {
    Serial.println("Gagal membuka File info untuk ditulis");
    return;
  }

  serializeJson(doc, configFileKoreksi);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    configFileKoreksi.close();
    Serial.println("Berhasil mengubah configFileJws");

    loadKoreksi(filekoreksi);
  }   
}
void handleSimpanTampil(){
  String datainfoTampil = server.arg("plain");
  
  Serial.println(datainfoTampil);
  
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, datainfoTampil);

  File configFileTampil = SPIFFS.open(filetampil, "w");

  if (!configFileTampil) {
    Serial.println("Gagal membuka File Tampil untuk ditulis");
    return;
  }

  serializeJson(doc, configFileTampil);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    configFileTampil.close();
    Serial.println("Berhasil mengubah configFileJws");

    loadTampil(filetampil);
  }   
}
void handleSimpanTampilinfo(){
String dataTampilinfo = server.arg("plain");

  Serial.print("isi tampil info : ");
  Serial.println(dataTampilinfo);
  
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataTampilinfo);

  File configTampilinfo = SPIFFS.open(filetplinfo, "w");

  if (!configTampilinfo) {
    Serial.println("Gagal membuka File info untuk ditulis");
    return;
  }

  serializeJson(doc, configTampilinfo);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    configTampilinfo.close();
    Serial.println("Berhasil mengubah configFileJws");

    loadTampilinfo(filetplinfo); 
  }   
}
