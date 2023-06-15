int val,val1;

void setup() {
 Serial.begin(9600);
}

void loop() {
val=analogRead(A0);
val1=((float) val)/171;
Serial.print(val);
Serial.print("   :   ");
Serial.println(val1);

delay(5000);

}

