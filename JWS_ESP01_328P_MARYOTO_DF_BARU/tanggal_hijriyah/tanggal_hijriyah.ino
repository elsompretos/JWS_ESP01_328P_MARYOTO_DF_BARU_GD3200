//208 bytes (10%)
#include "tangal_hijriyah.h"
String str_tanggal_hijriyah;

void setup(){

  Serial.begin (9600);
  str_tanggal_hijriyah = Kuwaiti_algorithm(20, 4, 2022, 3, 1);
  Serial.println(str_tanggal_hijriyah);
 
 
}
void loop(){


      
}
