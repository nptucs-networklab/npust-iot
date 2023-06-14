int val,val1,last;

void setup() {
 Serial.begin(9600);
 last=0;
}

void loop() {
val=analogRead(A0);
val1=((float) val)/512;

if (val1!=last) {
Serial.print(val);
Serial.print("   :   ");
Serial.println(val1);
last=val1;
}

delay(5000);

}

