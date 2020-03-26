#define PIR1_PIN D7
#define PIR2_PIN D6
#define PIR3_PIN D2
#define PIR4_PIN D1

void setup() {
  pinMode(PIR1_PIN,INPUT);
  pinMode(PIR2_PIN,INPUT);
  pinMode(PIR3_PIN,INPUT);
  pinMode(PIR4_PIN,INPUT);

  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {

  Serial.print(digitalRead(PIR1_PIN));
  Serial.print(" | ");
  Serial.print(digitalRead(PIR2_PIN));
  Serial.print(" | ");
  Serial.print(digitalRead(PIR3_PIN));
  Serial.print(" | ");
  Serial.print(digitalRead(PIR4_PIN));
  Serial.println("");
  
  // put your main code here, to run repeatedly:

}
