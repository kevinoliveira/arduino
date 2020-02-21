const int ledPin = A3;
const int pontentiometerPin = A0;

void setup(){
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(pontentiometerPin, INPUT);
}


int transform(int value){
  if(value <= 0) return 0;
  if(value >= 255) return 255;
  else return value / 4;
 }

void loop(){


  int pontentiometerValue = analogRead(pontentiometerPin);
  Serial.print("A");
  Serial.println(pontentiometerValue);
  Serial.print("B");
  Serial.println(transform(pontentiometerValue));
  analogWrite(ledPin,transform(pontentiometerValue));

}