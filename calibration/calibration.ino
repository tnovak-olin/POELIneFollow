void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  //reads numbers higher than 700 when on the line and lower than 400 when off the line
  Serial.print("Left");
  Serial.print(analogRead(A5));
  //reads numbers higher than 650 when on the line and lower than 450 when off the line
  Serial.print("Right");
  Serial.println(analogRead(A0));
  
  // put your main code here, to run repeatedly:

}
