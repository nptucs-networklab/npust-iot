
#include <WiFiNINA.h>

int status=WL_IDLE_STATUS;
char ssid[]="iPhone CF";
char pass[]="cf205666";



void setup() {
  
int i=1;  
Serial.begin(9600);
pinMode(LED_BUILTIN,OUTPUT);
 digitalWrite(LED_BUILTIN,LOW);

while (status!=WL_CONNECTED && i<=10) {
  Serial.print("嘗試連線第 ");
  Serial.print(i);
  Serial.println("次...");
  
  status=WiFi.begin(ssid,pass);

  if (status!=WL_CONNECTED)
     Serial.println("     無法連線");
  else {
     Serial.println("     成功連線");
     digitalWrite(LED_BUILTIN,HIGH);
  }
  i++;
  delay(5000);
}

}

void loop() {
 
}
