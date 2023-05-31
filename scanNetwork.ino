

// 功能:
// 1. 嘗試10次連線
// 2. 印出 MAC address --- 格式修正
// 3. 印出 local IP & Gateway IP
// 4. 準備上傳至 Things Speak網站之HTTP request message (要求下載數值)
// 5. 與Things Speak網站建立TCP 連線，並從網站下載數值回來                
// 6. 根據數值結果決定亮燈 -- 1則亮，0則暗                          

#include <SPI.h>
#include <WiFiNINA.h>

byte mac[6];
int val=0;
int val1=8;
int last_state=8;
String upload; 
int i=0;

int status=WL_IDLE_STATUS;
char ssid[]="iPhone CF";
char pass[]="cf205666";

char server[]="api.thingspeak.com";
WiFiClient client;


void print_MAC(){
 
  // WiFi.macAddress(mac);
    Serial.print("MAC:");
    for (i=5;i>=1;i--){
       if (mac[i]<16)  Serial.print("0");
      Serial.print(mac[i],HEX);   
       Serial.print(":");
    }
    Serial.println(mac[0],HEX);
  
}


void setup() {
  // put your setup code here, to run once:
int i=1;  
Serial.begin(9600);
pinMode(LED_BUILTIN,OUTPUT);

// 嘗試連線

while (status!=WL_CONNECTED && i<=10) {
  Serial.print("嘗試連線第 ");
  Serial.print(i);
  Serial.println("次...");
  
  status=WiFi.begin(ssid,pass);

  if (status!=WL_CONNECTED)
     Serial.println("     無法連線");
  else 
     Serial.println("     成功連線");

  i++;
  delay(5000);
}

  if (status==WL_CONNECTED){
     WiFi.macAddress(mac);
     print_MAC();
     Serial.println("");

     IPAddress ip=WiFi.localIP();
     Serial.print("IP Address: ");
     Serial.println(ip);
     Serial.println("");
     
     IPAddress gateway=WiFi.gatewayIP();
     Serial.print("Gateway Address: ");
     Serial.println(gateway);
     Serial.println(""); 
  }

  
      upload="GET /channels/2165358/fields/1/last.txt";
      Serial.print("  :   ");
      Serial.println(upload);

      if (client.connect(server, 80)) {
          Serial.println("connected to server ");
          client.println(upload);
          client.println();
      } 
}



void loop() { 
         while (client.available()) {
         char c = client.read();
         Serial.print("reading incoming data :  ");
         Serial.write(c);
         i=(int) c-48;
         Serial.println("");
         Serial.println(i);
         if (i==1) digitalWrite(LED_BUILTIN,HIGH); else digitalWrite(LED_BUILTIN,LOW);
      }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.print("connection teardown");
   // client.stop();

  if (client.connect(server, 80)) {
          Serial.println("connected to server ");
          client.println(upload);
          client.println();
      }

    
  }
 
  delay(5000);

}
