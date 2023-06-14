

// 功能:
// 1. 嘗試10次連線
// 2. 連線後可以點亮LED燈
// 3. 印出 MAC address --- 格式修正
// 4. 印出 local IP & Gateway IP
// 5. 感測數值並印出，有變化才輸出
// 6. 輸出格式改用準備上傳至 Things Speak網站之HTTP request message
// 7. 與ThingSpeak網站建立TCP 連線，並傳數值至網站上面 (數值不遺漏)

#include <SPI.h>
#include <WiFiNINA.h>

byte mac[6];
int val=0;
int val1=0;
int last_state=0;
String upload; 
int i;
int received=0;

int status=WL_IDLE_STATUS;
char ssid[]="iPhone CF";
char pass[]="cf205666";

char server[]="api.thingspeak.com";
WiFiClient client;


void print_MAC(){
   int i;
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
pinMode(12,OUTPUT);

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
     digitalWrite(12,HIGH);
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

      val1=0;
      upload="GET /update?key=XKNMFSU5N0WYZETA&field1="+String(val1);
      Serial.print("  :   ");
      Serial.println(upload);

      if (client.connect(server, 80)) {
          Serial.println("connected to server (in setup())");
          client.println(upload);
          client.println();
      }
      last_state=val1;
}



void loop() {

   // receiving data from server
    
      while (client.available()) {
         char c = client.read();
         i=(int) c;
         if (i>=48 && i<=57) received=1;
         Serial.write(c);
      }


  // To detect whether or not the uploading process is correct
     
if (received==1) {
  Serial.println();
  Serial.println("Upload scuess...!");
  
  // try to compose (or sense) the next uploading data
  val=analogRead(A0);
  val1=((float) val)/512;
   
  if (val1!=last_state) {
      Serial.print(val);
      Serial.print("  :   ");
      Serial.print(val1);
      last_state=val1;

      upload="GET /update?key=XKNMFSU5N0WYZETA&field1="+String(val1);
      Serial.print("  :   ");
      Serial.println(upload);
  }
   received=0;
   
   
  } // end of if (received==1) 
 else {   // the uploading process failed
   if (!client.connected()) {
    Serial.println();
    Serial.println("connection teardown");
    client.stop();

  if (client.connect(server, 80)) {
          Serial.println("connected to server ");
          client.println(upload);
          client.println();
      }

    
  }  // end of else clause

  
 }

  delay(7000);
}
